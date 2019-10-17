#include <parser.h>
#include <QThread>

storage Parser::parseData(const QByteArray& data)
{
    storage container;
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

storage Parser::multiThreadParseData(const QByteArray& data)
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
    storage result(offsets.size());
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
