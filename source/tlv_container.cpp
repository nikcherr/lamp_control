#include <tlv_container.h>
#include <iostream>
#include <thread>
#include <paginator.h>

namespace tlv{

TLVContainer::TLVContainer() : QObject () { }

std::deque<TLV> TLVContainer::parseSocketData(const QByteArray& data)
{
    std::deque<TLV> result;
    char tmp_type;
    uint16_t tmp_length = 0;
    int offset = 0;

    while(offset < data.size())
    {
        tmp_type = *(char*)(data.begin() + offset);
        offset += sizeof(char);
        tmp_length = swapOctects(*(uint16_t *)(data.begin() + offset));
        offset += sizeof(uint16_t);
        result.emplace_back(tmp_type, tmp_length, std::vector<unsigned char>(data.begin() + offset, data.begin() + offset + tmp_length));
        offset += tmp_length;
    }
    return result;
}

void TLVContainer::attach(Observer *obs) const
{
    m_views.push_back(obs);
}

void TLVContainer:: notify(const TLV& package)
{
    for(auto view : m_views)
    {
        view->update(package);
    }
}


void TLVContainer::getFromSocket(const QByteArray& data)
{
    packages = parseSocketData(data);
    for(const auto& package : packages)
    {
        notify(package);
    }
}

uint16_t TLVContainer::swapOctects(uint16_t w)
{
    return ((w << 8) | (w >> 8));
}

}
