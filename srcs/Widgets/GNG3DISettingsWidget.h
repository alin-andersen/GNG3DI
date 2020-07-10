#ifndef GNG3DISETTINGS_WIDGET_H
#define GNG3DISETTINGS_WIDGET_H

#include <QObject>
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QLineEdit>
#include <QCheckBox>
#include <QColor>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>

#include "Core/GNG3DConfig.h"

#include "ui_GNG3DISettingsWidget.h"

#include "Windows/GNG3DIWindow.h"

class GNG3DISettingsWidget : public QWidget
{
    Q_OBJECT

public:

    GNG3DISettingsWidget(QWidget *parent, GNG3DConfig* config);
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent* event);

private slots:

    void SlotApply();
    void SlotCancel();

private:

    GNG3DConfig* config = NULL;
    Ui::GNG3DISettingsWidget ui;

    QGroupBox general;
    QLabel    label_seed;
    QLineEdit label_seed_input;

    QGroupBox rendering;

};

#endif
