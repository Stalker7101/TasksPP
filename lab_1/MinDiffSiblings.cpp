#include "MinDiffSiblings.hpp"

#include <pthread.h>
#include <unistd.h>
#include <climits>
#include <algorithm>
#include <iostream>


struct ThreadPart
{
    int * arr;
    int   beg;
    int   len;
    int   answIndex;
    int   answMin;
};

//----------------------------------------------------------------------------//
void* min_diff_siblings_thread(void* data)
{
    ThreadPart* part = (ThreadPart*)data;

    part->answMin = INT_MAX;
    part->answIndex = 0;

    const auto start = part->beg + 1;
    const auto end = part->beg + part->len;

    for (int i = start; i < end; ++i)
    {
        int current = std::abs(part->arr[i] - part->arr[i - 1]);
        if (current && (current < part->answMin))
        {
            part->answMin = current;
            part->answIndex = i;
        }
    }

    return nullptr;
}

//----------------------------------------------------------------------------//
int min_diff_siblings(std::vector<int>& vec, const int nThreads)
{
    std::vector<pthread_t> threadId(nThreads);
    std::vector<ThreadPart> parts(nThreads);

    const int elemsByThread = vec.size() / nThreads;
    const int remainingElems = vec.size() % nThreads;
    
    for (int i = 0; i < nThreads - 1; ++i)
    {
        parts[i].arr = vec.data();
        parts[i].beg = i * elemsByThread;
        parts[i].len = elemsByThread + 1;
    }

    parts[nThreads - 1].arr = vec.data();
    parts[nThreads - 1].beg = (nThreads - 1) * elemsByThread;
    parts[nThreads - 1].len = elemsByThread + remainingElems;

    for (int i = 0; i < nThreads; ++i)
    {
        pthread_create(
            &threadId[i],
            NULL,
            min_diff_siblings_thread,
            &parts[i]);
    }

    for (int i = 0; i < nThreads; ++i)
    {
        pthread_join(threadId[i], NULL);
    }

    int min = parts[0].answMin;
    int minIndex = parts[0].answIndex;
    for (int i = 1; i < parts.size(); ++i)
    {
        if (parts[i].answMin < min)
        {
            min = parts[i].answMin;
            minIndex = parts[i].answIndex;
        }
    }

    return minIndex;
}

// 1 2 3 4 5 6 7 8 9 10 11 12
// 1 2 3 4 5
//         5 6 7 8 9
//                 9 10 11 12
