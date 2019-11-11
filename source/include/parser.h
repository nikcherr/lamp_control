#pragma once
#include <QThread>
#include <future>
#include <deque>
#include <tlv.h>
#include <paginator.h>
#include <iostream>

template<typename ElemType, template<typename> class Storage = std::deque>
class Parser
{
public:
    static Storage<ElemType> parseData(const QByteArray& data);
    static Storage<ElemType> multiThreadParseData(const QByteArray& data);
    template<typename Container>
    static void parseData(Container& section, const QByteArray& data, int begin);

private:
    static uint16_t swapOctects(uint16_t w)
    {
        return ((w << 8) | (w >> 8));
    }
};

template<typename ElemType, template<typename> class Storage>
Storage<ElemType> Parser<ElemType, Storage>::multiThreadParseData(const QByteArray& data)
{
    int threadCount;
    threadCount = QThread::idealThreadCount();
    std::vector<int> offsets;
    uint16_t tmp_length = 0;
    int offset = 0;
    while(offset < data.size())
    {
        offset += sizeof(char);
        tmp_length = swapOctects(*(uint16_t *)(data.begin() + offset));
        offset += sizeof(uint16_t);
        offset += tmp_length;
        offsets.push_back(offset);
    }
    std::vector<std::future<void>> f;
    offset = 0;
    int shift = 1;
    Storage<ElemType> result(offsets.size());
    for(auto page : Paginate(result, offsets.size() / threadCount)){
        int end = offsets[shift * page.size() - 1];

        f.push_back(std::async(std::launch::async, [=](){
            parseData(page, data, offset);
        }));

        offset = end;
        ++shift;
    }
    return result;
}

template<typename ElemType, template<typename> class Storage>
Storage<ElemType> Parser<ElemType, Storage>::parseData(const QByteArray& data)
{
    Storage<ElemType> container;
    char tmp_type;
    uint16_t tmp_length = 0;
    int offset = 0;
    while(offset < data.size())
    {
        tmp_type = *(char*)(data.begin() + offset);
        offset += sizeof(char);
        tmp_length = swapOctects(*(uint16_t *)(data.begin() + offset));
        offset += sizeof(uint16_t);
        container.emplace_back(tlv::TLV(tmp_type, tmp_length, std::vector<unsigned char>(data.begin() + offset, data.begin() + offset + tmp_length)));
        offset += tmp_length;
    }
    return container;
}

template<typename ElemType, template<typename> class Storage>
template<typename Container>
void Parser<ElemType, Storage>::parseData(Container& section, const QByteArray& data, int begin)
{
    char tmp_type;
    uint16_t tmp_length = 0;
    int offset = begin;
    for(auto& item : section)
    {
        tmp_type = *(char*)(data.begin() + offset);
        offset += sizeof(char);
        tmp_length = swapOctects(*(uint16_t *)(data.begin() + offset));
        offset += sizeof(uint16_t);
        item = tlv::TLV(tmp_type, tmp_length, std::vector<unsigned char>(data.begin() + offset, data.begin() + offset + tmp_length));
        offset += tmp_length;
    }
}
