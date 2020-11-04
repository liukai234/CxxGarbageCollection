//
// Created by lk234 on 2020/11/2 002.
//
#include <new>

#ifndef GARBAGE_COLLECTION_MEMORY_BADALLOC_H
#define GARBAGE_COLLECTION_MEMORY_BADALLOC_H


class MemoryBadAlloc : public std::bad_alloc{
public:
    const char* what() const noexcept override;
};


#endif //GARBAGE_COLLECTION_MEMORY_BADALLOC_H
