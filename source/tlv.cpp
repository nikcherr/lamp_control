#include <tlv.h>
#include <cstring>

namespace tlv{

char TLV::getType() const
{
    return pType;
}

uint16_t TLV::getLength() const
{
    return pLength;
}

const unsigned char* TLV::getData() const
{
    return pValue.data();
}

const std::vector<unsigned char>& TLV::getValue() const
{
    return pValue;
}

}
