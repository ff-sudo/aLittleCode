#ifndef ARERR_H
#define ARERR_H

#include <iostream>
#include <cstring>

// 定义错误码枚举
enum ErrorCode {
    SUCCESS = 0,            // 成功
    FAILURE = -1,           // 失败
    INVALID_INPUT = -2,     // 无效输入
    OUT_OF_MEMORY = -3,     // 内存不足
    FILE_NOT_FOUND = -4,    // 文件未找到
    PERMISSION_DENIED = -5, // 权限被拒绝
    TIMEOUT = -6,           // 超时
    CONNECTION_LOST = -7,   // 连接丢失
    UNKNOWN_ERROR = -99     // 未知错误
};

// 定义一个函数来打印错误信息
const char* getErrorString(ErrorCode code) {
    switch (code) {
        case SUCCESS: return "成功";
        case FAILURE: return "失败";
        case INVALID_INPUT: return "无效输入";
        case OUT_OF_MEMORY: return "内存不足";
        // case FILE_NOT_FOUND: return "文件未找到";
        // case PERMISSION_DENIED: return "权限被拒绝";
        // case TIMEOUT: return "超时";
        // case CONNECTION_LOST: return "连接丢失";
        // case UNKNOWN_ERROR: return "未知错误";
        // default: return "未知错误码";
    }
}

// 如果需要在多线程环境中使用，可以考虑使用线程安全的实现
void printError(ErrorCode code) {
    std::cerr << "错误: " << getErrorString(code) << " (" << code << ")" << std::endl;
}



#endif