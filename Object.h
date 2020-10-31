//
// Created by lk234 on 2020/10/22 022.
//
#include <iostream>
#ifndef GARBAGECOLLECTION_OBJECT_H
#define GARBAGECOLLECTION_OBJECT_H
#include "MemoryManager.h"
#include <string>
#ifdef DEBUG
#include "dbg.h"
#endif
class Object {
public:
    void* operator new(size_t size); // 重写operator new
    void operator delete(void *pointer) noexcept;
    Object& operator=(Object &obj);
    int refCount() const;
    void setObjectName(std::string str) {
        objectName_ = str;
    }
    std::string objectName() const;
    
//    ~Object();
private:
//    static MemoryManager* memoryManager;
    int refCount_ = 0;
    std::string objectName_;
protected:

};

#endif //GARBAGECOLLECTION_OBJECT_H
