/*
# Author: samle
# CreateTime: 2024/09/03

*/


function get_self_package_name(){
    var openPtr = Module.getExportByName("libc.so","open");
    var open = new NativeFunction(openPtr,'int',['pointer','int']);

    var readPtr = Module.getExportByName("libc.so","read");
    var read = new NativeFunction(readPtr,'int',['int','pointer','int']);

    var closePtr = Module.getExportByName("libc.so","close");
    var close = new NativeFunction(closePtr,'int',['int']);
    //申请字符串空间，声明字符串
    var path = Memory.allocUtf8String("/proc/self/cmdline");
    /*
    cmdline内容：
    6F 72 67 2E 73 61 6D 6C 65 2E 74 65 73 74 64 65                 org.samle.testdex
    78 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00
    */
    var fd = open(path,0);
    if(fd != -1){
        var buffer = Memory.alloc(0x1000);
        var result = read(fd,buffer,0x1000);
        close(fd);
        result = ptr(buffer).readCString();      //读取c风格字符串，会被/0截断字符串，从而拿到包名
        return result;
    }
    return -1;

}

  
function mkdir(path){

    var mkdirPtr = Module.getExportByName("libc.so","mkdir");
    var mkdirFun = new NativeFunction(mkdirPtr,'int',['pointer','int']);

    var opendirPtr = Module.getExportByName("libc.so",'opendir');
    var opendir = new NativeFunction(opendirPtr,'pointer',['pointer']);

    var closedirPtr = Module.getExportByName('libc.so','closedir');
    var closedir = new NativeFunction(closedirPtr,'int',['pointer']);

    var cPath = Memory.allocUtf8String(path);
    var dir = opendir(cPath);
    //打开指定路径文件夹失败就退出
    if(dir != 0){
        closedir(dir);
        return 0;
    }
    //创建文件夹
    mkdirFun(cPath,755);
    //修改权限
    chmod(path);
    //已经是755权限了，干嘛还是调用chmod，？？？
}
function chmod(path){
    var chmodPtr = Module.getExportByName('libc.so','chmod');
    var chmodFun = new NativeFunction(chmodPtr,'int',['pointer','int']);
    var cPath = Memory.allocUtf8String(path);
    chmodFun(cPath,755);


}
function dump_dex(){

    console.log("dump_dex");
    //查找libart.so的模块基址
    var libart = Process.findModuleByName("libart.so");
    var addr_DefineClass  = null;
    //枚举函数符号，查找defineclass的函数签名
    var symbols = libart.enumerateSymbols();
    for(var index = 0; index < symbols.length; index++){
        var symbol = symbols[index];
        var symbol_name = symbol.name;
        //这个DefineClass的函数签名是android 9的
        //_ZN3art11ClassLinker11DefineClassEPNS_6ThreadEPKcmNS_6HandleINS_6mirror11ClassLoaderEEERKNS_7DexFileERKNS9_8ClassDefE

        //android 10的DefineClass函数签名
        //_ZN3art11ClassLinker11DefineClassEPNS_6ThreadEPKcmNS_6HandleINS_6mirror11ClassLoaderEEERKNS_7DexFileERKNS_3dex8ClassDefE
        /*mirror::Class* ClassLinker::DefineClass(Thread* self,
                                        const char* descriptor,
                                        size_t hash,
                                        Handle<mirror::ClassLoader> class_loader,
                                        const DexFile& dex_file,
                                        const DexFile::ClassDef& dex_class_def) 
        可以看到DefineClass的参数有一个dexfile对象                                                      
                                        */
        if( symbol_name.indexOf("ClassLinker") >= 0 && 
            symbol_name.indexOf("DefineClass") >= 0 &&
            symbol_name.indexOf("Thread") >= 0 &&
            symbol_name.indexOf("DexFile") >= 0){

                console.log(symbol_name,symbol.address);
                addr_DefineClass = symbol.address;
            }
        //console.log("libart.so symbols_name",symbol_name)
    }
    
    if(addr_DefineClass == null){
        console.log("没有找到defineClass函数符号信息，退出!");
        return;
    }

    var dex_maps = { };
    var dex_count = 1;
    console.log("[DefineClass:]",addr_DefineClass);
    if(addr_DefineClass){
        Interceptor.attach(addr_DefineClass,{
            onEnter: function(args) {
    
                var dex_file = args[5];
                //console.log(Process.pointerSize) = 4
                //var magic = dex_file.readByteArray(10)
                //var dex_file_addr = ptr(dex_file)
                //console.log("dex_file_addr ",dex_file_addr);

                //var addr_P = ptr(dex_file).add(Process.pointerSize)
                //console.log("addr_P ",addr_P);

                //var addr_PP = ptr(dex_file).add(Process.pointerSize + Process.pointerSize)
                //console.log("addr_PP ",addr_PP);

                var base = ptr(dex_file).add(Process.pointerSize).readPointer();
                var size = ptr(dex_file).add(Process.pointerSize + Process.pointerSize).readUInt();
                //console.log("magic  " ,Memory.readUtf8String(base)) 读出”dex换行035“

                //console.log("base " , base);
                //console.log("size ", size)
                //var size = 0x60; // 要读取的字节长度
                //var bytes = Memory.readByteArray(base, size);
                //console.log(hexdump(bytes, {
                //offset: 0,
                //length: bytes.byteLength,
                //header: true,
                //ansi: true
                //}));

                //ptr(dex_file).add(Process.pointerSize) is "const uint8_t* const begin_;"
                //ptr(dex_file).add(Process.pointerSize + Process.poniterSize) is "const size_t size_ "

                //Short-hand for `new NativePointer(value)`.
                //将这个地址解析一个native指针
            /*
            DexFile::DexFile(const uint8_t* base,
                            size_t size,    dex文件长度
                            const uint8_t* data_begin,
                            size_t data_size,
                            const std::string& location,
                            uint32_t location_checksum,
                            const OatDexFile* oat_dex_file,
                            std::unique_ptr<DexFileContainer> container,
                            bool is_compact_dex)
           
            */
                //dex_maps是一个空对象，被size赋值后变成了Object {1193046: 26505}，避免再次进入后重复dump，过滤器的作用
                if(dex_maps[base] == undefined){
                    dex_maps[base] = size;
                    var magic = ptr(base).readCString();
                    if(magic.indexOf("dex") == 0){
                        var package_name  = get_self_package_name();
                        if(package_name != "-1"){
                            var dex_dir_path = "/data/data/" + package_name + "/files/dump_dex_" + package_name;
                            mkdir(dex_dir_path);
                            var dex_path = dex_dir_path + "/class" + (dex_count == 1 ? "" : dex_count) + ".dex";
                            //console.log("[find dex]: ", dex_path);
                            var fd = new File(dex_path, "wb");
                            if(fd && fd != null){
                                dex_count++;
                                var dex_buffer = ptr(base).readByteArray(size);
                                fd.write(dex_buffer);
                                fd.flush();
                                fd.close();
                                console.log("[dump dex]: ",dex_path);
    
                            }
                        }
                        
                    }
                }
    
            },
            onLeave:function(retval) {}
        });
    }
    
}




setImmediate(dump_dex)

/*
总结以下dump_dex函数的流程

    1. 获取libart.so的地址，枚举符号找到DefineClass的函数地址
    2. 使用Interceptor内联hook DefineClass函数，获取dexFile对象的base和size
    3. 在包名的files目录下创建一个dump_dex_包名的文件夹，从dexfile的base读取size个字节写入文件


*/