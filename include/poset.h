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

    void addInequality(std::size_t a, std::size_t b) // ADD a>=b
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

    void generateLinearExtension(std::queue<std::vector<std::size_t>>& q);

    void findAllLinearExtensions(std::vector<std::size_t>& lin, std::queue<std::vector<std::size_t>>& q)
    {
        findAllChildren(lin, lin.size(), q);
    }

private:
    std::vector<std::vector<bool>> order;
    bool isPlus;
    std::vector<std::size_t> a, b, le, li;
    std::size_t size, maxPair;

    void move(std::vector<std::size_t>& vec, std::size_t index, std::ptrdiff_t change)
    {
        std::swap(vec[index], vec[index+change]);
    }

    void moveA(std::size_t i, direction d)
    {
        if(i==0 && d == leftdir)
        {
            //hi
            return;
        }
        std::swap(le[a[i]], le.at(a[i]+d));
        if(li.at(a[i]+d) < size && a.at(li[a[i]+d])==a[i]+d)
        {
            std::swap(a[i], a[li[a[i]+d]]);
            std::swap(li[a[i]], li[a[i]-d]);
        }
        else if (li[a[i]+d] < size && b[li[a[i]+d]]==a[i]+d)
        {
            std::swap(a[i], b[li[a[i]+d]]);
            std::swap(li[a[i]], li[a[i]-d]);
        }
        else
        {
            a[i] = a[i]+d;
            std::swap(li[a[i]], li[a[i]-d]);
        }
    }

    void moveB(std::size_t i, direction d)
    {
        b[i] = b.at(i);
        std::swap(le.at(b[i]), le.at(b[i]+d));
        if(li.at(b[i]+d) < size && a.at(li[b[i]+d])==b[i]+d)
        {
            std::swap(b[i], a[li.at(b[i]+d)]);
            std::swap(li.at(b[i]), li.at(b[i]-d));
        }
        else if(li.at(b[i]+d) < size && b.at(li.at(b[i]+d))==b[i]+d)
        {
            std::swap(b[i], b[li[b[i]+d]]);
            std::swap(li[b[i]], li[b[i]-d]);
        }
        else
        {
            b[i] = b[i]+d;
            std::swap(li[b[i]], li[b[i]-d]);
        }
    }

    bool rightB(std::size_t i)
    {
        return b[i]+1 < size && !comparable(le[b[i]], le[b[i]+1]);
    }

    bool rightA(std::size_t i)
    {
        return a[i]+1 < size && !comparable(le[a[i]], le[a[i]+1]) && a[i]+1 != b[i];
    }

    void signSwitch(std::size_t i)
    {
        if (i==0)
        {
            isPlus = !isPlus;
        }
        else {
            std::swap(le[a[i]], le[b[i]]);
            // DO NOT std::swap(a[i], b[i]) - a, b are the leftmost, rightmost of the pair after switch, this does not change
        }
    }

    void preprocess();
    void generateLinearExtension(std::size_t i, std::queue<std::vector<std::size_t>>&);

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
