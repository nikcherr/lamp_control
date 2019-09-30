#pragma once
#include <tlv.h>
#include <memory>

class Observer
{
  public:
    virtual ~Observer() { }
    virtual void update(const tlv::TLV& package) = 0;
};
