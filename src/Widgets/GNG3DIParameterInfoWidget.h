#ifndef GNG3DIPARAMETERINFOWIDGET_H
#define GNG3DIPARAMETERINFOWIDGET_H

#include <QWidget>
#include <QLabel>

#include "ui_GNG3DIParameterInfoWidget.h"

class GNG3DIParameterInfoWidget : public QWidget
{
    Q_OBJECT

public:

    GNG3DIParameterInfoWidget(QWidget* parent);

    void UpdateInfo(QString* name, QString* type, QString* domain, QString* description);
    Ui::GNG3DIParameterInfoWidget ui;
};

#endif
