#include "GNG3DIParameterWidget.h"

GNG3DIParameterWidget::GNG3DIParameterWidget(QWidget* parent, QString* label) : QWidget(parent)
{
    ui.setupUi(this);

    ui.label->setText(*label);
}
