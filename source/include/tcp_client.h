#pragma once
#include <QTcpSocket>
#include <tlv_container.h>
#include <render_lantern.h>

auto custom_deleter = [](QTcpSocket* socket) {
    socket->close();
    delete socket;
};

class TCPClient : public QTcpSocket
{
    Q_OBJECT

public:
    TCPClient();
    void run(const std::string&, unsigned short);
    void tryGetSocket(const std::string& address, unsigned short port);
    QTcpSocket* getSocket() const;

public slots:
    void readSocketData();

signals:
    void send_to_TLVContainer(const QByteArray& data);

private:
    QTcpSocket* socket;
};
