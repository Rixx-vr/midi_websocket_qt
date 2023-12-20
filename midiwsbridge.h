#ifndef MIDIWSBRIDGE_H
#define MIDIWSBRIDGE_H

#include <QObject>

#include <QMidiOut.h>
#include <QMidiIn.h>
#include <QMidiFile.h>
#include <QWebSocketServer>

class MidiWsBridge: public QObject
{
Q_OBJECT

public:
    MidiWsBridge();
    QMap<QString, QString> getDevices();

public slots:
    void midiEvent(quint32 message, quint32 timing);
    void bindPort(quint16 port = 12345);
    void onNewConnection();
    void socketDisconnected();
    void connectMidi(QString device);
    void disconnect();

private:
    QMidiIn *midiIn;
    QWebSocketServer *ws;
    QList<QWebSocket *> clients;
};

#endif // MIDIWSBRIDGE_H
