#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LCS _lrotl //left circular shift, get from pdf paper but only used for unsigned long
#define RCS _lrotr //right circular shift, get from pdf paper but only used for unsigned long
//#define R(x,y,k) (x=RCS(x,7), x+=y, x^=k, y=LCS(y,2), y^=x)

#define SPECK_KEY_LEN       4
#define SPECK_ROUNDS        22
#define SPECK_TYPE          uint16_t
#define ROR(x, r) ((x >> r) | (x << ((sizeof(SPECK_TYPE) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(SPECK_TYPE) * 8) - r)))
#define R(x, y, k) (x = ROR(x, 7), x += y, x ^= k, y = ROL(y, 2), y ^= x)

typedef uint32_t u32;
typedef uint16_t u16;


void Speck32ExpandKeyAndEncrypt(u16 pt[], u16 ct[], u16 K[])
{
    u16 i,D=K[3], C=K[2], B=K[1], A=K[0];
    ct[0]=pt[0]; ct[1]=pt[1];
    for(i=0; i<7; i++)
    {
        R(ct[1], ct[0], A); //data calculate
        printf("%04X %04X %04X\n",ct[1], ct[0], A);
        R(B, A, i*3); // ki schedule
//        printf("%04X\n",B);
        R(ct[1], ct[0], A); //data calculate
        printf("%04X %04X %04X\n",ct[1], ct[0], A);
        R(C, A, i*3+1); // k[i+1] schedule
//        printf("%04X\n",C);
        R(ct[1], ct[0], A); //data calculate
        printf("%04X %04X %04X\n",ct[1], ct[0], A);
        R(D, A, i*3+2); // k[i+2] schedule
//        printf("%04X\n",D);
    }
    R(ct[1], ct[0], A); //data calculate
    printf("%04X %04X %04X\n",ct[1], ct[0], A);
}

void speck_encrypt_combined(SPECK_TYPE const pt[static 2], SPECK_TYPE ct[static 2], SPECK_TYPE const K[static SPECK_KEY_LEN])
{
  SPECK_TYPE i, b = K[0];
  SPECK_TYPE a[SPECK_KEY_LEN - 1];
  ct[0]=pt[0]; ct[1]=pt[1];

  for (i = 0; i < (SPECK_KEY_LEN - 1); i++)
  {
    a[i] = K[i + 1];
  }

  R(ct[1], ct[0], b);
  printf("%04X %04X %04X\n",ct[1], ct[0], b);
  for(i = 0; i < SPECK_ROUNDS - 1; i++){
    R(a[i % (SPECK_KEY_LEN - 1)], b, i);
    R(ct[1], ct[0], b);
    printf("%04X %04X %04X\n",ct[1], ct[0], b);
  }
}

int main()
{
    u16 pt[2] = {0x6574,0x694c};
    u16 ptinv[2] = {0x694c,0x6574};
    u16 ct[2] = {0};
    u16 K[4] = {0x1918,0x1110,0x0908,0x0100};
    u16 Kinv[4] = {0x0100,0x0908,0x1110,0x1918};
    //res: ct[0] = {0x42f2, 0xA868};
//    printf("K\n %X %X %X %X\n",K[0],K[1],K[2],K[3]);
    Speck32ExpandKeyAndEncrypt(ptinv, ct, Kinv);
    printf("==============\n");
    speck_encrypt_combined(ptinv,ct,Kinv);
    printf("==============\n");
    printf("pt\n %04X %04X\n",pt[0],pt[1]);
    printf("ct\n %04X %04X\n",ct[0],ct[1]);
    printf("K\n %04X %04X %04X %04X\n",K[0],K[1],K[2],K[3]);

//    printf("pt = %08X, ptLCS = %08X, ptROL = %08X\n", pt[0], (u16)LCS(pt[0],4), (u16)ROL(pt[0],4));
    return 0;
}
