#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <initguid.h>
#include <devguid.h>          // GUID_DEVCLASS_*
#include <winioctl.h>         // GUID_DEVINTERFACE_MOUSE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void applyMouseSpeed();

private:
    QLabel *labelMouse;
    QSlider *slider;
    QPushButton *applyButton;

    void setupUI();
    void detectMouse();
    void getCurrentMouseSpeed(); // <-- добавлено!
};

#endif // MAINWINDOW_H
