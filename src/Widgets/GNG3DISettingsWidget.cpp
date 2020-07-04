#include "GNG3DISettingsWidget.h"

#include <QStyleOption>

GNG3DISettingsWidget::GNG3DISettingsWidget(QWidget* parent, GNG3DConfig* config) : QWidget(parent)
{
    this->config = config;

    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Settings");
    setWindowFlags(Qt::Drawer);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(ui.apply, SIGNAL(clicked(bool)), this, SLOT(SlotApply()));
    connect(ui.cancel, SIGNAL(clicked(bool)), this, SLOT(SlotCancel()));

    //grabKeyboard();

    // general
    QString seed; seed.sprintf("%d", config->seed);
    ui.seed->setText(seed);

    // rendering
    ui.separate_camera_control->setChecked(config->separate_camera_control);
    ui.skip_rendering->setChecked(config->skip_rendering);
    //ui.skip_auto_animation->setChecked(config->skip_auto_animation);
    ui.dynamicLod->setChecked(config->dynamic_lod);
    ui.mode_edge->setChecked(config->age_rendering);

    // rendering nodes
    QString point_size_node;
    point_size_node.sprintf("%f", config->point_size_node);
    ui.point_size_node->setText(point_size_node);
    ui.no_winner_color_node->setText(config->no_winner_color_node.name());
    ui.signal_color_node->setText(config->signal_color_node.name());
    ui.winner_color_node->setText(config->winner_color_node.name());
    ui.winner2_color_node->setText(config->winner2_color_node.name());

    // rendering edges
    ui.line_color_edge->setText(config->line_color_edge.name());
    QString line_size_edge;
    point_size_node.sprintf("%f", config->line_size_edge);
    ui.line_size_edge->setText(line_size_edge);

    // dynamic lof

    QString min_detail;
    min_detail.sprintf("%f", config->min_detail);
    ui.minDetail->setText(min_detail);

    QString min_detail_distance;
    min_detail_distance.sprintf("%f", config->min_detail_distance);
    ui.minDetailDistance->setText(min_detail_distance);

    QString max_detail;
    max_detail.sprintf("%f", config->max_detail);
    ui.maxDetail->setText(max_detail);

    QString max_detail_distance;
    max_detail_distance.sprintf("%f", config->max_detail_distance);
    ui.maxDetailDistance->setText(max_detail_distance);

}

void GNG3DISettingsWidget::SlotApply()
{
    // general
    config->seed = ui.seed->text().toInt();

    // rendering
    config->separate_camera_control = ui.separate_camera_control->isChecked();
    config->skip_rendering = ui.skip_rendering->isChecked();
    config->dynamic_lod = ui.dynamicLod->isChecked();
    config->age_rendering = ui.mode_edge->isChecked();

    // rendering nodes
    config->point_size_node = ui.point_size_node->text().toFloat();
    config->no_winner_color_node = ui.no_winner_color_node->text();
    config->signal_color_node = ui.signal_color_node->text();
    config->winner_color_node = ui.winner_color_node->text();
    config->winner2_color_node = ui.winner2_color_node->text();

    // rendering edges
    config->line_color_edge = ui.line_color_edge->text();
    config->line_size_edge = ui.line_size_edge->text().toFloat();

    // dynamic lod
    float tmp = ui.minDetail->text().toFloat();
    if(tmp >= 0 && tmp <= 1.0) config->min_detail = tmp;
    config->min_detail_distance = ui.minDetailDistance->text().toFloat();

    tmp = ui.maxDetail->text().toFloat();
    if(tmp >= 0 && tmp <= 1.0) config->max_detail = tmp;
    config->max_detail_distance = ui.maxDetailDistance->text().toFloat();

    GNG3DIWindow* window = dynamic_cast<GNG3DIWindow*>(parent());
    window->UpdatedConfig();

    close();
}

void GNG3DISettingsWidget::SlotCancel()
{
    close();
}

void GNG3DISettingsWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GNG3DISettingsWidget::keyPressEvent(QKeyEvent* event)
{
    event->accept();

    if(event->type() == QKeyEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        switch(key->key())
        {
        case Qt::Key_Return: SlotApply(); break;
        default: break;
        }
    }
}
