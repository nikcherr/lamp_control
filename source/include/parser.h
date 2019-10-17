#pragma once
#include <future>
#include <deque>
#include <tlv.h>
#include <paginator.h>
#include <iostream>

using storage = std::deque<tlv::TLV>;

class Parser
{
public:
    static storage parseData(const QByteArray& data);
    static storage multiThreadParseData(const QByteArray& data);
    template<typename Container>
    static void parseData(Container& section, const QByteArray& data, int begin);

private:
    static uint16_t swapOctects(uint16_t w)
    {
        return ((w << 8) | (w >> 8));
    }
};

template<typename Container>
void Parser::parseData(Container& section, const QByteArray& data, int begin)
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
