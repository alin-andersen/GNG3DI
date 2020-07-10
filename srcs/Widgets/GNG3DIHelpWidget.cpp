#include "GNG3DIHelpWidget.h"
#include <QStyleOption>

GNG3DIHelpWidget::GNG3DIHelpWidget(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Help");
    setWindowFlags(Qt::Drawer);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    QString str;
    GNG3DIParameterInfoWidget* widget;

    // alpha_max
    widget = new GNG3DIParameterInfoWidget(this);
    str = "alpha_max";
    widget->ui.name->setText(str);
    str = "INTEGER";
    widget->ui.type->setText(str);
    str = "0 < alpha_max";
    widget->ui.domain->setText(str);
    str = "Defines the maxium age of edges. Edges exceeding the age limit will get removed.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // alpha
    widget = new GNG3DIParameterInfoWidget(this);
    str = "alpha";
    widget->ui.name->setText(str);
    str = "FLOAT";
    widget->ui.type->setText(str);
    str = "0 < alpha < 1";
    widget->ui.domain->setText(str);
    str = "Defines a scalar value that will multipliyed with the error counter to decrease it each step.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // d
    widget = new GNG3DIParameterInfoWidget(this);
    str = "d";
    widget->ui.name->setText(str);
    str = "FLOAT";
    widget->ui.type->setText(str);
    str = "0 < epsilon_n < 1";
    widget->ui.domain->setText(str);
    str = "Is multiplied by the local error counter to ...";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // N
    widget = new GNG3DIParameterInfoWidget(this);
    str = "N";
    widget->ui.name->setText(str);
    str = "INTEGER";
    widget->ui.type->setText(str);
    str = "1 < N";
    widget->ui.domain->setText(str);
    str = "Defines the maxiumum number of nodes that the algorithm is allowed to create. After "
          "reaching this limit, the algorithm can only remove nodes, before creating new onces.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // epsilon_b
    widget = new GNG3DIParameterInfoWidget(this);
    str = "epsilon_b";
    widget->ui.name->setText(str);
    str = "FLOAT";
    widget->ui.type->setText(str);
    str = "0 < epsilon_b < 1";
    widget->ui.domain->setText(str);
    str = "Defines the fraction of the distance between winner node and signal. The winner "
          "is moved in the direction of the signal.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // epsilon_n
    widget = new GNG3DIParameterInfoWidget(this);
    str = "epsilon_n";
    widget->ui.name->setText(str);
    str = "FLOAT";
    widget->ui.type->setText(str);
    str = "0 < epsilon_n < 1";
    widget->ui.domain->setText(str);
    str = "Defines the fraction of the distance between the secondary winner node and signal.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // lambda
    widget = new GNG3DIParameterInfoWidget(this);
    str = "lambda";
    widget->ui.name->setText(str);
    str = "INTEGER";
    widget->ui.type->setText(str);
    str = "0 < lambda";
    widget->ui.domain->setText(str);
    str = "Defines the number of iterations before the algorithm is allowed to insert or remove "
          "nodes.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);

    // mu
    widget = new GNG3DIParameterInfoWidget(this);
    str = "mu";
    widget->ui.name->setText(str);
    str = "INTEGER";
    widget->ui.type->setText(str);
    str = "0 < mu";
    widget->ui.domain->setText(str);
    str = "Is used to determine when to remove nodes that have not been activated.";
    widget->ui.description->appendPlainText(str);
    ui.parameters->addWidget(widget);
}

void GNG3DIHelpWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
