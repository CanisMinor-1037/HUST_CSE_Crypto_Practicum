#include<gmp.h>
#include<stdio.h>
void NewGcd(mpz_t rop,const mpz_t op1,const mpz_t op2)
{
        if(mpz_cmp_ui(op1,0)==0 && mpz_cmp_ui(op2,0)==0)
        {
                mpz_set_ui(rop,0);
                return;
        } //gcd(0,0)=0
        if(mpz_cmp_ui(op1,0)!=0 && mpz_cmp_ui(op2,0)==0)
        {
                mpz_set(rop,op1);
                return;
        }
        if(mpz_cmp_ui(op1,0)==0 && mpz_cmp_ui(op2,0)!=0)
        {
                mpz_set(rop,op2);
                return;
        }

        mpz_t a, b, q,r;
        mpz_init_set(a,op1);
        mpz_init_set(b,op2);
        mpz_init(q);
        mpz_init(r);
        
        while(true)
        {               
                mpz_tdiv_qr(q,r,a,b);
                if(mpz_cmp_ui(r,0)==0)
                {
                        break;
                }
                mpz_set(a,b);
                mpz_set(b,r);
        }
        mpz_set(rop,b);
}

void NewInv(mpz_t rop,const mpz_t op1, const mpz_t op2)
{
        mpz_t array00,array01,array10,array11,r,q,temp;
        mpz_init_set_ui(array00,1);
        mpz_init_set(array01,op1);
        mpz_init_set(array11,op2);
        mpz_init(q);
        mpz_init(r);
        mpz_init(temp);
        
        mpz_tdiv_qr(q,r,array11,array01);
        mpz_init_set(array10,q);
        mpz_set(array11,r);

        mpz_tdiv_qr(q,r,array01,array11);
        mpz_mul(temp,array10,q);
        mpz_add(array00,array00,temp);
        mpz_set(array01,r);

        while(mpz_cmp_ui(array01,1)!=0)
        {
                mpz_tdiv_qr(q,r,array11,array01);
                mpz_mul(temp,array00,q);
                mpz_add(array10,array10,temp);
                mpz_set(array11,r);          
                
                mpz_tdiv_qr(q,r,array01,array11);
                mpz_mul(temp,array10,q);
                mpz_add(array00,array00,temp);
                mpz_set(array01,r);

        }
        mpz_set(rop, array00);
}

int main()
{
        int n;
        mpz_t e,d,p,q, p_1, q_1, phi_N,gcd,p_q,p_10;
        mpz_init(e);
        mpz_init(d);
        mpz_init(p);
        mpz_init(q);
        mpz_init(p_1);
        mpz_init(q_1);
        mpz_init(phi_N);
        mpz_init(gcd);
        mpz_init(p_q);
        mpz_init(p_10);
        int isPrimeP;
        int isPrimeQ; 
        
        scanf("%d",&n);
        getchar();

        for(int i=0; i<n; i++)
        {
                gmp_scanf("%Zd%Zd%Zd",e,p,q);
                getchar();

                mpz_sub_ui(p_1,p,1);
                mpz_sub_ui(q_1,q,1);
                mpz_mul(phi_N,p_1,q_1);
                
                NewGcd(gcd,phi_N,e);
                if(mpz_cmp_ui(gcd,1)!=0)
                {
                        printf("ERROR\n");
                        continue;
                }
                
                isPrimeP = mpz_probab_prime_p(p,15);
                if(isPrimeP==0)
                {
                        printf("ERROR\n");
                        continue;
                }

                isPrimeQ = mpz_probab_prime_p(q,15);
                if(isPrimeQ==0)
                {
                        printf("ERROR\n");
                        continue;
                }
                
                if(mpz_cmp_ui(e,10)<0)
                {
                        printf("ERROR\n");
                        continue;
                }

                NewGcd(gcd,p_1,q_1);
                if(mpz_cmp_ui(gcd,100000)>0)
                {
                        printf("ERROR\n");
                        continue;
                }
                
                mpz_sub(p_q,p,q);
                mpz_abs(p_q,p_q);
                mpz_div_ui(p_10,p,10);
                if(mpz_cmp(p_q,p_10)<0)
                {
                        printf("ERROR\n");
                        continue;
                }
                NewInv(d,e,phi_N);
                gmp_printf("%Zd\n",d);
        }
        return 0;
}