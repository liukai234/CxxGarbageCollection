//
// Created by lk234 on 2020/10/22 022.
//
#include <iostream>
#ifndef GARBAGE_COLLECTION_OBJECT_H
#define GARBAGE_COLLECTION_OBJECT_H
#include "MemoryManager.h"
#include <string>

class Object {
public:
    void* operator new(size_t size);
    void operator delete(void *pointer) noexcept;

    void setObjectName(std::string str) {
        objectName_ = str;
    }
    std::string objectName() const;
    virtual ~Object() = default;
private:
    std::string objectName_;
};

#endif // GARBAGE_COLLECTION_OBJECT_H
