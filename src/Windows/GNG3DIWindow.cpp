#include "GNG3DIWindow.h"

#include "Widgets/GNG3DIHelpWidget.h"
#include "Widgets/GNG3DIAboutWidget.h"
#include "Widgets/GNG3DIMainWidget.h"
#include "Widgets/GNG3DIEmptyWidget.h"
#include "Widgets/GNG3DISettingsWidget.h"
#include "Widgets/GNG3DIParameterWidget.h"

GNG3DIWindow::GNG3DIWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowTitle(QString("GNG3D Interactive"));
    setAttribute(Qt::WA_DeleteOnClose);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    connect(ui.actionLoad_mesh, SIGNAL(triggered(bool)), this, SLOT(SlotLoadMesh()));
    connect(ui.actionSwap_mesh, SIGNAL(triggered(bool)), this, SLOT(SlotSwapMesh()));
    connect(ui.saveMesh, SIGNAL(triggered(bool)), this, SLOT(SlotSaveMesh()));
    connect(ui.resetMesh, SIGNAL(triggered(bool)), this, SLOT(SlotResetMesh()));
    connect(ui.actionHelp, SIGNAL(triggered(bool)), this, SLOT(SlotOpenHelp()));
    connect(ui.actionSettings, SIGNAL(triggered(bool)), this, SLOT(SlotOpenSettings()));
    connect(ui.actionAbout, SIGNAL(triggered(bool)), this, SLOT(SlotOpenAbout()));
    connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(SlotExit()));

    config = new GNG3DConfig();

    QString str;
    QWidgetAction* a;

    str = "N (in %)";
    a = new QWidgetAction(this);
    parameter_N = new GNG3DIParameterWidget(this, &str);
    connect(parameter_N->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedN(void)));
    a->setDefaultWidget(parameter_N);
    ui.menuParameters->addAction(a);
    UpdateParameterN();

    str = "alpha";
    a = new QWidgetAction(this);
    parameter_alpha = new GNG3DIParameterWidget(this, &str);
    connect(parameter_alpha->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedAlpha(void)));
    a->setDefaultWidget(parameter_alpha);
    ui.menuParameters->addAction(a);
    UpdateParameterAlpha();

    str = "alpha_max";
    a = new QWidgetAction(this);
    parameter_alpha_max = new GNG3DIParameterWidget(this, &str);
    connect(parameter_alpha_max->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedAlphaMax(void)));
    a->setDefaultWidget(parameter_alpha_max);
    ui.menuParameters->addAction(a);
    UpdateParameterAlphaMax();

    str = "lambda";
    a = new QWidgetAction(this);
    parameter_lambda = new GNG3DIParameterWidget(this, &str);
    connect(parameter_lambda->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedLambda(void)));
    a->setDefaultWidget(parameter_lambda);
    ui.menuParameters->addAction(a);
    UpdateParameterLambda();

    str = "epsilon_b";
    a = new QWidgetAction(this);
    parameter_epsilon_b = new GNG3DIParameterWidget(this, &str);
    connect(parameter_epsilon_b->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedEpsilonB(void)));
    a->setDefaultWidget(parameter_epsilon_b);
    ui.menuParameters->addAction(a);
    UpdateParameterEpsilonB();

    str = "epsilon_n";
    a = new QWidgetAction(this);
    parameter_epsilon_n = new GNG3DIParameterWidget(this, &str);
    connect(parameter_epsilon_n->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedEpsilonN(void)));
    a->setDefaultWidget(parameter_epsilon_n);
    ui.menuParameters->addAction(a);
    UpdateParameterEpsilonN();

    str = "d";
    a = new QWidgetAction(this);
    parameter_d = new GNG3DIParameterWidget(this, &str);
    connect(parameter_d->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedD(void)));
    a->setDefaultWidget(parameter_d);
    ui.menuParameters->addAction(a);
    UpdateParameterD();

    str = "mu";
    a = new QWidgetAction(this);
    parameter_mu = new GNG3DIParameterWidget(this, &str);
    connect(parameter_mu->ui.line, SIGNAL(editingFinished()), this, SLOT(SlotChangedMu(void)));
    a->setDefaultWidget(parameter_mu);
    ui.menuParameters->addAction(a);
    UpdateParameterMu();

    EnableMenuReset(false);
    EnableMenuSave(false);

    GNG3DIEmptyWidget* widget = new GNG3DIEmptyWidget(this);
    QWidget* old = centralWidget();
    setCentralWidget(widget);
    if(old != NULL) old->deleteLater();
}

void GNG3DIWindow::UpdatedConfig(void)
{
    if(main == NULL) return;
    main->SetConfig(config);
}

void GNG3DIWindow::SlotLoadMesh(void)
{
    QStringList filters;
    filters << "Autodesk (*.fbx)"
            << "Collada (*.dae)"
            << "glTF (*.gltf *.glb)"
            << "Blender 3D (*.blend)"
            << "3ds Max 3DS (*.3ds)"
            << "3ds Max ASE (*.ase)"
            << "Wavefront Object (*.obj)"
            << "Industry Foundation Classes (*.ifc)"
            << "XGL (*.xgl *.zgl)"
            << "Stanford Polygon Library (*.ply)"
            << "AutoCAD DXF (*.dxf)"
            << "LightWave (*.lwo)"
            << "LightWave Scene (*.lws)"
            << "Modo (*.lxo)"
            << "Stereolithography (*.stl)"
            << "DirectX X (*.x)"
            << "AC3D (*.ac)"
            << "Milkshape 3D (*.ms3d)"
            << "TrueSpace (*.cob *.scn)";

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setViewMode(QFileDialog::List);
    dialog.setNameFilters(filters);

    QStringList fileNames;
    if (dialog.exec()) fileNames = dialog.selectedFiles();
    if(fileNames.count() == 0) return;

    // main widget
    main = new GNG3DIMainWidget(this, config);
    main->LoadMesh(&fileNames.first());
    QWidget* old = centralWidget();
    setCentralWidget(main);
    if(old != NULL) old->deleteLater();

    EnableMenuReset(true);
    EnableMenuSave(true);
    EnableMenuLoad(false);
    EnableMenuSwap(true);
}

void GNG3DIWindow::SlotSwapMesh(void)
{
    QStringList filters;
    filters << "Autodesk (*.fbx)"
            << "Collada (*.dae)"
            << "glTF (*.gltf *.glb)"
            << "Blender 3D (*.blend)"
            << "3ds Max 3DS (*.3ds)"
            << "3ds Max ASE (*.ase)"
            << "Wavefront Object (*.obj)"
            << "Industry Foundation Classes (*.ifc)"
            << "XGL (*.xgl *.zgl)"
            << "Stanford Polygon Library (*.ply)"
            << "AutoCAD DXF (*.dxf)"
            << "LightWave (*.lwo)"
            << "LightWave Scene (*.lws)"
            << "Modo (*.lxo)"
            << "Stereolithography (*.stl)"
            << "DirectX X (*.x)"
            << "AC3D (*.ac)"
            << "Milkshape 3D (*.ms3d)"
            << "TrueSpace (*.cob *.scn)";

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setViewMode(QFileDialog::List);
    dialog.setNameFilters(filters);

    QStringList fileNames;
    if (dialog.exec()) fileNames = dialog.selectedFiles();
    if(fileNames.count() == 0) return;

    if(main == NULL) return;

    if(!main->SwapMesh(&fileNames.first()))
    {
        QMessageBox box(this);
        box.setText(QString("Could not swap mesh! Check filesystem permission!"));
        box.exec();
    }

    EnableMenuReset(true);
    EnableMenuSave(true);
    EnableMenuLoad(false);
    EnableMenuSwap(true);
}

void GNG3DIWindow::SlotSaveMesh(void)
{
    if(main == NULL) return;

    QStringList filters;

    for(int i = 0 ; i < main->handler->exporter.GetExportFormatCount(); i++)
    {
        const aiExportFormatDesc* desc = main->handler->exporter.GetExportFormatDescription(i);

        std::string d = std::string(desc->description) +
                        std::string(" (*.") +
                        std::string(desc->id) +
                        std::string(")");
        filters << QString(d.c_str());
    }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilters(filters);

    QStringList fileNames;
    if (dialog.exec()) fileNames = dialog.selectedFiles();
    if(fileNames.count() == 0) return;

    QString path = fileNames.first();
    QString filter = dialog.selectedNameFilter();

    int index = filter.indexOf(QChar('*'));
    QString extension = filter.mid(index+2, filter.size()-index-3);

    qDebug() << path << " " << extension;

    bool ret = main->handler->Save(&path, &extension);

    QMessageBox box(this);

    if(!ret)
    {
        box.setText(QString("Could not save mesh! Check filesystem permission!"));
    }
    else
    {
        box.setText(QString("Mesh saved!"));
    }

    box.exec();
}


void GNG3DIWindow::SlotResetMesh(void)
{
    // set to empty widget
    GNG3DIEmptyWidget* widget = new GNG3DIEmptyWidget(this);

    QWidget* old = centralWidget();
    setCentralWidget(widget);
    if(old != NULL) old->deleteLater();

    if(main != NULL)
    {
        delete main;
        main = NULL;
    }

    EnableMenuReset(false);
    EnableMenuSave(false);
    EnableMenuLoad(true);
    EnableMenuSwap(false);
}

void GNG3DIWindow::SlotOpenSettings(void)
{
    GNG3DISettingsWidget* widget = new GNG3DISettingsWidget(this, config);
    widget->show();
}

void GNG3DIWindow::SlotOpenHelp(void)
{
    GNG3DIHelpWidget* window = new GNG3DIHelpWidget(this);
    window->show();
}

void GNG3DIWindow::SlotOpenAbout(void)
{
    GNG3DIAboutWidget* window = new GNG3DIAboutWidget(this);
    window->show();
}

void GNG3DIWindow::SlotExit(void)
{
    close();
}

void GNG3DIWindow::SlotChangedN(void)
{
    bool valid;
    int N = parameter_N->ui.line->text().toInt(&valid);

    if(valid)
    {
        if(N > 1)
        {
            config->N = N;
            UpdatedConfig();
        }
    }

    UpdateParameterN();
}

void GNG3DIWindow::SlotChangedAlpha(void)
{
    bool valid;
    float alpha = parameter_alpha->ui.line->text().toFloat(&valid);

    if(valid)
    {
        if(alpha >= 0.0f && alpha <= 1.0f)
        {
            config->alpha = alpha;
            UpdatedConfig();
        }
    }

    UpdateParameterAlpha();
}

void GNG3DIWindow::SlotChangedAlphaMax(void)
{
    bool valid;
    int alpha_max = parameter_alpha_max->ui.line->text().toInt(&valid);

    if(valid)
    {
        if(alpha_max > 0)
        {
            config->alpha_max = alpha_max;
            UpdatedConfig();
        }
    }

    UpdateParameterAlphaMax();
}

void GNG3DIWindow::SlotChangedLambda(void)
{
    bool valid;
    int lambda = parameter_lambda->ui.line->text().toInt(&valid);

    if(valid)
    {
        if(lambda > 0)
        {
            config->lambda = lambda;
            UpdatedConfig();
        }
    }

    UpdateParameterLambda();
}

void GNG3DIWindow::SlotChangedEpsilonB(void)
{
    bool valid;
    float epsilon_b = parameter_epsilon_b->ui.line->text().toFloat(&valid);

    if(valid)
    {
        if(epsilon_b >= 0.0f && epsilon_b <= 1.0f)
        {
            config->epsilon_b = epsilon_b;
            UpdatedConfig();
        }
    }

    UpdateParameterEpsilonB();
}

void GNG3DIWindow::SlotChangedEpsilonN(void)
{
    bool valid;
    float epsilon_n = parameter_epsilon_n->ui.line->text().toFloat(&valid);

    if(valid)
    {
        if(epsilon_n >= 0.0f && epsilon_n <= 1.0f)
        {
            config->epsilon_n = epsilon_n;
            UpdatedConfig();
        }
    }

    UpdateParameterEpsilonN();
}

void GNG3DIWindow::SlotChangedD(void)
{
    bool valid;
    float d = parameter_d->ui.line->text().toFloat(&valid);

    if(valid)
    {
        if(d >= 0.0f && d <= 1.0f)
        {
            config->d = d;
            UpdatedConfig();
        }
    }

    UpdateParameterD();
}

void GNG3DIWindow::SlotChangedMu(void)
{
    bool valid;
    int mu = parameter_mu->ui.line->text().toInt(&valid);

    if(valid)
    {
        if(mu >= 0)
        {
            config->mu = mu;
            UpdatedConfig();
        }
    }

    UpdateParameterMu();
}

void GNG3DIWindow::UpdateParameterN()
{
    QString tmp;
    tmp.sprintf("%d", config->N);
    parameter_N->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterAlpha(void)
{
    QString tmp;
    tmp.sprintf("%f", config->alpha);
    parameter_alpha->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterAlphaMax()
{
    QString tmp;
    tmp.sprintf("%d", config->alpha_max);
    parameter_alpha_max->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterLambda()
{
    QString tmp;
    tmp.sprintf("%d", config->lambda);
    parameter_lambda->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterEpsilonB(void)
{
    QString tmp;
    tmp.sprintf("%f", config->epsilon_b);
    parameter_epsilon_b->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterEpsilonN(void)
{
    QString tmp;
    tmp.sprintf("%f", config->epsilon_n);
    parameter_epsilon_n->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterD(void)
{
    QString tmp;
    tmp.sprintf("%f", config->d);
    parameter_d->ui.line->setText(tmp);
}

void GNG3DIWindow::UpdateParameterMu(void)
{
    QString tmp;
    tmp.sprintf("%d", config->mu);
    parameter_mu->ui.line->setText(tmp);
}

void GNG3DIWindow::EnableMenuSave(bool v)
{
    ui.saveMesh->setEnabled(v);
    ui.saveMesh->setVisible(v);
}

void GNG3DIWindow::EnableMenuReset(bool v)
{
    ui.resetMesh->setEnabled(v);
    ui.resetMesh->setVisible(v);
}

void GNG3DIWindow::EnableMenuLoad(bool v)
{
    ui.actionLoad_mesh->setEnabled(v);
    ui.actionLoad_mesh->setVisible(v);
}

void GNG3DIWindow::EnableMenuSwap(bool v)
{
    ui.actionSwap_mesh->setEnabled(v);
    ui.actionSwap_mesh->setVisible(v);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    GNG3DIWindow* gng3di = new GNG3DIWindow();
    gng3di->show();

    return app.exec();
}
