/*
 * g++ main.cpp -o main.exe -D DEBUG -std=c++14
 * for(auto &x : inputStaff) { std::cout << *x; }
 */

#include <iostream>
#include <list>
#include "MemoryManager.h"
#include "Object.h"
#include <string.h>

#ifdef DEBUG
#include "dbg.h"
#endif


class Add : public Object {
public:
    explicit Add(int a, std::string name) : a_ {a} {
        setObjectName(name);
    }
    explicit Add() {}
    int a() const { return a_; }
    
private:
    int a_;
};

class B : public Add {
public:
    explicit B(int a, std::string name) : a_ {a} {
        setObjectName(name);
    }
private:
    int a_;
};

int main() {

    Add *add1 = new Add(5, "add1");
    delete(add1);
    Add *add2 = new Add(2, "add2");
    B *b = new B(5, "b");
    B *b2 = new B(2, "b2");

    *b2 = *b;
    *add2 = *add1;
//    MemoryManager::getAllObjPointer();

    b->refCount();
    add1->refCount();

#ifdef DEBUG
    dbg(add1->refCount());
    dbg(add2->refCount());
#endif
//    if(add1->refCount() == 1) delete add1; // 引用为0时释放资源
//    if(add2->refCount() == 0) delete add2; // 程序结束前释放所有资源
    return 0;
}
