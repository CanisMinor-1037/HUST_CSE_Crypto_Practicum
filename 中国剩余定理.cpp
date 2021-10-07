#include<gmp.h>
#include<stdio.h>
int main()
{
    int n;
    scanf("%d",&n);
    getchar();

    mpz_t p,q,e,d,c,pq,p_1,q_1,phi;
    mpz_t c1,c2,invp,invq;
    mpz_t tempp,tempq;
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(d);
    mpz_init(c);
    mpz_init(pq);
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_init(phi);
    mpz_init(c1);
    mpz_init(c2);
    mpz_init(invp);
    mpz_init(invq);
    mpz_init(tempp);
    mpz_init(tempq);
    gmp_scanf("%Zd%Zd%Zd",p,q,e);
    getchar();
    mpz_mul(pq,p,q);
    mpz_sub_ui(p_1,p,1);
    mpz_sub_ui(q_1,q,1);
    mpz_mul(phi,p_1,q_1);
    mpz_invert(d,e,phi);
    mpz_invert(invq,q,p);
    mpz_invert(invp,p,q);
    mpz_mul(tempq,q,invq);
    mpz_mul(tempp,p,invp);
    for(int i=0; i<n; i++)
    {
        gmp_scanf("%Zd",c);
        getchar();

        mpz_powm(c1,c,d,p);
        mpz_powm(c2,c,d,q);
        
        mpz_mul(c1,c1,tempq);      
        mpz_mul(c2,c2,tempp);

        mpz_add(c1,c1,c2);
        mpz_mod(c1,c1,pq);

        gmp_printf("%Zd\n",c1);
    }

    return 0;
}