//
// Created by lk234 on 2020/10/22 022.
//

#include <thread>
#include <synchapi.h>
#include "MemoryManager.h"

/*
 * type_pointer.c 内存地址输出工具
 */
#include "type_pointer.c"

MemoryManager * MemoryManager::memoryManager = nullptr;

void * MemoryManager::Memory_;
size_t MemoryManager::memorySize_;
size_t MemoryManager::lastPointer_ = 0;

int MemoryManager::THREAD_KILL = 0;
std::thread* MemoryManager::memoryTrimThread = nullptr;

int MemoryManager::LOCK_ = 0;

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
        memoryStruct_.mark[i] = false;
    }
    memoryStruct_.listIndex = 0;

    /*
     * 初始化时开启内存整理线程
     */
    memoryTrimThread = new std::thread(memoryTrim);
#ifdef DEBUG
    std::cout << "thread start\n";
#endif

}

/*
 * MemoryManager类内重写operator new
 */
void* MemoryManager::operator new(size_t size) // 重写operator new , Object &obj
{
    while(true) {
        if(!LOCK_) {
            LOCK_ = 1;
            /*
             * 内存不足时异常处理
             */
            if (size > memorySize_ - lastPointer_) {
                throw MemoryBadAlloc();
            }

            /*
             * Memory_起始地址 + 偏移量
             */
            void *objPointer = (byte_pointer) Memory_ + lastPointer_;
#ifdef POINTER_DEBUG
            std::cout << "MemoryManager operator new called, new: ";
            show_pointer(objPointer);
#endif
            lastPointer_ += size;

            memoryStruct_.list[memoryStruct_.listIndex] = (byte_pointer) objPointer;

            // TODO: 内存越界检查
            memoryStruct_.listPreSize[memoryStruct_.listIndex] = size;
            memoryStruct_.listIndex += 1;
            LOCK_ = 0;
            return objPointer;
        }
    }
}

/*
 * MemoryManager类内重载operator delete
 * toMark() 标记
 */
void MemoryManager::operator delete(void *pointer) noexcept {
#ifdef POINTER_DEBUG
    std::cout << "MemoryManager operator delete called, delete: ";
    show_pointer(pointer);

#endif
    toMark(pointer);
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
 * 整理时让new的自旋锁自旋
 */
void MemoryManager::memoryTrim() {
    while(true) {
#ifdef DEBUG
        //        std::cout << "thread pause by memoryTrim()\n";
#endif
        // 为互斥资源上锁
        if(!LOCK_) {
            LOCK_ = 1;
            markClear();
            markCompress();

            /*
             * Sleep(300)不应该存在，目的在于演示LOCK_让new中的自旋锁自旋
             */
            Sleep(300);

            LOCK_ = 0;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(300));
        if(THREAD_KILL) break;
    }
}

/*
 * 增加标记
 */
void MemoryManager::toMark(void *pointer) {
    for(std::size_t i = 0; i < MAX_OBJECT; ++i) {
        if(pointer == memoryStruct_.list[i]) {
            memoryStruct_.mark[i] = true;
        }
    }
}

/*
 * Mark - Clear
 * Mark - Compress
 */
void MemoryManager::markClear() {
    for(std::size_t i = 0; i < MAX_OBJECT; ++i) {
        if(memoryStruct_.mark[i]) {
             /*
              * ::operator delete(memoryStruct_.list[i]);
              * 这里无需调用全局delete, 因为这块内存是Memory_所拥有的, free时释放了Memory_的内存
              */
             memoryStruct_.list[i] = nullptr;
             memoryStruct_.mark[i] = false;

             /*
              * 每次释放资源后listIndex减一
              */
              memoryStruct_.listIndex -= 1;
              lastPointer_ -= memoryStruct_.listPreSize[i];
              memoryStruct_.listPreSize[i] = 0;
        }
    }
}


void MemoryManager::markCompress() {
    /*
     * front nullptr index
     * tail not nullptr index
     */
    size_t front = 0;
    size_t tail = 0;
    while(front < MAX_OBJECT && tail < MAX_OBJECT){
        while(front < MAX_OBJECT) {
            if(memoryStruct_.list[front] != nullptr) { ++ front; }
            else break;
        }
        while(tail < MAX_OBJECT) {
            if(memoryStruct_.list[tail] == nullptr) { ++ tail; }
            else break;
        }

        /*
         * 当整个内存剩余全为空或全满时结束
         */
        if(tail >= MAX_OBJECT && front >= MAX_OBJECT) { break; }

        if(front < tail && front < MAX_OBJECT && tail < MAX_OBJECT) {
//            memoryStruct_.listIndex -= 1;
//            lastPointer_ -= memoryStruct_.listPreSize[tail];

            swap(front, tail);

            ++ front;
            ++ tail;
        } else {
            ++ tail;
        }
    }
}

void MemoryManager::swap(size_t front, size_t tail) {
    memoryStruct_.list[front] = memoryStruct_.list[tail];
    memoryStruct_.list[tail] = nullptr;

    memoryStruct_.listPreSize[front] = memoryStruct_.listPreSize[tail];
    memoryStruct_.listPreSize[tail] = 0;
}

/*
 * 单例模式中析构函数会成环
 */
//MemoryManager::~MemoryManager() {
//    std::cout << "MemoryManager\n";
//    if(Memory_) {
//        ::operator delete(Memory_);
//        Memory_ = nullptr;
//    }
//}

void MemoryManager::deleteInstance(){
#ifdef DEBUG
    std::cout << "~MemoryManager\n";
#endif
    if(Memory_) {
        ::operator delete(Memory_);
        Memory_ = nullptr;
    }

    if(memoryManager) {
        ::operator delete(memoryManager);
        memoryManager = nullptr;
    }

    /*
     * 将memoryTrim的循环结束
     */
    THREAD_KILL = 1;
    memoryTrimThread->join();
}


