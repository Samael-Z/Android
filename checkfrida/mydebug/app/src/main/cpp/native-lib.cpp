#include <jni.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>

extern "C" JNIEXPORT jstring JNICALL
Java_org_samle_mydebug_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_org_samle_mydebug_MainActivity_killProcess(JNIEnv *env, jobject thiz, jint pid) {
    // TODO: implement killProcess()

    int ret = kill(pid,SIGKILL);

    return ret;
}
extern "C"
JNIEXPORT jint JNICALL
Java_org_samle_mydebug_MainActivity_checkdbus(JNIEnv *env, jobject thiz) {
    // TODO: implement checkdbus()


    //定义1个socket地址结构体变量

    sockaddr_in sa;
    int sock;
    char res[7];
    //循环遍历所有可能的端口号，从0-65535
    for (int i = 0; i < 65535; ++i) {
        sock = socket(AF_INET,SOCK_STREAM,0);
        sa.sin_port = htons(i);
        if (connect(sock , (sockaddr*)&sa,sizeof(sa)) != -1){
            //如果链接成功，记录日志信息，表示发现了一个开放的端口
            memset(res,0,7);
            //向sock发送一个空的字节
            send(sock,"\x00",1,0);
            //发送AUTH请求
            send(sock,"AUTH\r\n",6,0);
            //等待100微妙
            usleep(100);
            //尝试接收回应
            if (recv(sock,res,6,MSG_DONTWAIT) != -1){
                //如果接收到接收到响应，检查相应内容是否为“REJECT”
                if (strcmp(res,"REJECT") == 0){
                    //如果是，关闭socket并返回1，表示检测到了frida服务器
                    close(sock);
                    return 1;
                    //pixel4 android10真机实践貌似没作用，frida16.4.18。connect链接失败，原因未知
                }
            }
        }

    }
    close(sock);
    return 0;
}
//不好使
extern "C"
JNIEXPORT jint JNICALL
Java_org_samle_mydebug_MainActivity_checkfd(JNIEnv *env, jobject thiz) {
    // TODO: implement checkfd()


    DIR *dir = NULL;
    struct dirent *entry;
    char link_name[100];
    char buf[100];
    int ret = 0;
    if ((dir = opendir("/proc/self/fd/")) == NULL) {
       // LOGI(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
    } else {
        entry = readdir(dir);
        while (entry) {
            switch (entry->d_type) {
                case DT_LNK:
                    sprintf(link_name, "%s/%s", "/proc/self/fd/", entry->d_name);
                    readlink(link_name, buf, sizeof(buf));
                    if (strstr(buf, "frida") || strstr(buf, "gum-js-loop") ||
                        strstr(buf, "gmain") ||
                        strstr(buf, "-gadget") || strstr(buf, "linjector")) {
                       // LOGI("check_fd -> find frida:%s", buf);
                        ret = 1;
                    }
                    break;
                default:
                    break;
            }
            entry = readdir(dir);
        }
    }
    closedir(dir);
    return ret;



}
//好使
extern "C"
JNIEXPORT jint JNICALL
Java_org_samle_mydebug_MainActivity_checkmaps(JNIEnv *env, jobject thiz) {
    // TODO: implement checkmaps()

    // 定义一个足够大的字符数组line，用于存储读取的行
    char line[512];
    // 打开当前进程的内存映射文件/proc/self/maps进行读取
    FILE* fp = fopen("/proc/self/maps", "r");
    if (fp) {
        // 如果文件成功打开，循环读取每一行
        while (fgets(line, sizeof(line), fp)) {
            // 使用strstr函数检查当前行是否包含"frida"字符串
            if (strstr(line, "frida") || strstr(line, "gadget")) {
                // 如果找到了"frida"，关闭文件并返回true，表示检测到了恶意库
                fclose(fp);
                return 1; // Evil library is loaded.
            }
        }
        // 遍历完文件后，关闭文件
        fclose(fp);
    } else {
        // 如果无法打开文件，记录错误。这可能意味着系统状态异常
        // 注意：这里的代码没有处理错误，只是注释说明了可能的情况
    }
    // 如果没有在内存映射文件中找到"frida"，返回false，表示没有检测到恶意库
    return 0; // No evil library detected.


}