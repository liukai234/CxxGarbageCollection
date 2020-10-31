//
// Created by lk234 on 2020/10/22 022.
//

// 直接先申请一块空间 或者 申请俩块一块申请，一块拷贝
// Object C llvm   编译优化自动增加 delete
//

#include "Object.h"
void * Object::operator new(size_t size) {
#ifdef DEBUG
    dbg("operator new called");
#endif
    return MemoryManager::getInstance()->operator new(size); // 调用ManageAllocate::operator new
}

void Object::operator delete(void *pointer) noexcept {
#ifdef DEBUG
    dbg("operator delete called");
#endif
    MemoryManager::getInstance()->operator delete(pointer);
}

// TODO: [PROBLEM] deep copy 深拷贝
Object& Object::operator=(Object &obj) {
#ifdef DEBUG
    dbg("operator= &obj");
#endif
    // TODO: 对象被释放时减计数
    // 赋值时增减计数 ? 减 怎么判断减
    // 在ManageAllocation中判断对象不被引用  不被引用?   c=a=b 是否可以说b失去了价值，程序可能会继续使用b
    // 或者来这么理解  a=b b.refCount() = 1   a.refCount = -1;  Error
    // 判断对象的生命周期结束 使用ManageAllocatio进行回收，但是当对象减少了一个引用  回收时利用ManageAllcation判断其refCount为0

    obj.refCount_ += 1;
    return *this;
}

int Object::refCount() const {
    return refCount_;
}

std::string Object::objectName() const{
    return objectName_;
}