#ifndef NEWCONTACTSERVER_H
#define NEWCONTACTSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QSqlDatabase>
#include "src/DATABASE/convertrequest.h"
#include "src/config.h"

//Class operates one new client, receives request and send response.

class NewClient : public QThread
{
    Q_OBJECT
public:
    explicit NewClient(qintptr ID, QObject *parent);
    ~NewClient() override;
    void run() override;
signals:
    void error(QTcpSocket::SocketError socketerror);
public slots:
    void readyRead();
    void disconnected();
    void writeToSocket(QByteArray);
private:
    QTcpSocket *socket{nullptr};
    qintptr socketDescriptor;
    ConvertRequest *conveterQuery{nullptr};
};

#endif // NEWCONTACTSERVER_H
