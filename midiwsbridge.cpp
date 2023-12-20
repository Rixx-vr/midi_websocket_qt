#include "midiwsbridge.h"
#include <QDebug>
#include <QWebSocket>

MidiWsBridge::MidiWsBridge() :
    midiIn(new QMidiIn()),
    ws(new QWebSocketServer(QStringLiteral("MIDI Websocket Bridge"), QWebSocketServer::NonSecureMode, this))
{ }

QMap<QString, QString> MidiWsBridge::getDevices()
{
    return  this->midiIn->devices();
}



void MidiWsBridge::midiEvent(quint32 message, quint32 timing)
{
    Q_UNUSED(timing);
    int controll = message & 0xff;
    int note = (message>>8) & 0xff;
    int velocity = (message>>16) & 0xff;

    QString message_string = QString("%1-%2-%3").arg(
        QString("%1").arg(controll, 2, 16, QChar('0')),
        QString("%1").arg(note, 2, 16, QChar('0')),
        QString("%1").arg(velocity, 2, 16, QChar('0'))
    );

    qDebug() << message_string;

    foreach (QWebSocket *client, this->clients) {
        client->sendTextMessage(message_string);
    }
}



void MidiWsBridge::bindPort(quint16 port)
{
    if (this->ws->listen(QHostAddress::Any, port)) {
        qDebug() << "Echoserver listening on port" << port;
        connect(this->ws, &QWebSocketServer::newConnection,
                this, &MidiWsBridge::onNewConnection);
        connect(this->ws, &QWebSocketServer::closed, this, &MidiWsBridge::socketDisconnected);
    }
}

void MidiWsBridge::onNewConnection()
{
    QWebSocket *client = this->ws->nextPendingConnection();
    connect(client, &QWebSocket::disconnected, this, &MidiWsBridge::socketDisconnected);
    qDebug() << "Disconnected:" << client;
    this->clients << client;

}

void MidiWsBridge::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Disconnected:" << client;

    if (client) {
        this->clients.removeAll(client);
        client->deleteLater();
    }
}

void MidiWsBridge::connectMidi(QString device)
{
    this->midiIn->connect(device);

    this->connect(this->midiIn, &QMidiIn::midiEvent, this, &MidiWsBridge::midiEvent);
    this->midiIn->start();
}

void MidiWsBridge::disconnect()
{
    this->midiIn->stop();
    foreach (QWebSocket *client, this->clients) {
        this->clients.removeAll(client);
        client->deleteLater();
    }
    this->ws->close();
    this->ws->disconnect();
}
