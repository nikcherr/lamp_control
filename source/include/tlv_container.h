#pragma once
#include <vector>
#include <memory>
#include <tlv.h>
#include <observer.h>
#include <QObject>

namespace tlv{

using tlv_shared_ptr = std::shared_ptr<TLV>;

class TLVContainer : public QObject
{
    Q_OBJECT

public:
    TLVContainer();
    std::vector<tlv_shared_ptr> parseSocketData(const QByteArray& data);
    void attach(Observer* obs) const;
    void notify(tlv_shared_ptr package);

public slots:
    void getFromSocket(const QByteArray& data);

private:
    std::vector<tlv_shared_ptr> packages;
    mutable std::vector<Observer*> m_views;
    uint16_t swapOctects(uint16_t w);
};

}
