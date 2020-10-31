### C++ 跟踪收集器🌈
1. 所有子类都应继承Object基类
2. MemoryManager::memoryTrim()定时使主线程暂停，并进行内存整理
---
### 类
1. class MemeoryManager 单例模式 实现对内存空间的管理
2. class Object 对象基类
---
### 回收算法
1. MemoryManager::toMark() 标记
2. MemoryManager::markClear() 清除
3. MemeoryManager::Compress() 压缩
