#include "GNG3DIEmptyWidget.h"

#include <QPainter>
#include <QDebug>
#include <QStyleOption>

GNG3DIEmptyWidget::GNG3DIEmptyWidget(QWidget* parent) : QWidget(parent)
{
    setupUi(this);
}

void GNG3DIEmptyWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
