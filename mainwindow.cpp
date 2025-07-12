#include "mainwindow.h"
#include <QMessageBox>
#include <QFont>
#include <windows.h>
#include <QDebug>

#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>

#pragma comment(lib, "Setupapi.lib")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    detectMouse();
    getCurrentMouseSpeed();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("Lunarch Mouse Config");
    resize(400, 200);

    labelMouse = new QLabel("Detecting mouse...", this);
    labelMouse->setGeometry(20, 20, 360, 30);
    labelMouse->setStyleSheet("color: white; font-size: 16px;");

    slider = new QSlider(Qt::Horizontal, this);
    slider->setGeometry(20, 70, 360, 30);
    slider->setRange(1, 20);

    applyButton = new QPushButton("Apply", this);
    applyButton->setGeometry(150, 130, 100, 30);

    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyMouseSpeed);

    setStyleSheet("background-color: #1E1E2F;");
}

void MainWindow::detectMouse() {
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_MOUSE, NULL, NULL, DIGCF_PRESENT);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        labelMouse->setText("Failed to get devices.");
        return;
    }

    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    bool found = false;

    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &devInfoData); ++i) {
        TCHAR buffer[256];
        DWORD buffersize = 0;

        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &devInfoData, SPDRP_DEVICEDESC,
                                             NULL, (PBYTE)buffer, sizeof(buffer), &buffersize)) {
            QString deviceName = QString::fromWCharArray(buffer);
            if (deviceName.contains("Logitech", Qt::CaseInsensitive)) {
                labelMouse->setText("Detected: " + deviceName);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        labelMouse->setText("No Logitech mouse detected.");
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

void MainWindow::getCurrentMouseSpeed() {
    int speed = 0;
    SystemParametersInfo(SPI_GETMOUSESPEED, 0, &speed, 0);
    slider->setValue(speed);
}

void MainWindow::applyMouseSpeed() {
    int speed = slider->value();
    BOOL result = SystemParametersInfo(SPI_SETMOUSESPEED, 0, (PVOID)speed, SPIF_SENDCHANGE);

    if (result) {
        QMessageBox::information(this, "Success", "Mouse speed set to " + QString::number(speed));
    } else {
        QMessageBox::warning(this, "Error", "Failed to set mouse speed.\nTry running as administrator.");
    }
}
