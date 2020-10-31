//
// Created by lk234 on 2020/10/31 031.
//

#include <stdio.h>

/*
 * byte_pointer -> unsigned char *
 * 地址指针
 */
typedef unsigned char * byte_pointer;

/*
 * 内存地址输出
 * 应根据大小端机器模式读
 */
void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for(i = 0; i < len; i++) {
        printf("%.2x ", start[i]);
    }
    printf("\n");
}