#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();
    QImage readImageFromSharedMemory(const QString& sharedMemoryName, int width, int height);

private:
    Ui::QtWidgetsApplication1Class ui;
};
