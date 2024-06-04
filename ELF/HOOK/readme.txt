此项目实现了对malloc的hook，可实现对plt表，got表，libc符号表中的mallocHOOK。
直接调用malloc函数HOOK		成功
函数指针调用malloc函数HOOK		成功
动态调用malloc函数HOOK		成功

测试环境位android4.4  x86.其他环境使用应该会失败，需要调整下代码。

部分代码和思路参考了xhook和guoshan-yang的
https://github.com/iqiyi/xHook.git
https://github.com/guoshan-yang/GotHook.git