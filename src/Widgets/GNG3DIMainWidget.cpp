#include "GNG3DIMainWidget.h"

#include <QPainter>
#include <QDebug>
#include <QStyleOption>

GNG3DIMainWidget::GNG3DIMainWidget(QWidget* parent, GNG3DConfig* config) : QWidget(parent)
{
    ui.setupUi(this);

    this->config = config->Clone();

    leftGL = new GNG3DIOpenGLWidget(this, config);
    rightGL = new GNG3DIOpenGLWidget(this, config);

    leftGL->SetOther(rightGL);
    rightGL->SetOther(leftGL);

    ui.gridLayout_3->addWidget(leftGL,0,0,1,2);
    ui.gridLayout_3->addWidget(rightGL,0,2,1,2);

    connect(ui.run_button, SIGNAL(released()), this, SLOT(SlotRunClick()));
    connect(ui.steps_button, SIGNAL(released()), this, SLOT(SlotStepClick()));

    connect(&timer, SIGNAL(timeout()), this, SLOT(SlotRunLoop()));

    IncrementIterations(1);
}

GNG3DIMainWidget::~GNG3DIMainWidget()
{
    delete leftGL;
    delete rightGL;
    delete handler;
    delete config;
}

void GNG3DIMainWidget::SetConfig(GNG3DConfig* config)
{
    mutex.lock();

    if(this->config != NULL)
        delete this->config;
    this->config = config->Clone();

    leftGL->SetConfig(config);
    rightGL->SetConfig(config);
    handler->SetConfig(config);

    mutex.unlock();
}

bool GNG3DIMainWidget::LoadMesh(QString* path)
{
    handler = new GNG3DHandler(config);
    if(!handler->Load(path)) return false;

    leftGL->LoadVertices(handler->imesh->vertices, handler->imesh->vertices_count, NULL, true);
    rightGL->LoadVertices(handler->smesh->vertices, handler->smesh->vertices_count, NULL, false);

    leftGL->AutoSetCamera();

    UpdateLabels();

    return true;
}

bool GNG3DIMainWidget::SwapMesh(QString* path)
{
    if(!handler->Swap(path)) return false;

    leftGL->LoadVertices(handler->imesh->vertices, handler->imesh->vertices_count, NULL, true);
    UpdateLabels();

    return true;
}

void GNG3DIMainWidget::StepsUntilFinished(int N_goal)
{
    mutex.lock();

    int steps = handler->StepsUntilFinished(&result, N_goal);

    leftGL->LoadVertices(handler->imesh->vertices, handler->imesh->vertices_count, &result, true);
    rightGL->LoadVertices(handler->smesh->vertices, handler->smesh->vertices_count, &result, false);
    rightGL->LoadEdges(handler->smesh->_edges, handler->smesh->_edges_count);

    leftGL->update();
    rightGL->update();

    IncrementIterations(steps);

    mutex.unlock();

    UpdateLabels();
    update();
}

void GNG3DIMainWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GNG3DIMainWidget::SlotRunClick()
{
    mutex.lock();

    if(state_running != 0)
    {
        // STOPPED

        state_running = 0;
        ui.run_button->setText("Start");

        timer.stop();
    }
    else
    {
        // STARTED

        state_running = 1;
        ui.run_button->setText("Stop");

        // hard limit for delay
        int delay = ui.delay->text().toInt();
        if(delay < 0) delay = 0;
        timer.start(delay);
    }

    mutex.unlock();
}

void GNG3DIMainWidget::SlotRunLoop()
{
    mutex.lock();

    if(!state_running)
    {
        mutex.unlock();
        return;
    }

    int ret = handler->Step(1, &result);

    leftGL->LoadVertices(handler->imesh->vertices, handler->imesh->vertices_count, &result, true);
    rightGL->LoadVertices(handler->smesh->vertices, handler->smesh->vertices_count, &result, false);
    rightGL->LoadEdges(handler->smesh->_edges, handler->smesh->_edges_count);

    leftGL->update();
    rightGL->update();

    IncrementIterations(1);

    mutex.unlock();

    UpdateLabels();
    update();
}

void GNG3DIMainWidget::SlotStepClick()
{
    mutex.lock();

    int steps = ui.steps->text().toInt();

    int ret = handler->Step(steps, &result);

    leftGL->LoadVertices(handler->imesh->vertices, handler->imesh->vertices_count, &result, true);
    rightGL->LoadVertices(handler->smesh->vertices, handler->smesh->vertices_count, &result, false);
    rightGL->LoadEdges(handler->smesh->_edges, handler->smesh->_edges_count);

    leftGL->update();
    rightGL->update();

    IncrementIterations(steps);

    mutex.unlock();

    UpdateLabels();
    update();
}

void GNG3DIMainWidget::IncrementIterations(int iterations)
{
    this->iterations += iterations;
    QString tmp; tmp.sprintf("%d iter.", this->iterations);
    ui.total_iterations->setText(tmp);
}

void GNG3DIMainWidget::UpdateLabels()
{
    mutex.lock();
    SetOriginalVertices(handler->imesh->vertices_count, handler->imesh->mesh->mNumFaces);
    SetSimplifiedVertices(handler->smesh->vertices_count, handler->smesh->_edges_count/2);
    SetSizeComparison(handler->imesh->vertices_count, handler->smesh->vertices_count);
    mutex.unlock();
}

void GNG3DIMainWidget::SetOriginalVertices(int vertices_count, int faces_count)
{
    QString tmp;
    tmp.sprintf("%d vertices, %d faces", vertices_count, faces_count);
    SetLabelOriginal(&tmp);
}

void GNG3DIMainWidget::SetSimplifiedVertices(int vertices_count, int edges_count)
{
    QString tmp;
    tmp.sprintf("%d vertices, %d edges", vertices_count, edges_count);
    SetLabelSimplified(&tmp);
}

void GNG3DIMainWidget::SetSizeComparison(int o_count, int s_count)
{
    SetLabelCenter(((float)s_count/(float)o_count)*100.0f);
}

void GNG3DIMainWidget::SetLabelOriginal(QString* str)
{
    ui.label_original->setText(*str);
}

void GNG3DIMainWidget::SetLabelSimplified(QString* str)
{
    ui.label_simplified->setText(*str);
}

void GNG3DIMainWidget::SetLabelCenter(float value)
{
    QString tmp;
    tmp.sprintf("%3.2f%%", value);
    ui.label_center->setText(tmp);

    if(value >= 100.0f) ui.label_center->setStyleSheet("color:#ff0000");
    else ui.label_center->setStyleSheet("color:#00ff00");
}

