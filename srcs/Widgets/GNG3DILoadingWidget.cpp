#include "GNG3DILoadingWidget.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QStyleOption>

GNG3DILoadingWidget::GNG3DILoadingWidget(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);

    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(SlotProgressUpdate()));
    timer.start();

    //move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void GNG3DILoadingWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GNG3DILoadingWidget::SlotProgressUpdate(void)
{
    value += 0.1f;
    if(value >= 100.0f) value = 0.0f;

    ui.progress->setValue(value);
}
