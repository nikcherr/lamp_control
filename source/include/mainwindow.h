#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <tcp_client.h>
#include <tlv_container.h>
#include <render_lantern.h>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onClientStateChange(QAbstractSocket::SocketState);
    void tryToConnect();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QTimer> connection_timer;
    std::unique_ptr<TCPClient, decltype(custom_deleter)> client;
    std::shared_ptr<tlv::TLVContainer> container;
    std::unique_ptr<RenderLantern> lantern;
    std::string address = "127.0.0.1";
    unsigned short port = 9999;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
