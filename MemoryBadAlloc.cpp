//
// Created by lk234 on 2020/11/2 002.
//

#include "MemoryBadAlloc.h"

const char * MemoryBadAlloc::what() const noexcept {
    return "Fatal: does not have enough memory\n";
}