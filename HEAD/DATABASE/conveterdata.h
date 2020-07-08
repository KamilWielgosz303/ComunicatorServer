#ifndef DATABASERESPONSE_H
#define DATABASERESPONSE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>
#include <QDebug>
#include <QMap>
#include <QVector>

#include "HEAD/conststaticflags.h"

//Class executes a request to database and return the result.


class ConveterData : public QObject
{
    Q_OBJECT
public:
    ConveterData(QSqlDatabase database, qintptr ID);
    ~ConveterData();
signals:
    void readyToSend(QByteArray);
public slots:
    void exec(QByteArray);
    void disconnected();
private:
    // Confirming method sends a status of request. (Execute positive or negative)
    void C_Confirm(QString, QString errorCode=FLAG::ERR);
    QVector<QByteArray> C_UserPsswdDecoding(QByteArray);

    /////////////////////////////////////////////////////////////
    void C_LOGIN_DbRequest(QVector<QByteArray>);

    void C_SEND_DbRequest(QByteArray nick, QByteArray message);
    void C_SEND_decoding(QByteArray);

    QMap<int,QString> C_ULIST_DbRequest();
    void C_ULIST_coding(QMap<int,QString>);

    void C_GET_MSG_decoding(QByteArray);
    void C_GET_MSG_DbRequest(QByteArray nick);
    void C_MESSAGES_coding(QByteArray sender, QByteArray message, QByteArray date);

    void C_REGISTER_DbRequest(QVector<QByteArray>);

    QByteArray getHeader(QByteArray);
    bool _result;
    int _idUser;
    QSqlQuery request;
    QSqlDatabase database;
};

#endif // HEAD\DATABASE\DATABASERESPONSE_H
