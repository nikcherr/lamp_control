#include <tlv_container.h>
#include <parser.h>

namespace tlv{

TLVContainer::TLVContainer() : QObject () { }

void TLVContainer::attach(const Observer& obs) const
{
    m_views.push_back(obs);
}

void TLVContainer::notify(const TLV& package)
{
    for(auto& view : m_views)
    {
        view.update(package);
    }
}

void TLVContainer::getFromSocket(const QByteArray& data)
{
    this->packages = Parser<tlv::TLV>::multiThreadParseData(data);
    for(const auto& package : packages)
    {
        notify(package);
    }
}

}
