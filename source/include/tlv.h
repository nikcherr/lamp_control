#pragma once
#include <QByteArray>
#include <vector>

namespace tlv {

class TLV
{
public:
    TLV(){ }
    template<typename Type>
    explicit TLV(char type, uint16_t length, Type&& value);
    char getType() const;
    uint16_t getLength() const;
    const unsigned char* getData() const;
    const std::vector<unsigned char>& getValue() const;

private:
    char pType;
    uint16_t pLength = 0;
    std::vector<unsigned char> pValue;
};

template<typename Type>
TLV::TLV(char type, uint16_t length, Type&& value) : pType(type), pLength(length)
{
    pValue = std::forward<std::vector<unsigned char>>(value);
}

}
