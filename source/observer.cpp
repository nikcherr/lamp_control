#include <observer.h>

Observer::Observer(UpdateDelegate fun)
    : delegate(fun) { }

void Observer::update(const tlv::TLV &package)
{
    if(delegate)
        delegate(package);
}
