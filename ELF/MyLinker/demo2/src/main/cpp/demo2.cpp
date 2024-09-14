#include <jni.h>
#include <string>
#include <android/log.h>
#include "../../../../util/util.h"

using namespace std;
extern "C" JNIEXPORT jstring JNICALL
Java_org_samael_demo2_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    LOGD("Java_org_samael_demo2_NativeLib_stringFromJNI native log called");
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_org_samael_mylinker_MainActivity_demo2fun1(JNIEnv *env, jobject thiz) {
    // TODO: implement demo2fun1()
    LOGD("Java_org_samael_mylinker_MainActivity_demo2fun1 native log called");

    string  str = "Java_org_samael_mylinker_MainActivity_demo2fun1 string str";

    return env->NewStringUTF(str.c_str());
}

__attribute__((constructor())) void sayHello(){
    LOGD("[from libdemo2.so .init_array]  Hello called");
}

extern  "C" {
    void _init(void){
        LOGD("[from libdemo2.so .init] _init called");
    }
}