#ifndef GNG3DIOpenGLWidget_H
#define GNG3DIOpenGLWidget_H

#include <float.h>

#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QErrorMessage>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMutex>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QBoxLayout>

#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/GNG3DVec.h"
#include "Core/GNG3DConfig.h"
#include "Core/GNG3DResult.h"
#include "Core/GNG3DSimplifiedMesh.h"

class GNG3DIOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:

    GNG3DConfig* config = NULL;
    GNG3DResult  result;

    QOpenGLFunctions_3_3_Core* gl;
    QMutex mutex;

    GLuint shader_node, shader_edge;
    GLuint vao;
    GLuint vbo_node, vbo_edge; // vertices
    GLuint vbo_node_types; // types of vertices
    GLuint vbo_edge_ages; // ages

    GLfloat* node_types = NULL;

    struct Vec3* _edges = NULL;
    GLfloat* edge_ages = NULL;

    glm::mat4x4 v_matrix;
    glm::mat4x4 w_matrix;
    glm::mat4x4 m_matrix;

    // uniforms node shader
    int v_matrix_uniform_node;
    int w_matrix_uniform_node;
    int m_matrix_uniform_node;
    int color_no_winner_uniform_node;
    int color_signal_uniform_node;
    int color_winner_uniform_node;
    int color_winner2_uniform_node;
    int point_size_uniform_node;
    int depth_steepness_uniform_node;
    int depth_offset_uniform_node;

    float depth_steepness;
    float depth_offset;

    // uniforms edge shader
    int v_matrix_uniform_edge;
    int w_matrix_uniform_edge;
    int m_matrix_uniform_edge;
    int line_color_uniform_edge;
    int mode_uniform_edge;
    int depth_steepness_uniform_edge;
    int depth_offset_uniform_edge;
    int age_steepness_uniform_edge;
    int age_offset_uniform_edge;

    float depth_steepness_edge;
    float depth_offset_edge;
    float age_steepness;
    float age_offset;

    void _resizeGL();

    // for camera movement

    float max_distance = 0;
    float distance     = 50.0;
    float rot_x, rot_y =  0.0;
    int   old_x, old_y = -1;
    float x_offset, y_offset = 0.0;
    bool  xy_movement = false;
    bool  grab = false;

    void ViewMatrix(void);

    // for depth
    float min = 0.5;
    float max = 1.0;
    void SetDepth(float v_min, float v_max);
    void SetEdgeDepth(float v_min, float v_max);

    // age coloring
    float age_min = 1.0;
    float age_max = 0.2;
    void SetAgeFunc(float age_min_value, float age_max_value);

    // animation
    bool animation = false;
    QTimer timer;

    bool alter_other = true;
    GNG3DIOpenGLWidget* other = NULL;
    void UpdateOther(void);

    int  LoadShader(const char* path, int type);

public:

    int vertices_count = 0;
    int edges_count = 0;

    GNG3DIOpenGLWidget(QWidget* parent, GNG3DConfig* config);
    ~GNG3DIOpenGLWidget();

    void SetConfig(GNG3DConfig* config);
    void SetOther(GNG3DIOpenGLWidget* other);

    void initializeGL();
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);

    void UpdateOrbit(float x_offset, float y_offset, float rot_x, float rot_y, float distance);
    void LoadVertices(struct Vec3* vertices, int len, struct GNG3DResult* result, bool only_signal);
    void LoadEdges(struct EdgeInfo* edges, int count);

    void AutoSetCamera(void);

    void SetSignal(GNG3DResult* result);
    void SetWinners(GNG3DResult* result);

public slots:

    void UpdateAnimation(void);
};

#endif
