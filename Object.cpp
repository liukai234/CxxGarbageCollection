//
// Created by lk234 on 2020/10/22 022.
//
#include <thread>
#include "Object.h"
void * Object::operator new(size_t size) {
#ifdef DEBUG
    std::cout << "Object operator new called\n";
#endif
    return MemoryManager::getInstance()->operator new(size); // 调用ManageAllocate::operator new
}

void Object::operator delete(void *pointer) noexcept {
#ifdef DEBUG
    std::cout << "Object operator delete called\n";
#endif

    MemoryManager::getInstance()->operator delete(pointer);
}

std::string Object::objectName() const{
    return objectName_;
}

/*
 * 引用计数收集器[已弃用]
 */
/*
Object& Object::operator=(Object &obj) {
    obj.refCount_ += 1;
    return *this;
}

int Object::refCount() const {
    return refCount_;
}
*/

