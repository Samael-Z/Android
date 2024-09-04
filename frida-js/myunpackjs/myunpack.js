

/*
OpenCommon在android 10中有两个函数，一个在dex_file_loader.cc中，另一个在art_dex_file_loader.cc中，art_dex_file_loader.cc中的OpenCommon调用的是dex_file_loader.cc的OpenCommon返回的同样是DexFile指针
名称粉碎：

art::DexFileLoader::OpenCommon(unsigned char const*, unsigned long, unsigned char const*, unsigned long, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, unsigned int, art::OatDexFile const*, bool, bool, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, std::__1::unique_ptr<art::DexFileContainer, std::__1::default_delete<art::DexFileContainer>>, art::DexFileLoader::VerifyResult*)
(从piexl 4 android 10 导出libdexfile.so objdump -T 导出符号得到的)_ZN3art13DexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE
(抄unpack的)_ZN3art13DexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE
objdump和nm中得到的导出符号都是m的，奇怪哦
art::ArtDexFileLoader::OpenCommon(unsigned char const*, unsigned long, unsigned char const*, unsigned long, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, unsigned int, art::OatDexFile const*, bool, bool, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, std::__1::unique_ptr<art::DexFileContainer, std::__1::default_delete<art::DexFileContainer>>, art::DexFileLoader::VerifyResult*)
_ZN3art16ArtDexFileLoader10OpenCommonEPKhmS2_mRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS_13DexFileLoader12VerifyResultE


std::unique_ptr<DexFile> ArtDexFileLoader::OpenCommon(const uint8_t* base,
                                                      size_t size,
                                                      const uint8_t* data_base,
                                                      size_t data_size,
                                                      const std::string& location,
                                                      uint32_t location_checksum,
                                                      const OatDexFile* oat_dex_file,
                                                      bool verify,
                                                      bool verify_checksum,
                                                      std::string* error_msg,
                                                      std::unique_ptr<DexFileContainer> container,
                                                      VerifyResult* verify_result) {
  return DexFileLoader::OpenCommon(base,
                                   size,
                                   data_base,
                                   data_size,
                                   location,
                                   location_checksum,
                                   oat_dex_file,
                                   verify,
                                   verify_checksum,
                                   error_msg,
                                   std::move(container),
                                   verify_result);
}

}  // namespace art

OpenMemory改到OpenCommon是从android 8开始的
*/
function dump_dex(){
    var libdexfileAddr = Module.getExportByName("libdexfile.so","_ZN3art13DexFileLoader10OpenCommonEPKhjS2_jRKNSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEjPKNS_10OatDexFileEbbPS9_NS3_10unique_ptrINS_16DexFileContainerENS3_14default_deleteISH_EEEEPNS0_12VerifyResultE")
    if (libdexfileAddr == undefined || libdexfileAddr == null){
        console.error("libdexfileAddr = ",libdexfileAddr)
        return;
    }
    Interceptor.attach(libdexfileAddr,{
        onEnter: function(args){
            //dex在内存中的起始位置
            var base = args[1];
            //console.log("base = " , base)
            var size = args[2];
            //console.log("size = " ,size)
            //打印magic 并验证
           // var magic1 = Memory.readUtf8String(base);
            //console.log("magic1",magic1)
            var magic = ptr(base).readCString();
            if(magic.indexOf("dex") == 0){
                //找到了dex文件

                //Dexd filesize地址
                //var address = parseInt(base,16) + 0x20
                //var dex_size = Memory.readInt(ptr(address))
                //console.log("dex_size = ", size);
                var dex_size = parseInt(size,16)
                console.log("[found dex ] base = "+ base +" size = " + dex_size )
                //dump dex到/sdcard/pkg目录下，需要确保手机sd卡下有这个文件夹，没有则手动创建
                var file = new File("/sdcard/Download/package/"  + dex_size + ".dex","wb");

                file.write(Memory.readByteArray(base,dex_size))
                file.flush();
                file.close();


            }


        },
        onLeave:function(retval){ }
    } )
}

setImmediate(dump_dex)