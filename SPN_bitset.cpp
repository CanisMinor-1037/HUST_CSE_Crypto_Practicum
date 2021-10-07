#include<iostream>
#include<string>
#include<bitset>

std::bitset<16> sBox(std::bitset<16>);
std::bitset<16> invSBox(std::bitset<16>);
std::bitset<16> pBox(std::bitset<16>);
std::bitset<16> SPN_enc(std::bitset<16>*, std::bitset<16>);
std::bitset<16> SPN_dec(std::bitset<16>*, std::bitset<16>);

const unsigned long arrSBox[] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
const unsigned long arrInvSBox[] = {14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5};
const unsigned long arrPBox[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

int main()
{
    //n次输入
    int n;
    int pos;
    scanf("%d",&n);
    getchar();

    unsigned long ulKey;
    unsigned long plainText;
    unsigned long ulCipherText;
    unsigned long ulNewPlainText;
    std::bitset<16> *pSubKey = new std::bitset<16> [5];
    for(int i=0; i<n; i++)
    {
        
        std::cin >> std::hex >> ulKey;    //密钥长度32位

        //轮密钥pSubKey[Nr+1]
        for(int j=0; j<5; j++)
        {
            pos = 31 - 4*j;
            for(int k=15; k>=0; k++)
            {
                pSubKey[j].set(k, ulKey[pos]);
                pos --;
            }
        }

        std::cin >> std::hex >> plainText;  
        std::bitset<16> bPlainText(plainText);

        std::bitset<16> bCipherText = SPN_enc(pSubKey, bPlainText);  //加密
        ulCipherText = bCipherText.to_ulong();
        
        std::bitset<16> bNewPlainText = SPN_dec(pSubKey, (bCipherText.flip(0)));   //将CipherText最低位取反，解密   
        ulNewPlainText = bNewPlainText.to_ulong();

        printf("%04lx %04lx\n",ulCipherText,ulNewPlainText);
    }
    return 0;
}

std::bitset<16> sBox(std::bitset<16> bIn)
{
    std::string strIn = bIn.to_string();
    std::string strOut = "";
    unsigned long n;
    for(int i=0; i<4; i++)
    {
        std::bitset<4> bTemp (strIn.substr(4*i, 4));
        n = bTemp.to_ulong();
        n = arrSBox[n];
        std::bitset<4> bTemp_t (n);
        strOut += bTemp_t.to_string();
    }

    std::bitset<16> bOut(strOut);
    return bOut;
}

std::bitset<16> invSBox(std::bitset<16> bIn)
{
    std::string strIn = bIn.to_string();
    std::string strOut = "";
    unsigned long n;
    for(int i=0; i<4; i++)
    {
        std::bitset<4> bTemp (strIn.substr(4*i, 4));
        n = bTemp.to_ulong();
        n = arrInvSBox[n];
        std::bitset<4> bTemp_t (n);
        strOut += bTemp_t.to_string();
    }

    std::bitset<16> bOut(strOut);
    return bOut;
}

std::bitset<16> pBox(std::bitset<16> bIn)
{
    std::bitset<16> bOut = bIn;
    for(unsigned long i=0; i<16; i++)
    {
        bOut.set(arrPBox[i],bIn[i]);
    }
    return bOut;
}

std::bitset<16> SPN_enc(std::bitset<16>* pSubKey, std::bitset<16> bPlainText)
{
    std::bitset<16> bOut = bPlainText;
    for(int i=0; i<3; i++)
    {
        bOut = bOut ^ pSubKey[i];
        bOut = sBox(bOut);
        bOut = pBox(bOut);
    }

    bOut = bOut ^ pSubKey[3];
    bOut = sBox(bOut);
    bOut = bOut ^ pSubKey[4];

    return bOut;
}

std::bitset<16> SPN_dec(std::bitset<16>* pSubKey, std::bitset<16> bCipherText)
{
    std::bitset<16> bOut = bCipherText;
    bOut = bOut ^ pSubKey[4];
    bOut = invSBox(bOut);
    for(int i=3; i>0; i--)
    {
        bOut = bOut ^ pSubKey[i];
        bOut = pBox(bOut);
        bOut = invSBox(bOut);
    }

    bOut = bOut ^ pSubKey[0];
    return bOut;
}
