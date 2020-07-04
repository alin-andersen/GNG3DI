#include "GNG3DIAboutWidget.h"

#include <iostream>
#include <QStyleOption>

GNG3DIAboutWidget::GNG3DIAboutWidget(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("About");
    setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void GNG3DIAboutWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GNG3DIAboutWidget::mousePressEvent(QMouseEvent* event)
{
    close();
}

void GNG3DIAboutWidget::focusOutEvent(QFocusEvent* event)
{
    close();
}
