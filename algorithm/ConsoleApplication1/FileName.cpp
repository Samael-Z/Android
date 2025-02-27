/* Round 2 */
GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
//MGG(a, b, c, d, x[1], S21, 0xf61e2562); /* M17 */
GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

/* Round 3 */
HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
//MHH(a, b, c, d, x[5], S31, 0xfffa3942); /* M33 */
HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

/* Round 4 */
II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */