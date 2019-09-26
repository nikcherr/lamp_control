#pragma once
#include <tlv.h>
#include <memory>

class Observer
{
  public:
    virtual ~Observer() { }
    virtual void update(std::shared_ptr<tlv::TLV> package) = 0;
};
