#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/ptrace.h>
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,"zxa",__VA_ARGS__)

int SearchObjProcess();
void processcheck();
void processPathCheck();
void pProcessCheck();
void taskCountCheck();
void ptraceCheck();
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_antidebug02_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    LOGI("Java_com_example_antidebug02_MainActivity_stringFromJNI  hello = %s ",hello.c_str());
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM * jvm ,void * reserved){
    LOGI("JNI_OnLoad is called");
    JNIEnv  * env = NULL;
    jint  result  = -1;
    if (jvm->GetEnv((void**)&env,JNI_VERSION_1_4) != JNI_OK){
        return -1;
    }
    result  = JNI_VERSION_1_4;

    ptraceCheck();
    taskCountCheck();

    pProcessCheck();
    //processPathCheck();
    //int flag = SearchObjProcess();

    return result;
}

extern "C"  JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm ,void * reserved){

    LOGI("JNI_OnUnload is called");

}



int SearchObjProcess(){
    LOGI("SearchObjProcess is called");

    int flag = -1;
    FILE * pFile = NULL;
    char buf[0x1000] = {0};
    pFile = popen("ps","r");
    if (pFile == NULL){
        LOGI("打开命令行失败!");
        return flag;
    }
    //获取结果
    while(fgets(buf,0x1000,pFile)){

        char* strA = NULL;
        char* strB = NULL;
        char* strC = NULL;
        char* strD = NULL;

        strA = strstr(buf,"android_server");
        strB = strstr(buf,"android_server64_patch");
        strC = strstr(buf,"gdbserver");
        strD = strstr(buf,"gdb");

        if (strA){
            LOGI("find android_server");
            flag = 1;
        }
        else if (strB){
            LOGI("find android_server64_patch");
            flag = 1;
        }
        else if (strC){
            LOGI("find gdbserver");
            flag = 1;
        }
        else if (strD){
            LOGI("find gdb");
            flag = 1;
        }

    }

    pclose(pFile);
    processcheck();
    return flag;
}


void processcheck(){
    LOGI("processcheck is called");
    const int buffsize = 1024;
    char filename[buffsize] ={0};
    char filename2[buffsize] ={0};
    char line[buffsize] = {0};
    char name[buffsize] = {0};
    char nameline[buffsize] = {0};
    int pid = getpid();
    //先读取tracerpid的值
    //当tracerpid不为零是就是被调试了，android studio调试进程名称未lldb-server
    sprintf(filename,"/proc/%d/status",pid);
    FILE* fd = fopen(filename,"r");
    if (fd == NULL){
        LOGI("打开文件失败 %s!",filename);
        return ;
    }
    while(fgets(line,buffsize,fd)){

        if (strstr(line,"TracerPid") != NULL){
            int status = atoi(&line[10]);
            if (status != 0){
                sprintf(name,"/proc/%d/cmdline",status);
                FILE* fdname = fopen(name,"r");
                if(fdname !=NULL){
                    while(fgets(nameline,buffsize,fdname)){
                        if (strstr(nameline,"android_server64_patch")!=NULL){
                            int ret = kill(pid,SIGKILL);
                        }
                    }
                }

            }
        }

    }

    fclose(fd);

}
void processPathCheck(){
    LOGI("SearchObjProcess is called");
    const char* rootPath = "/data/local/tmp";
    DIR* dir = opendir(rootPath);
    //打开失败，可能是有限制未找到原因
    int pid = getpid();
    if (dir == NULL){
        LOGI("打开命令行失败!");
    }
    dirent* currentDir;
    while((currentDir = readdir(dir)) != NULL){

        //readdir()方法就像java中迭代器的next()方法一样
        //currentDir->d_name //文件名，目录名
        //currentDir->d_type 文件类型
        if (strncmp(currentDir->d_name ,"android_server64_patch",22) == 0){
            LOGI("发现调试进程android_server64_patch，准备退出进程");
            kill(pid,SIGKILL);
        }
    }
    closedir(dir);
}


void pProcessCheck(){
    LOGI("pProcessCheck is called");
   char strPPidCmdline[0x100] = {0};
   FILE* file;
   int pid = getpid();
   sprintf(strPPidCmdline,"/proc/%d/cmdline",pid);
   file = fopen(strPPidCmdline,"r");

    if(file == NULL){
        LOGI("打开文件失败!");
    }
    memset(strPPidCmdline,0,sizeof(strPPidCmdline));

    size_t ret = fread(strPPidCmdline,1,sizeof(strPPidCmdline),file);
    if (ret == -1){
        LOGI("读取文件失败!");
    }

    char * sRet = strstr(strPPidCmdline,"zygote");
    if(sRet == NULL){
       // kill(pid,SIGKILL);
        LOGI("正在被调试! cmdlineProcess = %s" , strPPidCmdline);
    } else{
        LOGI("没有被调试! cmdlineProcess = %s" , strPPidCmdline);
    }


}

void taskCountCheck(){
    LOGI("taskCountCheck is called");
    char buf[0x100] = {0};
    DIR* dir;
    int pid = getpid();
    sprintf(buf,"/proc/%d/task",pid);
    dir = opendir(buf);

    if(dir == NULL){
        LOGI("打开文件夹失败!");
    }

    //查看目录下文件个数
    struct dirent* pde= NULL;
    int count = 0;
    while (pde = readdir(dir)){

        //字符过滤
        if ((pde->d_name[0] <= '9') && (pde->d_name[0] >= '0')){
            ++count;
        }
    }


    if (1 > count){
        LOGI("正在被调试");
    }


}

void ptraceCheck(){
    LOGI("ptraceCheck is called");
   int ret =  ptrace(PTRACE_TRACEME,0,0,0);;//返回-1即为已经被调试

   LOGI("当前进程ptrace返回值 ret= %d", ret);
}

typedef uint8_t u8;
typedef uint32_t u32;
void bkptCheck(u8* addr ,u32* size){
    LOGI("bkptCheck is called");
    //结果
    u32 uRet = 0;
    //断点指令
    //u8 armBkpt[4] = {0xf0,0x01,0xf0,0xe7};
    //u8 thumbBkpt[2] = {0x10,0xde};

    u8 armBkpt[4] = {0xf0,0x01,0xf0,0xe7};
    u8 thumbBkpt[2] = {0x10,0xde};

    //判断模式
    int mode = (u32)(size_t)addr % 2;
    if (1 == mode){
        LOGI("%p 该地址为thumb模式",addr );
        u8* start = (u8*)((u32)(size_t)addr-1);
        u8* end = (u8*)((u32)(size_t)start + size);

        //遍历对比
        while(1){
            if (start > end){
                uRet = 0;
                break;
            }
            if (0 == memcmp(start ,thumbBkpt,2)){
                uRet = 1;
                LOGI("发现断点 thumb");
                return;
            }
            start +=2;
        }
    }else{
        LOGI("%p 该地址为arm模式",addr );
        u8* start=(u8*)addr;
        u8* end=(u8*)((u32)(size_t)start+size);

        while(1){
            if (start > end){
                uRet = 0;
                break;
            }
            if (0 == memcmp(start ,armBkpt,4)){
                uRet = 1;
                LOGI("发现断点 arm");
                return;
            }
            start +=4;
        }
    }



    LOGI("返回值 uRet= %d",uRet);
}