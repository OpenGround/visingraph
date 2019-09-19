#include "include/poset.h"
#include <set>
#include <algorithm>

void Poset::preprocess()
{
    isPlus=true;
    std::size_t i=0, j=0;
    std::vector<std::pair<std::size_t, std::size_t>> S;
    li.resize(size);
    for(std::size_t i=0; i<size; i++)
    {
        std::size_t less = 0;
        for(std::size_t j=0; j<size; j++)
        {
            if(i !=j && lte(i, j))
            {
                less++;
            }
        }
        S.push_back(std::make_pair(less, i));
    }

    std::sort(S.begin(), S.end(), [](std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b){return a.first<b.first;});

    std::queue<std::size_t> dec, dec2;

    while(S.size()>0)
    {
        std::queue<std::size_t>().swap(dec);
        std::queue<std::size_t>().swap(dec2);
        if(S.size() == 1 || S[1].first == 1)
        {
            le.push_back(S[0].second);
            li[S[0].second] = -1;
            for(std::size_t ii = 1; ii < S.size(); ii++)
            {
                if(lte(S[0].second, S[ii].second))
                {
                    dec.push(ii);
                }
            }
            j++;
            S.erase(S.begin());
            std::sort(S.begin(), S.end(), [](std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b){return a.second<b.second;});
            std::size_t it = 0;
            while(!dec.empty())
            {
                if(dec.front() == S[it].second)
                {
                    dec.pop();
                    S[it].first--;
                }
                it++;
            }
            std::sort(S.begin(), S.end(), [](std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b){return a.first<b.first;});
        }
        else
        {
            a.push_back(j);
            b.push_back(j+1);
            le.push_back(S[0].second);
            le.push_back(S[1].second);
            li[S[0].second] = i;
            li[S[1].second] = i;

            for(std::size_t ii = 2; ii < S.size(); ii++)
            {
                if(lte(S[0].second, S[ii].second))
                {
                    dec.push(ii);
                }
            }
            for(std::size_t ii = 2; ii < S.size(); ii++)
            {
                if(lte(S[1].second, S[ii].second))
                {
                    dec2.push(ii);
                }
            }

            i++;
            j += 2;
            S.erase(S.begin(), S.begin()+2);

            if (S.size()>0)
            {
                std::sort(S.begin(), S.end(), [](std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b){return a.second<b.second;});
                std::size_t it = 0;
                while(!dec.empty())
                {
                    while(dec.front()<S[it].second)
                    {

                    }
                    if(dec.front() == S[it].second)
                    {
                        dec.pop();
                        S[it].first--;
                    }
                    it++;
                }
                it = 0;
                while(!dec2.empty())
                {
                    if(dec2.front() == S[it].second)
                    {
                        dec2.pop();
                        S[it].first--;
                    }
                    it++;
                }

                std::sort(S.begin(), S.end(), [](std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b){return a.first<b.first;});
            }

        }
    }
    maxPair = i;
}


void Poset::generateLinearExtension(std::queue<std::vector<std::size_t>> &q)
{
    preprocess();
    generateLinearExtension(maxPair, q);
    signSwitch(maxPair-1);
    generateLinearExtension(maxPair, q);
}


/*
 * This is a simple implementation of an algorithm by Gara Pruesse and Frank Ruskey
 * from the paper "Generating Linear Extensions Fast"
 */
void Poset::generateLinearExtension(std::size_t i, std::queue<std::vector<std::size_t>>& q)
{
if(isPlus)
    q.push(std::vector<std::size_t>(le));

std::size_t mrb, mra, mla;
bool typical;
if(i > 0)
{
    i--;
    generateLinearExtension(i, q);
    mrb = 0;
    typical = false;
    while(rightB(i))
    {
        mrb++;
        moveB(i, rightdir);
        generateLinearExtension(i, q);
        mra = 0;
        if(rightA(i))
        {
            typical = true;
            do {
                mra++;
                moveA(i, rightdir);
                generateLinearExtension(i, q);
            }
            while (rightA(i));
        }
        if(typical)
        {
            signSwitch(i);
            generateLinearExtension(i, q);
            if(mrb % 2 == 1)
            {
                mla = mra - 1;
            }
            else
            {
                mla = mra + 1;
            }
            for(std::size_t j=1; j<=mla; j++)
            {
                moveA(i, leftdir);
                generateLinearExtension(i, q);
            }
        }
    }
    if (typical && (mrb % 2 == 1))
    {
        moveA(i, leftdir);
    }
    else
    {
        signSwitch(i);
    }
    generateLinearExtension(i, q);
    for(std::size_t j=1; j<=mrb; j++)
    {
        moveB(i, leftdir);
        generateLinearExtension(i, q);
    }
    ++i;
}

}
