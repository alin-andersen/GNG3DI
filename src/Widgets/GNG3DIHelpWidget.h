#ifndef GNG3DIHELPWIDGET_H
#define GNG3DIHELPWIDGET_H

#include <QObject>
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QWidgetAction>
#include <QListView>

#include "Widgets/GNG3DIParameterInfoWidget.h"

#include "ui_GNG3DIHelpWidget.h"

class GNG3DIHelpWidget : public QWidget
{
    Q_OBJECT

public:

    GNG3DIHelpWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent*);

private:

    Ui::GNG3DIHelpWidget ui;

};

#endif // GNG3DHELPWIDGET_H
