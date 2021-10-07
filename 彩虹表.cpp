#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;
void getstr(unsigned n, char str[8])
{
	str[0] = 'a'; str[1] = '0'; str[2] = '0'; str[3] = '0'; str[4] = '0'; str[5] = '0'; str[6] = '0'; str[7] = '0';
	int i = 2;
	while (n)
	{
		unsigned tmp = n % 36;
		if (tmp < 10)
			str[i++] = tmp + '0';
		else
		{
			str[i++] = tmp - 10 + 'a';
		}
		n = n / 36;
	}
}
inline void R(unsigned sha1[5], char str[8], int i)
{
	getstr((sha1[0] + sha1[1] * i) % 2176782336, str);
}
unsigned int SHA1_tmp;

#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))

#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))

inline int UnitSHA1(const char* str, int length, unsigned sha1[5]) {
	unsigned char* pp, * ppend;
	unsigned int l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
	H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
	for (i = 0; i < 20; K[i++] = 0x5A827999);
	for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
	for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
	for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
	l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
	if (!(pp = (unsigned char*)malloc((unsigned int)l))) return -1;
	for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
	for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
	*((unsigned int*)(pp + l - 4)) = length << 3;
	*((unsigned int*)(pp + l - 8)) = length >> 29;
	for (ppend = pp + l; pp < ppend; pp += 64) {
		for (i = 0; i < 16; W[i] = ((unsigned int*)pp)[i], i++);
		for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
		A = H0, B = H1, C = H2, D = H3, E = H4;
		for (i = 0; i < 80; i++) {
			TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
			E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
		}
		H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
	}
	free(pp - l);
	sha1[0] = H0, sha1[1] = H1, sha1[2] = H2, sha1[3] = H3, sha1[4] = H4;
	return 0;
}

int main() 
{
	bool flagFind = false;
    int m,pos,noLink,cnt,start_i;
	unsigned sha1[5], sha1Temp[5], sha1Test[5]; 
    char strTemp[9];
	
	scanf("%d", &m);
	getchar();

    char startStr[m][9];
    char endStr[m][9];  // startStr --Sha1--R--Sha1--R--...--Sha1--R--endStr
	
	for (int i = 0; i < m; i++) 
	{
		scanf("%s%s",startStr[i],endStr[i]);
		getchar();
	}

	
	/*strcpy(strTemp,startStr[0]);
	ofstream buffer("buffer.txt",ios::app);
	for(int i=0; i<10000; i++)
	{
		buffer << strTemp << endl;
		UnitSHA1(strTemp,8,sha1Temp);
		R(sha1Temp,strTemp, (i%100) + 1);
	}
	buffer.close();
	//printf("%s\n",strTemp);
	
	if(strcmp(strTemp,endStr[0]) == 0)
	{
		printf("equal\n");
	}
	return 0;*/
	
	scanf("%8x%8x%8x%8x%8x", &(sha1[0]), &(sha1[1]), &(sha1[2]), &(sha1[3]), &(sha1[4]));
	getchar();
	
	char posTemp[10000][9];

	for(start_i = 1; start_i <= 100; start_i ++)
	{
		memcpy(sha1Temp,sha1,5*sizeof(unsigned));
		for(pos = start_i-1; pos < 10000; pos++)
		{
			R(sha1Temp,strTemp,(pos%100)+1);
			if(pos%100 == 99)
			{
				memcpy(posTemp[(start_i-1)*100+(pos/100)],strTemp,8*sizeof(char));
			}
			UnitSHA1(strTemp,8,sha1Temp);
		}		
	}


	for(noLink = 0; noLink<m; noLink++)
	{
		for(pos=0; pos<10000; pos++)
		{
			if(strcmp(endStr[noLink],posTemp[pos]) == 0)
			{
				cnt = 99 - (pos%100);
				cnt = cnt*100 + pos/100;
				
				strcpy(strTemp, startStr[noLink]);
				for(int i=0; i<cnt; i++)
				{
					UnitSHA1(strTemp,8,sha1Temp);
					R(sha1Temp,strTemp,(i%100)+1);	
				}

				//printf("%d\n",noLink);
				UnitSHA1(strTemp,8,sha1Test);
				if(memcmp(sha1Test,sha1,5*sizeof(unsigned)) == 0)
				{
					printf("%s\n",strTemp);
					return 0;
				} 
			}
		}
	}
	printf("None\n");
	return 0;
}