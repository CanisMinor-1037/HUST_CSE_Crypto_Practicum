#include<iostream>
#include<gmp.h>
#define MAXLEN 10250
int main()
{
    mpz_t e,m,p,q,z;
    unsigned long size;
    char str_e[MAXLEN];
    char str_m[MAXLEN];
    char str_p[MAXLEN];
    char str_q[MAXLEN];
    int n;
    scanf("%d",&n);
    getchar();
    for(int i=0; i<n; i++)
    {
        scanf("%s %s %s %s",str_e,str_m,str_p,str_q);
        getchar();
        mpz_init_set_str(e,str_e,10);
        mpz_init_set_str(m,str_m,10);
        mpz_init_set_str(p,str_p,10);
        mpz_init_set_str(q,str_q,10);
        mpz_init_set_ui(z,1);
        
        //gmp_printf("%Zd %Zd %Zd %Zd\n",e,m,p,q);
        mpz_mul(p,p,q);
        size = mpz_sizeinbase(e,2);
        char *bin_str_E = new char [size+2];
        mpz_get_str(bin_str_E,2,e);
        //printf("%lu %s\n",size,bin_str_E);
        for(unsigned long i=0; i<size; i++)
        {
            mpz_mul(z,z,z);
            mpz_mod(z,z,p);
            //gmp_printf("%Zd\n",z);
            if(bin_str_E[i] == '1')
            {
                mpz_mul(z,z,m);
                mpz_mod(z,z,p);
            }
        }
        gmp_printf("%Zd\n",z);
        mpz_clear(e);
        mpz_clear(m);
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(z);
        delete [] bin_str_E;
    }
    return 0;
}