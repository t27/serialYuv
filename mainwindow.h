#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#define INTERLEAVED
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionFile_info_triggered();

private:
    void YUV422_to_RGB888(QByteArray rawStreamingFrame);
    void YUVtoRGB(QByteArray yuvData);
    Ui::MainWindow *ui;
//    QString data;
    QString file1Name;
};

#endif // MAINWINDOW_H
