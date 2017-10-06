#pragma once

#include <set>
#include <functional>

template <typename T>
class PriorityQueue
{
public:
    bool empty() const
    {
        return this->set.size() == 0;
    }

    void push(const T& t)
    {
        this->set.insert({t});
    }
    T top()
    {
        return *this->set.begin();
    }
    void pop()
    {
        this->set.erase(this->set.begin());
    }

    bool contains(const T& t)
    {
        return this->set.find(t) != this->set.end();
    }

private:
    std::set<T> set;
};
