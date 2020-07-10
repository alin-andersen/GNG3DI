#ifndef GNG3DIEMPTYWIDGET_H
#define GNG3DIEMPTYWIDGET_H

#include <iostream>
#include <QWidget>
#include <QDir>
#include <QFile>
#include <QPainter>

#include "ui_GNG3DIEmptyWidget.h"

class GNG3DIEmptyWidget : public QWidget , private Ui::GNG3DIEmptyWidget
{
    Q_OBJECT

public:
    GNG3DIEmptyWidget(QWidget* parent = 0);
    void paintEvent(QPaintEvent*);
};

#endif
