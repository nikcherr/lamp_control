#pragma once
#include <iterator_range.h>
#include <vector>

template<typename Iterator>
class Paginator
{
private:
  std::vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size);
  auto begin() const;
  auto end() const;
  size_t size() const;
};

template<typename Iterator>
Paginator<Iterator>::Paginator(Iterator begin, Iterator end, size_t page_size)
{
    for (size_t left = distance(begin, end); left > 0; )
    {
      size_t current_page_size = std::min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
}

template<typename Container>
auto Paginate(Container& c, size_t page_size)
{
  return Paginator(begin(c), end(c), page_size);
}

template<typename Iterator>
auto Paginator<Iterator>::begin() const
{
    return pages.begin();
}

template<typename Iterator>
auto Paginator<Iterator>::end() const
{
    return pages.end();
}

template<typename Iterator>
size_t Paginator<Iterator>::size() const
{
    return pages.size();
}
