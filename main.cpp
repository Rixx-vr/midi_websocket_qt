#include "mainwindow.h"
#include "midiwsbridge.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MidiWsBridge bridge;
    bridge.connect(&w, &MainWindow::onDeviceSelected, &bridge, &MidiWsBridge::connectMidi);
    bridge.connect(&w, &MainWindow::onPortSelected, &bridge, &MidiWsBridge::bindPort);
    bridge.connect(&w, &MainWindow::onDisconnect, &bridge, &MidiWsBridge::disconnect);
    w.onDeviceList(bridge.getDevices());
    w.show();
    return a.exec();
}
