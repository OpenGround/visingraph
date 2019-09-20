#ifndef POSET_H
#define POSET_H

#include <vector>
#include <queue>

enum direction : std::ptrdiff_t {
    leftdir = -1,
    rightdir = 1
};

class Poset
{
public:
    Poset() {}
    Poset(std::size_t size)
    {
        this->size = size;
        order.resize(size);
        for(std::size_t i =0; i<size; ++i)
        {
            order[i].resize(size, false);
        }
    }

    void addInequality(std::size_t a, std::size_t b) // ADD a<=b
    {
        std::queue<std::pair<std::size_t, std::size_t>> q;
        q.push(std::make_pair(a,b));

        while (!q.empty())
        {
            auto p = q.front();
            if(!order[p.first][p.second])
            {
                order[p.first][p.second] = true;
                for(std::size_t i=0; i<size; i++)
                {
                    if(order[i][p.first] && ! order[i][p.second])
                        q.push(std::make_pair(i, p.second));

                    if(!order[p.first][i] && order[p.second][i])
                        q.push(std::make_pair(p.first, i));
                }
            }
            q.pop();
        }
    }

    bool lte(std::size_t i, std::size_t j) // i <= j
    {
        if(i >= size || j >= size)
            return false;
        return order[i][j];
    }

    bool comparable(std::size_t i, std::size_t j)
    {
        return lte(i, j) || lte (j, i);
    }

    void findAllLinearExtensions(std::vector<std::size_t>& lin, std::queue<std::vector<std::size_t>>& q)
    {
        findAllChildren(lin, lin.size(), q);
    }

private:
    std::vector<std::vector<bool>> order;
    std::size_t size;

    /*
     * Implementation of the algorithm by Akimitsu Ono & Shin-ichi Nakano from
     * "Constant Time Generation of Linear Extensions"
     */
    void findAllChildren(std::vector<std::size_t>& lin, std::size_t level, std::queue<std::vector<std::size_t>>& q)
    {
        q.push(lin);
        if(level == 0)
            return;
        std::size_t j;
        for(std::size_t i = 0; i < level; i++)
        {
            j = i;
            while(j < lin.size()-1 && !lte(lin[j], lin[j+1]))
            {
                std::swap(lin[j], lin[j+1]);
                findAllChildren(lin, i, q);
                j++;
            }
            while(j>i)
            {
                std::swap(lin[j], lin[j-1]);
                j--;
            }
        }
    }
};

#endif // POSET_H
