#ifndef GNG3DIWINDOW_H
#define GNG3DIWINDOW_H

#include <QApplication>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLineEdit>
#include <QApplication>
#include <QTimer>

#include <QObject>
#include <QMainWindow>
#include <QPainter>
#include <QMenu>
#include <QWidgetAction>
#include <QMessageBox>

#include "Core/GNG3DConfig.h"

#include "Widgets/GNG3DIMainWidget.h"
#include "Widgets/GNG3DILoadingWidget.h"

#include "ui_GNG3DIWindow.h"

class GNG3DIWindow : public QMainWindow
{
    Q_OBJECT

public:

    GNG3DIWindow(QWidget* parent = 0);
    void UpdatedConfig(void);

public slots:

    void SlotLoadMesh(void);
    void SlotSaveMesh(void);
    void SlotSwapMesh(void);
    void SlotResetMesh(void);

    void SlotOpenSettings(void);
    void SlotOpenAbout(void);
    void SlotOpenHelp(void);
    void SlotExit(void);

    void SlotChangedN(void);
    void SlotChangedAlpha(void);
    void SlotChangedAlphaMax(void);
    void SlotChangedEpsilonB(void);
    void SlotChangedEpsilonN(void);
    void SlotChangedLambda(void);
    void SlotChangedD(void);
    void SlotChangedMu(void);

private:

    Ui::GNG3DIWindow ui;

    GNG3DIMainWidget* main = NULL;
    GNG3DConfig* config = NULL;

    GNG3DIParameterWidget* parameter_N;
    void UpdateParameterN(void);

    GNG3DIParameterWidget* parameter_alpha;
    void UpdateParameterAlpha(void);

    GNG3DIParameterWidget* parameter_alpha_max;
    void UpdateParameterAlphaMax(void);

    GNG3DIParameterWidget* parameter_lambda;
    void UpdateParameterLambda(void);

    GNG3DIParameterWidget* parameter_epsilon_b;
    void UpdateParameterEpsilonB(void);

    GNG3DIParameterWidget* parameter_epsilon_n;
    void UpdateParameterEpsilonN(void);

    GNG3DIParameterWidget* parameter_d;
    void UpdateParameterD(void);

    GNG3DIParameterWidget* parameter_mu;
    void UpdateParameterMu(void);


    void EnableMenuSave (bool v);
    void EnableMenuReset(bool v);
    void EnableMenuLoad (bool v);
    void EnableMenuSwap (bool v);
};

#endif

