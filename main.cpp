/*
 * g++ main.cpp -o main.exe -D DEBUG -std=c++14
 */

#include "MemoryManager.h"
#include "Object.h"
#include <thread>
// #include <synchapi.h>

class A : public Object {
public:
    explicit A(int a, std::string name) : a_ {a} {
        setObjectName(name);
    }
    explicit A() {}
    int a() const { return a_; }
    
private:
    int a_;
};

class B : public A {
public:
    explicit B(int a, std::string name) : a_ {a} {
        setObjectName(name);
    }
private:
    int a_;
};


void testDriver() {
    /*
     * 当捕获到某一块内存分配失败时，终止进程，释放全部内存
     */
    try{
        A *a1 = new A(5, "a1");

        delete(a1);
        a1 = nullptr;

        A *a2 = new A(2, "a2");
        A *a3 = new A(2, "a3");
        A *a4 = new A(2, "a4");
        A *a5 = new A(2, "a5");
        A *a6 = new A(2, "a6");

        delete(a2);
        delete(a3);
        delete(a5);
        a2 = nullptr;
        a3 = nullptr;
        a5 = nullptr;

        // If don't delete Cannot be Allocated # 256/48 = 5
        B *b1 = new B(5, "b1");

        /*
         * b2未new
         * B *b2;
         */
        B *b2 = nullptr;
        /*
         * b2已new时，若要给b2赋新值，则b2直接更新，list中内容无需更改
         */
        b2 = new B(2, "b2");

        /*
         * 应在delete前使用b2 = b1
         * 在b2 = b1时就算b1被clear或Compress也只是丢失了list表中的指针，b1的数据已经被b2指向
         * 所以b1置空后并不影响b2
         */
        b2 = b1;
        delete(b1);
        b1 = nullptr;

#ifdef DEBUG
//        std::cout << "b2->objectName(): " << b2->objectName() << std::endl;
#endif
    } catch (MemoryBadAlloc &e) {
        std::cout << e.what();
    }

}

int main() {
    testDriver();
    /*
     * 释放单例
     */

    MemoryManager::deleteInstance();
    return 0;
}
