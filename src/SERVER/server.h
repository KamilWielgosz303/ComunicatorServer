#ifndef SERVER_H
#define SERVER_H

#include <QCoreApplication>
#include <QNetworkInterface>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include "src/SERVER/newclient.h"
#include "src/config.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(quint16 port=50000, QObject *parent = nullptr);
    ~Server() override;
    QTcpSocket *socket ;
    void startServer();
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void clearLoggedTable();

    NewClient *thread;
};

#endif // SERVER_H
