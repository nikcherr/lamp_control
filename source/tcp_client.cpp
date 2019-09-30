#include "tcp_client.h"
#include <iostream>
#include <QDataStream>

TCPClient::TCPClient()
{
    socket = new QTcpSocket(this);
}

QTcpSocket* TCPClient::getSocket() const
{
    return socket;
}

void TCPClient::run(const std::string& address, unsigned short port)
{
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readSocketData()));
    socket->connectToHost(QString::fromStdString(address), port);
    if(socket->waitForConnected(2000))
    {
        std::cout << "Connection is established";
    }
    else
    {
        std::cerr << "Connection timeout.";
    }
}

void TCPClient::tryGetSocket(const std::string& address, unsigned short port)
{
    socket->connectToHost(QString::fromStdString(address), port);
}

void TCPClient::readSocketData()
{
    emit send_to_TLVContainer(socket->readAll());
}
