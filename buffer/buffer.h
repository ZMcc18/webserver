#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>  
#include <iostream>
#include <unistd.h>  
#include <sys/uio.h> 
#include <vector> 
#include <atomic>
#include <assert.h>

class Buffer {
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default; 

    size_t WritableBytes() const; //可写的字节数       这些方法分别用于查询缓冲区中可写、可读和可预填充的字节数。
    size_t ReadableBytes() const ; //可读的字节数 
    size_t PrependableBytes() const; //前面可以用的空间

    const char* Peek() const;   //返回一个指向缓冲区开头的常量指针，这样就可以查看缓冲区的内容但无法修改。
    void EnsureWriteable(size_t len);    //确保缓冲区有足够的空间写入给定长度的数据。
    void HasWritten(size_t len);  //更新写位置，一般在写入数据后调用。

    void Retrieve(size_t len);     //这些方法用于从缓冲区取走数据，更新读位置。
    void RetrieveUntil(const char* end);

    void RetrieveAll() ; 

    const char* BeginWriteConst() const;   //返回写位置的常量指针或指针，用于向缓冲区写数据。
    char* BeginWrite();

    void Append(const std::string& str);   //向缓冲区中追加数据。
    void Append(const char* str, size_t len);

    ssize_t ReadFd(int fd, int* Errno);     //从文件描述符读取数据到缓冲区。
    //ssize_t WriteFd(int fd, int* Errno);

private:
    char* BeginPtr_();       //私有方法，返回buffer_的起始位置的指针，后者为常量版本。
    const char* BeginPtr_() const;   //返回buffer_的起始位置的指针，后者为常量版本。
    void MakeSpace_(size_t len); //创建足够的空间以接纳给定长度的数据。

    std::vector<char> buffer_; //具体装数据的vector   实际的缓冲区，使用std::vector<char>存储数据。
    std::atomic<std::size_t> readPos_;  //使用std::atomic表示的读写位置，保证了在多线程环境下的安全性。
    std::atomic<std::size_t> writePos_; 
};

#endif //BUFFER_H
