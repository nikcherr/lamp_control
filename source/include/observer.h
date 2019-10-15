#pragma once
#include <tlv.h>

class Observer
{
  public:
    virtual ~Observer() { }
    virtual void update(const tlv::TLV& package) = 0;
};
