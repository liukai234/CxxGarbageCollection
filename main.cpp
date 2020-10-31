/*
 * g++ main.cpp -o main.exe -D DEBUG -std=c++14
 * for(auto &x : inputStaff) { std::cout << *x; }
 */

#include <iostream>
#include <list>
#include "MemoryManager.h"
#include "Object.h"
#include <string.h>
#include <thread>

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

int main() {

    A *a1 = new A(5, "a1");

//    std::thread t(MemoryManager::markClear);
//    t.join();
    delete(a1);
    a1 = nullptr;
    MemoryManager::markClear();
    A *a2 = new A(2, "a2");

    B *b1 = new B(5, "b1");
    B *b2 = new B(2, "b2");
    delete(b1);
    b1 = nullptr;
    MemoryManager::markClear();

//    *b2 = *b1;
//    *a2 = *a1;
////    MemoryManager::getAllObjPointer();
//
//    b1->refCount();
//    a1->refCount();



//    if(a1->refCount() == 1) delete a1; // 引用为0时释放资源
//    if(a2->refCount() == 0) delete a2; // 程序结束前释放所有资源
    return 0;
}
