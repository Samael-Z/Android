/*************************************************************************
  > File Name: md5.h
  > Author: tiankonguse(skyyuan)
  > Mail: i@tiankonguse.com 
  > Created Time: 2015年04月14日 20:49:19
***********************************************************************/
#ifndef MD5_H_
#define MD5_H_

/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)

 based on:

 md5.h and md5.c
 reference implementation of RFC 1321

 Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 rights reserved.

 License to copy and use this software is granted provided that it
 is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 Algorithm" in all material mentioning or referencing this software
 or this function.

 License is also granted to make and use derivative works provided
 that such works are identified as "derived from the RSA Data
 Security, Inc. MD5 Message-Digest Algorithm" in all material
 mentioning or referencing the derived work.

 RSA Data Security, Inc. makes no representations concerning either
 the merchantability of this software or the suitability of this
 software for any particular purpose. It is provided "as is"
 without express or implied warranty of any kind.

 These notices must be retained in any copies of any part of this
 documentation and/or software.

 */

#include <cstring>
#include <iostream>

// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class MD5 {
    public:
        typedef unsigned int size_type; // must be 32bit

        MD5();
        MD5(const std::string& text);
        void update(const unsigned char *buf, size_type length);
        void update(const char *buf, size_type length);
        MD5& finalize();
        std::string hexdigest() const;
        friend std::ostream& operator<<(std::ostream&, MD5 md5);

    public:
        void init();
        typedef unsigned char uint1; //  8bit
        typedef unsigned int uint4;  // 32bit
        typedef unsigned long long uint8;
        
        enum {
            blocksize = 64
        }; // VC6 won't eat a const static int here

        void transform(const uint1 block[blocksize]);
        static void decode(uint8 output[], const uint1 input[], size_type len);
        static void encode(uint1 output[], const uint8 input[], size_type len);

        bool finalized;
        uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
        uint8 count[2];   // 64bit counter for number of bits (lo, hi)
        uint8 state[4];   // digest so far
        uint1 digest[16]; // the result

        // low level logic operations
        static inline uint8 F(uint8 x, uint8 y, uint8 z);
        static inline uint8 G(uint8 x, uint8 y, uint8 z);
        static inline uint8 H(uint8 x, uint8 y, uint8 z);
        static inline uint8 I(uint8 x, uint8 y, uint8 z);
        static inline uint8 rotate_left(uint8 x, int n);
        static inline uint8 rotate_leftG(uint8 x, int n);
        static inline uint8 rotate_leftH(uint8 x, int n);
       
        static inline void FF(uint8&a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);
        static inline void GG(uint8&a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);
        static inline void HH(uint8&a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);
        static inline void II(uint8&a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);

        static inline void MGG(uint8& a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);
        static inline void MHH(uint8& a, uint8 b, uint8 c, uint8 d, uint8 x, uint8 s, uint8 ac);
};

std::string md5(const std::string str);



#endif /* MD5_H_ */
