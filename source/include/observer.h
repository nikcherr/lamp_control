#pragma once
#include <tlv.h>
#include <functional>

class Observer
{
public:
    using UpdateDelegate = std::function<void(const tlv::TLV&)>;

    Observer(UpdateDelegate fun);
    ~Observer() = default;
    void update(const tlv::TLV& package);
protected:
    UpdateDelegate delegate;
};
