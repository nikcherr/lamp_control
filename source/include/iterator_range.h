#pragma once
#include <algorithm>

template<typename Iterator>
class IteratorRange{
public:
    IteratorRange(Iterator begin, Iterator end)
        : _begin(begin), _end(end), _size(distance(_begin, _end)) { }
    Iterator begin() const;
    Iterator end() const;
    size_t size() const;
private:
    Iterator _begin;
    Iterator _end;
    size_t _size;
};

template<typename Iterator>
Iterator IteratorRange<Iterator>::begin() const
{
    return _begin;
}

template<typename Iterator>
Iterator IteratorRange<Iterator>::end() const
{
    return _end;
}

template<typename Iterator>
size_t IteratorRange<Iterator>::size() const
{
    return _size;
}
