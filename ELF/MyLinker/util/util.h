//
// Created by 69182 on 2024-09-12.
//

#include <android/log.h>

#ifndef MYLINKER_UTIL_H
#define MYLINKER_UTIL_H


#define  TAG  "zxa"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#endif //MYLINKER_UTIL_H
