#include <jni.h>
#include <string>
#include "MyLoader.h"
extern "C" JNIEXPORT jstring JNICALL
Java_org_samael_mylinker_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_org_samael_mylinker_MainActivity_test(JNIEnv *env, jobject thiz) {
    // TODO: implement test()

    MyLoader myloader;
    myloader.run("/data/local/tmp/libdemo2.so");
    //myloader.run("/data/local/tmp/libdemo2-v7a.so");  64位程序只能加载64位，否则变动太大了

}
extern "C"
JNIEXPORT void JNICALL
Java_org_samael_mylinker_MainActivity_test2(JNIEnv *env, jobject thiz) {
    // TODO: implement test2()
    LOGD("Java_org_samael_mylinker_MainActivity_test2");
}