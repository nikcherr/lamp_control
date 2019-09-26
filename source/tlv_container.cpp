#include <tlv_container.h>
#include <iostream>
#include <thread>
#include <vector>

namespace tlv{

TLVContainer::TLVContainer() : QObject () { }

std::vector<tlv_shared_ptr> TLVContainer::parseSocketData(const QByteArray& data)
{
    std::vector<tlv_shared_ptr> result;
    char tmp_type;
    uint16_t tmp_length = 0;
    int offset = 0;

    while(offset < data.size())
    {
        tmp_type = *(char*)(data.begin() + offset);
        offset += sizeof(char);
        tmp_length = swapOctects(*(uint16_t *)(data.begin() + offset));
        offset += sizeof(uint16_t);
        result.push_back(std::make_shared<TLV>(tmp_type, tmp_length, std::vector<unsigned char>(data.begin() + offset, data.begin() + offset + tmp_length)));
        offset += tmp_length;
    }
    return result;
}

void TLVContainer::attach(Observer *obs) const
{
    m_views.push_back(obs);
}

void TLVContainer:: notify(tlv_shared_ptr packages)
{
    for(auto view : m_views)
    {
        view->update(packages);
    }
}


void TLVContainer::getFromSocket(const QByteArray& data)
{
    packages = parseSocketData(data);
    for(const auto& package : packages)
    {
        notify(package);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

uint16_t TLVContainer::swapOctects(uint16_t w)
{
    return ((w << 8) | (w >> 8));
}

}
