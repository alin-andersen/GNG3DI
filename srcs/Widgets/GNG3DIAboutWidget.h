#ifndef GNG3DIABOUTWIDGET_H
#define GNG3DIABOUTWIDGET_H

#include <QObject>
#include <QMainWindow>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

#include "ui_GNG3DIAboutWidget.h"

class GNG3DIAboutWidget : public QWidget
{
    Q_OBJECT

public:

    GNG3DIAboutWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* event);
    void focusOutEvent(QFocusEvent* event);

private:

    Ui::GNG3DIAboutWidget ui;

};

#endif // GNG3DIABOUTWIDGET_H
