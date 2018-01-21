#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <vector>

#include "aqimage.h"
#include "model.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateImage();
private:
    static const int BUTTONS_COUNT = 2;
    QLabel* draw_widget;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QPushButton *open_button;
    QPushButton *buttons[BUTTONS_COUNT];
    AQImage image;
    Model model;
private slots:
    void openImage();
    void detect();
    void learn();
};

#endif // MAINWINDOW_H
