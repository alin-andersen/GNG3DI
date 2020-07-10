#include "Widgets/GNG3DIOpenGLWidget.h"

#include "Widgets/GNG3DIMainWidget.h"
#include "Windows/GNG3DIWindow.h"

#include "glm/ext.hpp"

#include <QtGlobal>
#include <QStyleOption>

char* load_file(const char* path, off_t* size)
{
    FILE* file = fopen(path, "r");
    if(file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    char* data = (char*) malloc(*size+1);

    for(int i = 0; i < *size; i++)
        fread((void*)&data[i], 1, 1, file);
    data[*size] = '\0';

    return data;
}

GNG3DIOpenGLWidget::GNG3DIOpenGLWidget(QWidget* parent, GNG3DConfig* config) : QOpenGLWidget()
{
    this->config = config->Clone();
    setMouseTracking(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(UpdateAnimation()));
    m_matrix = glm::mat4(1);
}

GNG3DIOpenGLWidget::~GNG3DIOpenGLWidget()
{
    if(config != NULL) delete config;

    makeCurrent();
    gl->glDeleteProgram(shader_node);
    gl->glDeleteProgram(shader_edge);
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &vbo_node);
    gl->glDeleteBuffers(1, &vbo_edge);
}

void GNG3DIOpenGLWidget::SetConfig(GNG3DConfig* config)
{
    mutex.lock();

    if(this->config != NULL) delete this->config;
    this->config = config->Clone();

    mutex.unlock();

    update();
}

void GNG3DIOpenGLWidget::SetOther(GNG3DIOpenGLWidget* other)
{
    this->other = other;
}

int GNG3DIOpenGLWidget::LoadShader(const char* path, int type)
{
    off_t size;
    char* data = load_file(path, &size);

    int shader = gl->glCreateShader(type);
    gl->glShaderSource(shader, 1, &data, (GLint*)&size);
    gl->glCompileShader(shader);

    GLint success = 0;
    gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint logSize = 0;
        gl->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        char* message = (char*)malloc(logSize);
        gl->glGetShaderInfoLog(shader, logSize, &logSize, message);
        qDebug() << "ERROR: opengl compiliation: " << QString(message);
        free(message);
    }
    free(data);

    return shader;
}

void GNG3DIOpenGLWidget::initializeGL()
{
    mutex.lock();

    gl = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if(!gl)
    {
        mutex.unlock();
        qDebug("Could not access opengl 3.3 functions!");
        return;
    }

    gl->glClearColor(0.3, 0.3, 0.3, 1.0);
    //gl->glClearColor(1, 1, 1, 1.0);

    gl->glGenVertexArrays(1, &vao);
    gl->glBindVertexArray(vao);

    // setup node shader

    int shader_vertex_node   = LoadShader("./node.vsh", GL_VERTEX_SHADER);
    int shader_fragment_node = LoadShader("./node.fsh", GL_FRAGMENT_SHADER);

    shader_node = gl->glCreateProgram();
    gl->glAttachShader(shader_node, shader_vertex_node);
    gl->glAttachShader(shader_node, shader_fragment_node);
    gl->glBindAttribLocation(shader_node, 0, "position");
    gl->glBindAttribLocation(shader_node, 1, "type");
    gl->glLinkProgram(shader_node);
    gl->glUseProgram(shader_node);

    gl->glDeleteShader(shader_vertex_node);
    gl->glDeleteShader(shader_fragment_node);

    v_matrix_uniform_node =        gl->glGetUniformLocation(shader_node, "v_matrix");
    w_matrix_uniform_node =        gl->glGetUniformLocation(shader_node, "w_matrix");
    m_matrix_uniform_node =        gl->glGetUniformLocation(shader_node, "m_matrix");
    color_no_winner_uniform_node = gl->glGetUniformLocation(shader_node, "color_no_winner");
    color_signal_uniform_node =    gl->glGetUniformLocation(shader_node, "color_signal");
    color_winner_uniform_node =    gl->glGetUniformLocation(shader_node, "color_winner");
    color_winner2_uniform_node =   gl->glGetUniformLocation(shader_node, "color_winner2");
    point_size_uniform_node =      gl->glGetUniformLocation(shader_node, "point_size");
    depth_steepness_uniform_node = gl->glGetUniformLocation(shader_node, "depth_steepness");
    depth_offset_uniform_node =    gl->glGetUniformLocation(shader_node, "depth_offset");

    // setup edge shader

    int shader_vertex_edge   = LoadShader("./edge.vsh", GL_VERTEX_SHADER);
    int shader_fragment_edge = LoadShader("./edge.fsh", GL_FRAGMENT_SHADER);

    shader_edge = gl->glCreateProgram();
    gl->glAttachShader(shader_edge, shader_vertex_edge);
    gl->glAttachShader(shader_edge, shader_fragment_edge);
    gl->glBindAttribLocation(shader_edge, 0, "position");
    gl->glLinkProgram(shader_edge);
    gl->glUseProgram(shader_edge);

    gl->glDeleteShader(shader_vertex_edge);
    gl->glDeleteShader(shader_fragment_edge);

    v_matrix_uniform_edge =        gl->glGetUniformLocation(shader_edge, "v_matrix");
    w_matrix_uniform_edge =        gl->glGetUniformLocation(shader_edge, "w_matrix");
    m_matrix_uniform_edge =        gl->glGetUniformLocation(shader_edge, "m_matrix");
    line_color_uniform_edge =      gl->glGetUniformLocation(shader_edge, "line_color");
    mode_uniform_edge =            gl->glGetUniformLocation(shader_edge, "mode");
    depth_steepness_uniform_edge = gl->glGetUniformLocation(shader_edge, "depth_steepness");
    depth_offset_uniform_edge =    gl->glGetUniformLocation(shader_edge, "depth_offset");
    age_steepness_uniform_edge =   gl->glGetUniformLocation(shader_edge, "age_steepness");
    age_offset_uniform_edge =      gl->glGetUniformLocation(shader_edge, "age_offset");

    mutex.unlock();
}

void GNG3DIOpenGLWidget::SetDepth(float v_min, float v_max)
{
    depth_steepness = (max - min) / (v_max - v_min);
    depth_offset = max - ((max - min) / (v_max - v_min)) * v_max;
}

void GNG3DIOpenGLWidget::SetEdgeDepth(float v_min, float v_max)
{
    depth_steepness_edge = (max - min) / (v_max - v_min);
    depth_offset_edge = max - ((max - min) / (v_max - v_min)) * v_max;
}

void GNG3DIOpenGLWidget::SetAgeFunc(float age_min_value, float age_max_value)
{
    age_steepness = (age_max - age_min) / (age_max_value - age_min_value);
    age_offset = age_max - ((age_max - age_min) / (age_max_value - age_min_value)) * age_max_value;
}


void GNG3DIOpenGLWidget::paintGL()
{
    if(!gl) return;

    mutex.lock();

    // render nodes

    gl->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    gl->glEnable(GL_PROGRAM_POINT_SIZE);
    gl->glEnable(GL_DEPTH_TEST);
    gl->glDepthFunc(GL_LESS);

    w_matrix = glm::mat4(1);
    w_matrix = glm::translate(w_matrix, glm::vec3(-x_offset,y_offset,-distance));
    w_matrix = glm::rotate(w_matrix, rot_x, glm::vec3(1.0,0.0,0.0));
    w_matrix = glm::rotate(w_matrix, rot_y, glm::vec3(0.0,1.0,0.0));

    ViewMatrix();

    if(!config->skip_rendering)
    {
        if(vertices_count <= 0)
        {
            mutex.unlock();
            return;
        }

        gl->glUseProgram(shader_node);

        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_node);
        gl->glEnableVertexAttribArray(0);
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_node_types);
        gl->glEnableVertexAttribArray(1);
        gl->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

        gl->glUniformMatrix4fv(v_matrix_uniform_node, 1, GL_FALSE, glm::value_ptr(v_matrix));
        gl->glUniformMatrix4fv(w_matrix_uniform_node, 1, GL_FALSE, glm::value_ptr(w_matrix));
        gl->glUniformMatrix4fv(m_matrix_uniform_node, 1, GL_FALSE, glm::value_ptr(m_matrix));

        struct Vec3 color_no_winner;
        color_no_winner.x = (float) config->no_winner_color_node.red() / 255.0;
        color_no_winner.y = (float) config->no_winner_color_node.green() / 255.0;
        color_no_winner.z = (float) config->no_winner_color_node.blue() / 255.0;

        struct Vec3 color_signal;
        color_signal.x = (float) config->signal_color_node.red() / 255.0;
        color_signal.y = (float) config->signal_color_node.green() / 255.0;
        color_signal.z = (float) config->signal_color_node.blue() / 255.0;

        struct Vec3 color_winner;
        color_winner.x = (float) config->winner_color_node.red() / 255.0;
        color_winner.y = (float) config->winner_color_node.green() / 255.0;
        color_winner.z = (float) config->winner_color_node.blue() / 255.0;

        struct Vec3 color_winner2;
        color_winner2.x = (float) config->winner2_color_node.red() / 255.0;
        color_winner2.y = (float) config->winner2_color_node.green() / 255.0;
        color_winner2.z = (float) config->winner2_color_node.blue() / 255.0;

        gl->glUniform3fv(color_no_winner_uniform_node, 1, (GLfloat*) &color_no_winner);
        gl->glUniform3fv(color_signal_uniform_node,    1, (GLfloat*) &color_signal);
        gl->glUniform3fv(color_winner_uniform_node,    1, (GLfloat*) &color_winner);
        gl->glUniform3fv(color_winner2_uniform_node,   1, (GLfloat*) &color_winner2);

        gl->glUniform1f(point_size_uniform_node, config->point_size_node);

        gl->glUniform1f(depth_steepness_uniform_node, depth_steepness);
        gl->glUniform1f(depth_offset_uniform_node, depth_offset);

        gl->glDrawArrays(GL_POINTS, 0, vertices_count);

        // render edges

        if(edges_count <= 0)
        {
            mutex.unlock();
            return;
        }

        gl->glUseProgram(shader_edge);

        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_edge);
        gl->glEnableVertexAttribArray(0);
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_edge_ages);
        gl->glEnableVertexAttribArray(1);
        gl->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

        gl->glUniformMatrix4fv(v_matrix_uniform_edge, 1, GL_FALSE, glm::value_ptr(v_matrix));
        gl->glUniformMatrix4fv(w_matrix_uniform_edge, 1, GL_FALSE, glm::value_ptr(w_matrix));
        gl->glUniformMatrix4fv(m_matrix_uniform_edge, 1, GL_FALSE, glm::value_ptr(m_matrix));

        struct Vec3 line_color_edge;
        line_color_edge.x = (float) config->line_color_edge.red()   / 255.0;
        line_color_edge.y = (float) config->line_color_edge.green() / 255.0;
        line_color_edge.z = (float) config->line_color_edge.blue()  / 255.0;

        gl->glUniform3fv(line_color_uniform_edge, 1, (GLfloat*) &line_color_edge);
        if(config->age_rendering) gl->glUniform1f(mode_uniform_edge, 1.0f);
        else gl->glUniform1f(mode_uniform_edge, 0.0f);
        gl->glUniform1f(depth_steepness_uniform_edge, depth_steepness_edge);
        gl->glUniform1f(depth_offset_uniform_edge, depth_offset_edge);
        gl->glUniform1f(age_steepness_uniform_edge, age_steepness);
        gl->glUniform1f(age_offset_uniform_edge, age_offset);

        gl->glDrawArrays(GL_LINES, 0, edges_count);
    }

    mutex.unlock();
}

void GNG3DIOpenGLWidget::ViewMatrix()
{
    float aspect = ((float) width()) / ((float) height());
    v_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 10000.0f);
}

void GNG3DIOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();

    mutex.lock();

    if(grab)
    {
        if(event->type() == QMouseEvent::MouseButtonPress)
        {
            if(event->button() == Qt::RightButton)
            {
                if(xy_movement) xy_movement = false;
                else xy_movement = true;

                mutex.unlock();
                return;
            }
        }
    }

    if(grab == false)
    { 
        // only grab if left mouse button
        if(event->type() == QMouseEvent::MouseButtonPress)
        {
            if(event->button() == Qt::LeftButton)
            {
                grab       = true;
                xy_movement = false;
                setCursor(Qt::BlankCursor);
                grabMouse();
                grabKeyboard();

                QCursor c = cursor();
                c.setPos(mapToGlobal(QPoint(width()/2.0,height()/2.0)));
                setCursor(c);
            }
        }
    }
    else
    {
        // only ungrab if left mouse button
        if(event->type() == QMouseEvent::MouseButtonPress)
        {
            if(event->button() == Qt::LeftButton)
            {
                grab       = false;
                xy_movement = false;
                setCursor(Qt::ArrowCursor);
                releaseMouse();
                releaseKeyboard();

                QCursor c = cursor();
                c.setPos(mapToGlobal(QPoint(width()/2.0,height()/2.0)));
                setCursor(c);
            }
        }
    }

    old_x = width() / 2.0;
    old_y = height() / 2.0;

    mutex.unlock();
}

void GNG3DIOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();

    if(grab == true)
    {
        int x = event->pos().rx();
        int y = event->pos().ry();

        QCursor c = cursor();
        QPoint pos = c.pos();

        if(pos.rx() == old_x && pos.ry() == old_y) return;

        mutex.lock();

        float rel_x = 0.01 * (old_x - x);
        float rel_y = 0.01 * (old_y - y);

        if(xy_movement)
        {
            x_offset += rel_x;
            y_offset += rel_y;
        }
        else
        {
            rot_x -= rel_y;
            rot_y -= rel_x;

            rot_x = fmod(rot_x, 360);
            rot_y = fmod(rot_y, 360);

            old_x = width() / 2.0;
            old_y = height() / 2.0;
        }

        c.setPos(mapToGlobal(QPoint(width()/2.0,height()/2.0)));
        setCursor(c);

        mutex.unlock();

        update();
        UpdateOther();
    }
}

void GNG3DIOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    event->accept();

    if(grab == true)
    {
        if(event->type() == QKeyEvent::KeyPress)
        {
            QKeyEvent* key = static_cast<QKeyEvent*>(event);

            switch(key->key())
            {

            case Qt::Key_Left:
                mutex.lock();
                rot_y -= 0.2;
                mutex.unlock();
                break;

            case Qt::Key_Right:
                mutex.lock();
                rot_y += 0.2;
                mutex.unlock();
                break;

            case Qt::Key_Up:
                mutex.lock();
                rot_x -= 0.2;
                mutex.unlock();
                break;

            case Qt::Key_Down:
                mutex.lock();
                rot_x += 0.2;
                mutex.unlock();
                break;

            case Qt::Key_R:
                mutex.lock();
                rot_x = 0.0;
                rot_y = 0.0;
                mutex.unlock();
                break;

            case Qt::Key_A:
                mutex.lock();

                // if other is true, stop it regardless
                if(other != NULL)
                {
                    if(other->animation)
                    {
                        other->animation = false;
                        other->timer.stop();
                    }
                }

                if(animation)
                {
                    animation = false;
                    timer.stop();
                }
                else
                {
                    animation = true;
                    timer.setInterval(15);
                    timer.start();
                }
                mutex.unlock();
                break;

            }

            update();
            UpdateOther();
        }
    }
}

void GNG3DIOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    event->accept();

    GNG3DIMainWidget* widget = dynamic_cast<GNG3DIMainWidget*>(parent());
    GNG3DIWindow* window = dynamic_cast<GNG3DIWindow*>(widget->parent());

    mutex.lock();
    float delta = (float) event->angleDelta().ry() / 100.0;
    if(distance < 0) distance = 0;
    else             distance -= delta;

    if(config->dynamic_lod)
    {
        int N_goal;
        if(distance >= config->min_detail_distance) N_goal = config->min_detail * 100.0f;
        else if(distance <= config->max_detail_distance) N_goal = config->max_detail * 100.0f;
        else
        {
            N_goal = (-((config->max_detail - config->min_detail)/(config->min_detail_distance - config->max_detail_distance)) *
                distance + config->max_detail) * 100.0f;

            if(N_goal <= config->min_detail * 100.0f)
                N_goal = config->min_detail * 100.0f;
        }

        mutex.unlock();

        widget->StepsUntilFinished(N_goal);
    }
    else mutex.unlock();

    update();
    UpdateOther();
}

void GNG3DIOpenGLWidget::UpdateOrbit(float x_offset, float y_offset, float rot_x, float rot_y, float distance)
{
    mutex.lock();
    this->x_offset = x_offset;
    this->y_offset = y_offset;
    this->rot_x = rot_x;
    this->rot_y = rot_y;
    this->distance = distance;
    mutex.unlock();

    update();
}

void GNG3DIOpenGLWidget::AutoSetCamera(void)
{
    if(vertices_count <= 0) return;

    mutex.lock();
    x_offset = 0.0;
    y_offset = 0.0;
    rot_x = 0.0f;
    rot_y = 0.0f;
    distance = max_distance + 2.0;
    mutex.unlock();

    update();
    UpdateOther();
}

void GNG3DIOpenGLWidget::UpdateOther(void)
{
    if(!config->separate_camera_control)
        if(other != NULL)
            other->UpdateOrbit(x_offset, y_offset, rot_x, rot_y, distance);
}

void GNG3DIOpenGLWidget::UpdateAnimation(void)
{
    mutex.lock();
    rot_y += 0.002;
    mutex.unlock();

    update();
    UpdateOther();
}

void GNG3DIOpenGLWidget::LoadVertices(struct Vec3* vertices, int len, struct GNG3DResult* result, bool only_signal)
{
    if(!gl) return;

    if(node_types != NULL) free(node_types);
    node_types = (GLfloat*)malloc(sizeof(GLfloat) * len);

    float v_min = FLT_MAX;
    float v_max = FLT_MIN;

    float max_distance = 0;

    for(int i = 0; i < len; i++)
    {
        //printf("%f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);

        if(v_min > vertices[i].z) v_min = vertices[i].z;
        if(v_max < vertices[i].z) v_max = vertices[i].z;

        float tmp = Vec3_len(&vertices[i]);
        if(tmp > max_distance) max_distance = tmp;

        if(result != NULL)
        {
            if(only_signal)
            {
                if(result->signal == i)
                {
                    node_types[i] = 1.0;
                }
                else
                {
                    node_types[i] = 0.0;
                }
            }
            else
            {
                if(result->winner == i)
                {
                    node_types[i] = 2.0;
                }
                else if(result->winner2 == i)
                {
                    node_types[i] = 3.0;
                }
                else
                {
                    node_types[i] = 0.0;
                }
            }
        }
        else
        {
            node_types[i] = 0.0;
        }
    }

    SetDepth(v_min, v_max);

    mutex.lock();
    makeCurrent();

    this->max_distance = max_distance;

    gl->glDeleteBuffers(1, &vbo_node);
    gl->glGenBuffers(1, &vbo_node);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_node);
    gl->glBufferData(GL_ARRAY_BUFFER, len * sizeof(struct Vec3), (GLfloat*) vertices, GL_STATIC_DRAW);
    vertices_count = len;

    gl->glDeleteBuffers(1, &vbo_node_types);
    gl->glGenBuffers(1, &vbo_node_types);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_node_types);
    gl->glBufferData(GL_ARRAY_BUFFER, len * sizeof(GLfloat), (GLfloat*) node_types, GL_STATIC_DRAW);

    mutex.unlock();

    update();
}

void GNG3DIOpenGLWidget::LoadEdges(struct EdgeInfo* edges, int count)
{
    if(!gl) return;

    if(_edges != NULL) free(_edges);
    _edges = (Vec3*)malloc(sizeof(struct Vec3) * count);

    if(edge_ages != NULL) free(edge_ages);
    edge_ages = (GLfloat*)malloc(sizeof(GLfloat) * count);

    float depth_min_value = INT_MAX;
    float depth_max_value = INT_MIN;

    int age_min_value = INT_MAX;
    int age_max_value = INT_MIN;

    for(int i = 0; i < count; i++)
    {
        // find min max depth
        if(depth_min_value > edges[i].pos.z) depth_min_value = edges[i].pos.z;
        if(depth_max_value < edges[i].pos.z) depth_max_value = edges[i].pos.z;

        // find min max age
        if(age_min_value > edges[i].age) age_min_value = edges[i].age;
        if(age_max_value < edges[i].age) age_max_value = edges[i].age;


        // copy pos and age
        _edges[i] = edges[i].pos;
        edge_ages[i] = edges[i].age;
    }

    SetEdgeDepth(depth_min_value, depth_max_value);
    SetAgeFunc(age_min_value, age_max_value);

    mutex.lock();
    makeCurrent();

    gl->glDeleteBuffers(1, &vbo_edge);
    gl->glGenBuffers(1, &vbo_edge);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_edge);
    gl->glBufferData(GL_ARRAY_BUFFER, count * sizeof(struct Vec3), (GLfloat*) _edges, GL_STATIC_DRAW);
    edges_count = count;

    gl->glDeleteBuffers(1, &vbo_edge_ages);
    gl->glGenBuffers(1, &vbo_edge_ages);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_edge_ages);
    gl->glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), (GLfloat*) edge_ages, GL_STATIC_DRAW);

    mutex.unlock();

    update();
}

void GNG3DIOpenGLWidget::SetSignal(GNG3DResult* result)
{
    mutex.lock();
    this->result.signal = result->signal;
    mutex.unlock();
}

void GNG3DIOpenGLWidget::SetWinners(GNG3DResult* result)
{
    mutex.lock();
    this->result.winner = result->winner;
    this->result.winner2 = result->winner2;
    mutex.unlock();
}
