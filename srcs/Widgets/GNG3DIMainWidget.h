#ifndef GNG3DIMAINWIDGET_H
#define GNG3DIMAINWIDGET_H

#include <iostream>

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QMutex>
#include <QTimer>

#include "Core/GNG3DConfig.h"
#include "Core/GNG3DHandler.h"

#include "Widgets/GNG3DIParameterWidget.h"

#include "GNG3DIOpenGLWidget.h"

#include "ui_GNG3DIMainWidget.h"

class GNG3DIMainWidget : public QWidget , private Ui::GNG3DIMainWidget
{
    Q_OBJECT

public:

    Ui_GNG3DIMainWidget ui;

    GNG3DResult result;
    GNG3DIOpenGLWidget* leftGL;
    GNG3DIOpenGLWidget* rightGL;
    GNG3DHandler* handler;

    GNG3DIMainWidget(QWidget* parent, GNG3DConfig* config);
    ~GNG3DIMainWidget();

    void SetConfig(GNG3DConfig* config);
    bool LoadMesh(QString* path);
    bool SwapMesh(QString* path);
    void UpdateLabels();

    void StepsUntilFinished(int N_goal);

    void paintEvent(QPaintEvent*);

public slots:

    void SlotRunClick();
    void SlotRunLoop();
    void SlotStepClick();

private:

    GNG3DConfig* config = NULL;

    QTimer timer;
    QMutex mutex;
    int state_running = 0;
    int iterations = -1;

    void IncrementIterations(int iterations);

    void SetOriginalVertices(int vertices_count, int faces_count);
    void SetSimplifiedVertices(int vertices_count, int edges_count);
    void SetSizeComparison(int o_count, int s_count);

    void SetLabelOriginal(QString* str);
    void SetLabelSimplified(QString* str);
    void SetLabelCenter(float value);

};

#endif
