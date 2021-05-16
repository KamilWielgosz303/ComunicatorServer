#include "src/SERVER/server.h"



Server::Server(quint16 port, QObject *parent) : QTcpServer(parent)
{
    if(!this->listen(QHostAddress::Any,port))qDebug() << "\nWeb server could not start";
    else {
        qDebug() <<"\nServer is waiting for a connection on port " << port << "\r\n";
        clearLoggedTable();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...\r\n";

    thread = new NewClient(socketDescriptor, this);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Server::clearLoggedTable()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DATABASE_PATH);
    database.open();
    QSqlQuery request = database.exec(
                "DELETE FROM Logged");
            if(request.isActive()){
                    qDebug() << "Logged table is clear." << endl;
            }else {
                qDebug() << "Logged table is NOT clear." << endl;
                qDebug() << request.lastError().databaseText();
            }
    database.close();
}

Server::~Server()
    {
    thread->terminate();
    delete thread;
    socket->close();
}
