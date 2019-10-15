#pragma once
#include <memory>
#include <tlv.h>
#include <observer.h>
#include <QObject>
#include <deque>

namespace tlv{

class TLVContainer : public QObject
{
    Q_OBJECT

public:
    TLVContainer();
    void attach(Observer* obs) const;
    void notify(const TLV& package);

public slots:
    void getFromSocket(const QByteArray& data);

private:
    std::deque<TLV> packages;
    mutable std::vector<Observer*> m_views;
};

}
