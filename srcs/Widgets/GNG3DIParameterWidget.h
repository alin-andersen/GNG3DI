#ifndef GNG3DIPARAMETERWIDGET_H
#define GNG3DIPARAMETERWIDGET_H

#include <QWidget>
#include <QLabel>

#include "ui_GNG3DIParameterWidget.h"

class GNG3DIParameterWidget : public QWidget
{
    Q_OBJECT

public:

    GNG3DIParameterWidget(QWidget* parent, QString* label);

    Ui::GNG3DIParameterWidget ui;
};

#endif // GNG3DIPARAMETERWIDGET_H
