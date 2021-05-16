#include "src\DATABASE\conveterdata.h"

using namespace FLAG;

ConveterData::ConveterData(const QSqlDatabase base, qintptr ID)
{
    this->_database = base;
    _result = false;
    if(!_database.open()){
        qDebug() << ID<< ERROR << _database.lastError();
    }else{
        qDebug() << ID << ": Connected to the database!" << endl;
    }
}


void ConveterData::exec(QByteArray data)
{
    QByteArray header = getHeader(data);
    if(header == C_ULIST){
        C_ULIST_coding(C_ULIST_DbRequest());
    }else if(header == C_GET_MSG){
            C_GET_MSG_decoding(data);
          }else if(header == C_SEND){
                    C_SEND_decoding(data);
                }else if(header == C_LOGIN){
                            C_LOGIN_DbRequest(C_UserPsswdDecoding(data));
                        }else if(header == C_REGISTER){
                                    C_REGISTER_DbRequest(C_UserPsswdDecoding(data));
                            }else{
                                    qDebug() << "Header compare is wrong!" << endl;
                            }
}

QByteArray ConveterData::getHeader(QByteArray data)
{
    if(data[0] == static_cast<char>(1) && data[data.length()-3]==static_cast<char>(30)){
         return data.mid(1,data.indexOf(static_cast<char>(2))-1);
    }else{
        qDebug() << "Header not finded!" << endl;
        return nullptr;
    }

}

QVector<QByteArray> ConveterData::C_UserPsswdDecoding(QByteArray data)
{
    QByteArray nick =
            data.mid(data.indexOf(static_cast<char>(2))+1,
                     data.indexOf(static_cast<char>(3))-data.indexOf(static_cast<char>(2))-1);
    QByteArray password =
            data.mid(data.indexOf(static_cast<char>(3))+1,
                     data.indexOf(static_cast<char>(30))-data.indexOf(static_cast<char>(3))-1);
    QVector<QByteArray> vec = *new QVector<QByteArray>(2);
    vec[0]=nick;
    vec[1]=password;
    qDebug() << vec.at(0) << endl;
    qDebug() << vec.at(1) << endl;
    return vec;
}

void ConveterData::C_LOGIN_DbRequest(QVector<QByteArray> vec){
    QSqlQuery request = _database.exec(
                "SELECT ID_user,Nick,Password, s.ID_user "
                "FROM Users LEFT NATURAL JOIN Logged s "
                "WHERE Nick='"+vec.at(0)+"' AND Password='"+vec.at(1)+"' AND s.ID_user IS NULL;");
            qDebug() << "Danych:"<< endl << request.at()+1;
            if(request.isActive() && (request.at()+1==0)){
                _result = true;
                if(request.first()){
                    QString id = request.value(0).toString();
                    qDebug() << id;
                    _idUser = id.toInt();
                    QString query= "INSERT INTO Logged (ID_user) VALUES (";
                    query.append(id).append(");");
                    qDebug() << query;
                    QSqlQuery log_in_user = _database.exec(
                      query);
                    if(!request.isActive())
                    {
                        qDebug() << ERROR << log_in_user.lastError().databaseText();
                        _result = false;
                    }else C_Confirm(S_LOGIN);
                }
            }else {
                qDebug() << ERROR << request.lastError().databaseText();
                _result = false;
                C_Confirm(S_LOGIN);
            }
}

void ConveterData::C_Confirm(QString headerFlag, QString errorCode)
{
    QString resultToSend;
    if(_result){
        resultToSend = QLatin1Char(1) + headerFlag + QLatin1Char(2) + ACT + QLatin1Char(30) + NEW_LINE;
        qDebug() << resultToSend << endl;
        emit readyToSend(resultToSend.toUtf8());
    }else{
        resultToSend = QLatin1Char(1) + headerFlag + QLatin1Char(2) + errorCode + QLatin1Char(30) + NEW_LINE;
        qDebug() << resultToSend << endl;
        emit readyToSend(resultToSend.toUtf8());
    }
}

void ConveterData::C_SEND_decoding(QByteArray data)
{
    QByteArray receiver =
            data.mid(data.indexOf(static_cast<char>(2))+1,
                     data.indexOf(static_cast<char>(3))-data.indexOf(static_cast<char>(2))-1);
    QByteArray message =
            data.mid(data.indexOf(static_cast<char>(3))+1,
                     data.indexOf(static_cast<char>(30))-data.indexOf(static_cast<char>(3))-1);
    qDebug() << receiver << endl;
    qDebug() << message << endl;
    C_SEND_DbRequest(receiver,message);
}

void ConveterData::C_SEND_DbRequest(QByteArray nick, QByteArray message)
{
    bool convertInt;
    int temp = nick.toInt(&convertInt);
    qDebug() << temp << endl;
    if(!convertInt){
        _result = false;
        C_Confirm(S_SEND);
        qDebug() << "Failed parse ID_receiver! Message will not insert to a database!" << endl;
    }else{
        QString date = QDateTime::currentDateTime().toString(FORMAT_DATA);
        QString str = "INSERT INTO Messages (ID_sender,ID_receiver,Message,Date) VALUES (";
        str.append(QString::number(_idUser))
                .append(',')
                .append(QString::number(temp))
                .append(",'")
                .append(message)
                .append("','")
                .append(date)
                .append("');");
        qDebug() << str << endl;
        QSqlQuery request = _database.exec(
                    str);
        if(!request.isActive())
        {
            qDebug() << ERROR << request.lastError().databaseText();
            _result = false;
            C_Confirm(S_SEND);
        }else{
            _result = true;
            C_Confirm(S_SEND);
        }
    }
}


QMap<int,QString> ConveterData::C_ULIST_DbRequest()
{
    if(!_database.isOpen())
        _database.open();
    QMap<int,QString> userList;
    QString str =
            "SELECT ID_user,Nick "
            "FROM Users NATURAL JOIN Logged "
            "WHERE ID_user <> "
            + QString::number(_idUser)
            + ";";
    QSqlQuery request = _database.exec(
                str);
    if(request.isActive()){
        while(request.next()){
            userList[request.value(0).toInt()] = request.value(1).toString();
            qDebug() << "\nID:" << request.value(0).toInt() <<
                        " Nick: " << request.value(1).toString() << endl;
        }
        _result = true;
        return userList;
    }else{
        qDebug() << ERROR << request.lastError().databaseText();
        _result = false;
        return userList;
    }

}
void ConveterData::C_ULIST_coding(QMap<int,QString> map)
{
    QString resultToSend;
    if(!_result){
        resultToSend = QLatin1Char(1) + S_ULIST + QLatin1Char(2) + ERR + QLatin1Char(30) + NEW_LINE;
        qDebug() << resultToSend << endl;
    }else{
        QMapIterator<int,QString> iterator(map);
        resultToSend = QLatin1Char(1) + S_ULIST + QLatin1Char(2);
        while(iterator.hasNext()){
            iterator.next();
            qDebug() << iterator.key() << ": " << iterator.value() << endl;
            resultToSend += QString::number(iterator.key()) + QLatin1Char(3) + iterator.value() + QLatin1Char(4);
        }
        resultToSend += QLatin1Char(30) + NEW_LINE;
        qDebug() << resultToSend << endl;
        emit readyToSend(resultToSend.toUtf8());
    }
}

void ConveterData::C_GET_MSG_decoding(QByteArray data)
{
    QByteArray ID_User =
            data.mid(data.indexOf(static_cast<char>(2))+1,
                     data.indexOf(static_cast<char>(30))-data.indexOf(static_cast<char>(2))-1);
    qDebug() << ID_User << endl;
    C_GET_MSG_DbRequest(ID_User);
}
void ConveterData::C_GET_MSG_DbRequest(QByteArray ID_User)
{
    if(!_database.isOpen())
        _database.open();
    QString str =
            "SELECT ID_sender, Message, Date "
            "FROM Messages WHERE (ID_receiver="
            + ID_User +" AND ID_sender="
            + QString::number(_idUser)
            + ") OR (ID_sender="
            + ID_User + " AND ID_receiver="
            + QString::number(_idUser)
            + ") ORDER BY Date DESC LIMIT 5;";
    qDebug() << str << endl;
    QSqlQuery request = _database.exec(
                str);
    if(request.isActive()){
         _result = true;
         request.last();
         request.next();
        while(request.previous()){
            qDebug() << "\nID:" << request.value(0).toInt() <<
                        " Message: " << request.value(1).toString() <<
                        " Date" << request.value(2).toString() << endl;
            C_MESSAGES_coding(request.value(0).toByteArray(), request.value(1).toByteArray(), request.value(2).toByteArray());
        }
        C_Confirm(S_GET_MSG,ACT);
    }else{
        qDebug() << ERROR << request.lastError().databaseText();
        _result = false;
        C_Confirm(S_GET_MSG);
    }
}

void ConveterData::C_MESSAGES_coding(QByteArray sender, QByteArray message, QByteArray date)
{
    QString resultToSend;
    resultToSend = QLatin1Char(1) + S_GET_MSG + QLatin1Char(2) + sender
                + QLatin1Char(3) + message + QLatin1Char(4) + date + QLatin1Char(30) + NEW_LINE;
    qDebug() << resultToSend << endl;
    emit readyToSend(resultToSend.toUtf8());

}

void ConveterData::C_REGISTER_DbRequest(QVector<QByteArray> vec)
{
    QSqlQuery request = _database.exec(
                "SELECT ID_user,Nick,Password FROM Users WHERE Nick='"+vec.at(0)+"';");
    qDebug() << "Danych:"<< endl << request.at();
    if(request.isActive() && (request.at()==-2)){
        _result = true;
        QString query= "INSERT INTO Users (Nick,Password) VALUES ('";
        query.append(vec.at(0))
                .append("','")
                .append(vec.at(1))
                .append("');");
        QSqlQuery log_in_user = _database.exec(
                    query);
        if(!log_in_user.isActive())
        {
            qDebug() << ERROR <<log_in_user.lastError().databaseText();
            _result = false;
            C_Confirm(S_REGISTER, ERR);
        }else C_Confirm(S_REGISTER);

    }else {
        qDebug() << ERROR << request.lastError().databaseText();
        _result = false;
        C_Confirm(S_REGISTER, NEQ);
    }
}


void ConveterData::disconnected()
{
    QString str =
            "DELETE FROM Logged "
            "WHERE ID_user="
            +  QString::number(_idUser)
            + ";";
    qDebug() << str << endl;
    QSqlQuery request = _database.exec(
                str);
    if(!request.isActive()){
        qDebug() << ERROR << request.lastError().databaseText();
    }
}
ConveterData::~ConveterData()
{
    qDebug() << "Close DB";
    _database.close();
}

