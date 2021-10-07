#include<stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEYBYTES (16)   //128位初始密钥
#define SPNBYTES (8)    //64位SPN
#define INPUTBYTES (1<<24)  // 2^27位明文
#define NBLOCK (1<<21)
typedef unsigned long long ULL;    // 64位
using namespace std;

ULL arrSBox[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
int arrPPos[64] = {0,16,32,48,1,17,33,49,2,18,34,50,3,19,35,51,4,20,36,52,5,21,37,53,6,22,38,54,7,23,39,55,8,24,40,56,9,25,41,57,10,26,42,58,11,27,43,59,12,28,44,60,13,29,45,61,14,30,46,62,15,31,47,63};
ULL arrPBox[64] = {0};
void genKey(ULL* arrKey)
{
    arrKey[1] = ((arrKey[0] << 16) & 0xffffffffffff0000) ^ ((arrKey[4] >> 48) & 0xffff);
	arrKey[2] = ((arrKey[0] << 32) & 0xffffffff00000000) ^ ((arrKey[4] >> 32) & 0xffffffff);
	arrKey[3] = ((arrKey[0] << 48) & 0xffff000000000000) ^ ((arrKey[4] >> 16) & 0xffffffffffffff);
}

ULL enSBox64(ULL in64)
{
    ULL out64 = arrSBox[in64& 0x000000000000000f] |
			((arrSBox[(in64 & 0x00000000000000f0) >> 4]) << 4) |
			((arrSBox[(in64 & 0x0000000000000f00) >> 8]) << 8) |
			((arrSBox[(in64 & 0x000000000000f000) >> 12]) << 12) |
			((arrSBox[(in64 & 0x00000000000f0000) >> 16]) << 16) |
			((arrSBox[(in64 & 0x0000000000f00000) >> 20]) << 20) |
			((arrSBox[(in64 & 0x000000000f000000) >> 24]) << 24) |
			((arrSBox[(in64 & 0x00000000f0000000) >> 28]) << 28) |
			((arrSBox[(in64 & 0x0000000f00000000) >> 32]) << 32) |
			((arrSBox[(in64 & 0x000000f000000000) >> 36]) << 36) |
			((arrSBox[(in64 & 0x00000f0000000000) >> 40]) << 40) |
			((arrSBox[(in64 & 0x0000f00000000000) >> 44]) << 44) |
			((arrSBox[(in64 & 0x000f000000000000) >> 48]) << 48) |
			((arrSBox[(in64 & 0x00f0000000000000) >> 52]) << 52) |
			((arrSBox[(in64 & 0x0f00000000000000) >> 56]) << 56) |
			((arrSBox[(in64 & 0xf000000000000000) >> 60]) << 60);
    return out64;
}

ULL enPBox(ULL in64)
{
    ULL mask = 1;
    
	for(int i = 0; i < 64; i++) 
    {
		arrPBox[i] = ((in64 & mask) == 0? 0 : 1);
		mask <<= 1;
	}
	in64 = 0;
	for (int i = 0; i < 64; i++) 
    {
		in64 ^= (arrPBox[arrPPos[i]]<<i);
	}
	return in64;
}

ULL SPN(ULL text, ULL* arrKey)
{
    for(int i=0;i <= 2;i++)
    {
        text ^= arrKey[i];
        text = enSBox64(text);
        text = enPBox(text);
    }

    text ^= arrKey[3];
    text = enSBox64(text);
    text ^= arrKey[4];  

    return text;   
}

int main()
{
    ULL arrKey[5] = {0};
    ULL plainText[1] = {0};
    ULL cipherText[1] = {0};
    ULL randomText = 0;
    fread(arrKey,SPNBYTES,1,stdin);
    fread(arrKey+4,SPNBYTES,1,stdin);
    genKey(arrKey);
    randomText = 22220012;

    fread(plainText,SPNBYTES,1,stdin);
    plainText[0] ^= randomText;
    cipherText[0] = SPN(plainText[0],arrKey);
    fwrite(cipherText,SPNBYTES,1,stdout);
    for(int i=1; i<(INPUTBYTES/SPNBYTES); i++)
    {
        fread(plainText,SPNBYTES,1,stdin);
        plainText[0] ^= cipherText[0];
        cipherText[0] = SPN(plainText[0],arrKey);
        fwrite(cipherText,SPNBYTES,1,stdout);
    }

    return 0;
}