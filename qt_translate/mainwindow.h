#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QPushButton>
class QTranslator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeEvent(QEvent *e);

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QTranslator trans;
    QApplication *qApp;
    QPushButton *btn;
};
#endif // MAINWINDOW_H
