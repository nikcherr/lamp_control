#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memory>
#include <tcp_client.h>

using namespace tlv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new TCPClient, custom_deleter)
{
    ui->setupUi(this);
    connection_timer = std::make_unique<QTimer>();
    container = std::make_shared<TLVContainer>();
    lantern = std::make_unique<RenderLantern>(this, container.get());

    if(QCoreApplication::arguments().size() > 1){
        address = QCoreApplication::arguments().at(1).toStdString();
        port = QCoreApplication::arguments().at(2).toUShort();
    }

    QObject::connect(client.get(), SIGNAL(send_to_TLVContainer(const QByteArray&)), container.get(), SLOT(getFromSocket(const QByteArray&)));
    QObject::connect(client->getSocket(), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onClientStateChange(QAbstractSocket::SocketState)));
    QObject::connect(connection_timer.get(), SIGNAL(timeout()), this, SLOT(tryToConnect()));

    client->run(address, port);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tryToConnect()
{
    client->tryGetSocket(address, port);
}

void MainWindow::onClientStateChange(QAbstractSocket::SocketState state)
{
    switch(state) {
    case QAbstractSocket::SocketState::HostLookupState:
    case QAbstractSocket::SocketState::ConnectingState:
        connection_timer->stop();
        this->setWindowTitle("Connecting...");
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        this->setWindowTitle("Connected to server");
        break;
    case QAbstractSocket::SocketState::ClosingState:
    case QAbstractSocket::SocketState::UnconnectedState:
        connection_timer->start(2000);
        this->setWindowTitle("Connection timeout");
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);
    lantern->drawLed(&p);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    lantern->resize();
}
