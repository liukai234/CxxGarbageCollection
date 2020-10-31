//
// Created by lk234 on 2020/10/22 022.
//

#include "MemoryManager.h"
#include "Object.h"

/*
 * type_pointer.c 内存地址输出工具
 */
#include "type_pointer.c"

// MemoryManager 直接申请一大块内存
// 通过内部new重载分配给Object

MemoryManager * MemoryManager::memoryManager = nullptr;
//std::list<void *> MemoryManager::objectCount;

size_t MemoryManager::lastPointer_ = 0;

size_t MemoryManager::memorySize_;
void * MemoryManager::Memory_;
using namespace std;
MemoryStruct MemoryManager::memoryStruct_;

/*
 * 重写全局operator new
 */
void* operator new(size_t size){
#ifdef DEBUG
    std::cout << "global new\n";
#endif
    return malloc(size);
}

/*
 * 重写全局operator delete
 */
void operator delete(void *pointer) noexcept {
#ifdef DEBUG
    std::cout << "global delete\n";
#endif
    free(pointer);
}

/*
 * 使用memoryManager前初始化任务
 * 初始化MemoryManager中的内存大小memorySize_，为Memory_分配内存空间
 * 初始化对象管理列表数据结构
 */
void MemoryManager::init() {
    memorySize_ = MEMORY_SIZE;
    Memory_ = ::operator new(memorySize_);

    for(std::size_t i = 0; i < MAX_OBJECT; ++i) {
        memoryStruct_.list[i] = nullptr;
        memoryStruct_.listPreSize[i] = 0;
    }
    memoryStruct_.listIndex = 0;
}

/*
 * MemoryManager类内重写operator new
 */
void* MemoryManager::operator new(size_t size) // 重写operator new , Object &obj
{
#ifdef DEBUG
    dbg("MemoryManager operator new called");
#endif

    // TODO 增加内存分配不足时的异常处理
    if(size > memorySize_ - lastPointer_) {
        std::cout << "Cannot Allocation\n"; // 应抛出异常
    }

//    show_bytes((byte_pointer) Memory_, sizeof())

    /*
     * Memory_起始地址 + 偏移量
     */
    void * objPointer = (byte_pointer)Memory_ + lastPointer_;
    lastPointer_ += size;

    memoryStruct_.list[memoryStruct_.listIndex] = (byte_pointer)objPointer;

    // TODO: 内存越界检查
    memoryStruct_.listPreSize[memoryStruct_.listIndex] = size;
    memoryStruct_.listIndex += 1;

//    objectCount.push_back(objPointer);
    return objPointer;
}
/*
 * MemoryManager类内重写operator delete
 * 释放后对指针置nullptr
 */
void MemoryManager::operator delete(void *pointer) noexcept {
#ifdef DEBUG
    std::cout << "MemoryManager operator delete called\n";
#endif

//    for(std::size_t i = 0; i < MAX_OBJECT; ++i) {
//
//    }
    ::operator delete(pointer);
    pointer = nullptr;
}

/*
 * 获得MemoryManager单例
 */
MemoryManager* MemoryManager::getInstance() {
    if(memoryManager == nullptr) {

        // MemoryManager(MEMORY_SIZE);
        memoryManager = static_cast<MemoryManager *>(::operator new(sizeof(MemoryManager)));

        /*
         * void MemoryManager::init()
         * 初始化
         */
        init();
    }
    return memoryManager;
}


/*
 * Mark - Clear
 * Mark - Compress
 * Mark 即管理列表中的内存地址为空（手动delete）
 * TODO: 下一个版本应该增加自动Mark的功能
 */

// static void markClear

// TODO: clearALL、move整理
// TODO: 线程暂停，处理全部 使用一个临时链表来倒换 objectCount 内容有序弹出并判断是否被delete正在占用资源给temp链表，然后再从temp表给回objectCount
// TODO: 寻找空闲地址，标记 size
//void MemoryManager::getAllObjPointer() {
//    // 逆序遍历
//    // using listIterType = decltype(objectCount.rbegin());
//    std::list<void *> objectListTemp;
//
//    for(auto iter = objectCount.rbegin(); iter != objectCount.rend(); ++ iter) {
//        auto *objTemp = (Object*)*iter; // Object *
//        int refCount = objTemp->refCount();
//        std::cout << refCount << (objTemp->objectName()) << std::endl;
//        if(0 == refCount) {
//            std::cout << "Above has be deleted" << std::endl;
//            delete objTemp;
//            objTemp = nullptr;
//        } else {
//            objectListTemp.push_front(*iter);
//        }
//    }
//
//    objectCount.clear();
//
//    std::cout << "---" << std::endl;
//    for(auto &x : objectListTemp) {
//        std::cout << ((Object*)x)->refCount() << std::endl;
//        objectCount.push_back(x);
//    }
//
//    objectListTemp.clear();
//}

//MemoryManager::MemoryManager{
//
//    std::cout << "MemoryManager< std::endl;
//    if(memoryManager) {
//        delete MemoryManager::memoryManager;
//        MemoryManager::memoryManager = nullptr;
//    }
//}


