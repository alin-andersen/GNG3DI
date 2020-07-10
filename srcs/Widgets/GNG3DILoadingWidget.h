#ifndef GNG3DILOADINGWIDGET_H
#define GNG3DILOADINGWIDGET_H

#include <iostream>
#include <QWidget>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QTimer>
#include <QMutex>

#include "ui_GNG3DILoadingWidget.h"

class GNG3DILoadingWidget : public QWidget
{
    Q_OBJECT

private:

    QTimer timer;
    float value = 0.0;
    QMutex mutex;

public:

    GNG3DILoadingWidget(QWidget* parent = 0);
    void paintEvent(QPaintEvent*);
    Ui::GNG3DILoadingWidget ui;

public slots:

    void SlotProgressUpdate(void);

};

#endif
