#include "HEAD/SERVER/newcontactserver.h"

NewContactServer::NewContactServer(qintptr ID, QObject *parent) : QThread(parent)
{
        this->socketDescriptor = ID;
        auto name = "my_db_" + QString::number((quint64)QThread::currentThread(), 16);
        if(QSqlDatabase::contains(name)){
            QSqlDatabase database = QSqlDatabase::database(name);
            database.setDatabaseName(DATABASE_PATH);
            this->conveterQuery = new ConveterData(database, ID);
        }else {
            QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", name);
            database.setDatabaseName(DATABASE_PATH);
        this->conveterQuery = new ConveterData(database, ID);
        }
}

NewContactServer::~NewContactServer()
{
    if(conveterQuery != nullptr)
        delete conveterQuery;
    if(socket != nullptr)
        delete socket;
}

void NewContactServer::run()
{
    qDebug() << " Thread started" << endl;

    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(conveterQuery, SIGNAL (readyToSend(QByteArray)), this, SLOT(writeToSocket(QByteArray)), Qt::DirectConnection);

    qDebug() << socketDescriptor << " Client connected" << endl;

    exec();
}

void NewContactServer::readyRead()
{
    QByteArray becameData = socket->readAll();
    qDebug() << socketDescriptor << "Arrived data=" << endl;
    qDebug() << becameData << endl;
    conveterQuery->exec(becameData);
}

void NewContactServer::writeToSocket(QByteArray dataToSend)
{
    socket->waitForBytesWritten();
    socket->write(dataToSend);
    socket->flush();
}

void NewContactServer::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected" << endl;
    conveterQuery->disconnected();
    conveterQuery->~ConveterData();
    socket->deleteLater();
    exit(0);
}
