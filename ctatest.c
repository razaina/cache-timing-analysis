#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <time.h>
#include "rijndael-api-fst.h"
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/time.h>
#include "SCORES"
#include "data_50"
#include "data_60"
#include "data_70"
#include "data_80"

#define AES_KEY_LEN_128 128
#define AES_KEY_LEN_192 192
#define AES_KEY_LEN_256 256
#define ENCRYPT DIR_ENCRYPT
#define DECRYPT DIR_DECRYPT
#define BLOCK_SIZE 16
#define LIBHCYCLECOUNTER_MAGIC 0xd34db33f

#define TEST_CASE_P1K1P1K1 1
#define TEST_CASE_P1K1P2K1 2
#define TEST_CASE_FLUSH_P1K1P1K1 3
#define TEST_CASE_GP1K1P1K1 4
#define TEST_CASE_GP1K1P2K1 5
#define TEST_CASE_BERNSTEIN 6
#define N_LOOP 100000
#define N_LOOP_2 2000
#define N_LOOP_3 1000

#define MAX_FMT_SIZE 30

#define L1D_CACHE_REFILL 0x03
#define L1D_CACHE_ACCESS 0x04
#define COUNTER_0 0
#define COUNTER_1 1

void printBlock(const BYTE *block, const char *tag);
void Test2();
void Test1();
void AESTimeEncryptP1K1P2K1();
void AESTimeEncryptFlushP1K1P1K1();
void AESTimeEncryptGP1K1P1K1();
void AESTimeEncryptGP1K1P2K1();
void AESTimeEncryptP13();
void analysis();



int main(int argc, char *argv[])
{
    int opt;
    int test = 0;
    int outerloop = 0;

    printf("CTATest: ./ctatest -t NUM_TEST_CASE\n");
    while ((opt = getopt(argc, argv, "t:r:")) != -1)
    {
        switch(opt)
        {
            case 't':   test = atoi(optarg);
                        printf("Test = %d\n", test); break;

            case 'r':   outerloop = atoi(optarg);
                        printf("Outerloop = %d\n", outerloop); break;

            default: printf("What???\n"); exit(0);
        }
    }

    if(test != 0/* && outerloop != 0*/)
    {
        if(test == TEST_CASE_P1K1P1K1)
        {
            printf("Execution test case Test1\n");
            Test1();
            /*printf("Execution test case Test2\n");*/
            /*Test2();*/

        }
    }

    return 0;
}

void printBlock(const BYTE *block, const char *tag)
{
    int i;
    char *str = (char*)malloc(2*BLOCK_SIZE +100);
    char *ptr = str;
    for(i = 0; i < BLOCK_SIZE; i++)
    {
       ptr += sprintf(ptr, "0x%02X ", block[i]);
    }

    *(ptr + 1) = '\0';
    printf("%s => %s\n", tag, str);
}

void printBlock16(const BYTE *block, const char *tag)
{
    int i;
    char *str = (char*)malloc(2*16 +1000);
    char *ptr = str;
    for(i = 0; i < 16; i++)
    {
       ptr += sprintf(ptr, "0x%02X ", block[i]);
    }

    *(ptr + 1) = '\0';
    printf("%s => %s\n", tag, str);
}


char* toString(const BYTE *block)
{
    int i;
    char *str = (char*)malloc(2*16 +1000);
    char *ptr = str;
    for(i = 0; i < 16; i++)
    {
       ptr += sprintf(ptr, "0x%02X ", block[i]);
    }

    *(ptr + 1) = '\0';
    return str;
}


static inline uint32_t Op(FILE* fd, BYTE plaintextCandidat[], cipherInstance *cipherInst, keyInstance* keyInst, BYTE output[], int offset, int nbhits, int nloop, BYTE keyCandidat[], FILE* fp )
{
    uint32_t t1, t2;
    int r;
    u32 cm1, ch1, o1, dr, dw;

    /*cyclecounter_flushall(fd);*/
    /*enable_counters();*/
    /*t1 = get_cycle_count();*/
    r = blockEncrypt2(cipherInst, keyInst, plaintextCandidat, AES_KEY_LEN_128, output, fp, fd);
    /*t2 = get_cycle_count();*/
    /*disable_counters();*/
    /*ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);*/

    /*fprintf(fp, "%u [%s] [%s] %u %u 0x%X %u %u\n", */
                /*(t2 - t1), toString(plaintextCandidat), toString(keyCandidat), */
                /*ch1, cm1, o1,*/
                /*dr, dw);*/
    /*fflush(fp);*/
    /*fprintf(fp, "%u [%s] [%s] %u %u 0x%X %u %u %d %d\n", */
                /*(t2 - t1), toString(plaintextCandidat), toString(keyCandidat), */
                /*ch1, cm1, o1,*/
                /*dr, dw,*/
                /*offset, nbhits);*/
    /*fflush(fp);*/

    if(r != AES_KEY_LEN_128)
    {
        printf("Error encrypt\n\t[+] Offset: %d\n\t[+] nbhits : %d\n\t[+] nloop : %d\n\t[+] plain: %s\n\t[+] key: %s\n", offset, nbhits, nloop, toString(plaintextCandidat), toString(keyCandidat));
        exit(-1);
    }
    return (t2 - t1);
}

void Test2()
{
    int i, j, r, k, l;
    BYTE block[BLOCK_SIZE];
    BYTE output[BLOCK_SIZE];
    BYTE output2[BLOCK_SIZE];
    char keyMaterial[320];
    keyInstance keyInst;
    cipherInstance cipherInst;

    FILE* fd = cyclecounter_open();
    int nLoop = 1000;
    int nbOffsets = 8;
    uint32_t t1;
    FILE *fp = fopen ("/data/local/tmp/trace", "w+");

    BYTE* keyCandidat;
    BYTE* plaintextCandidat;


    int offset;
    for(offset = 0; offset < nbOffsets; offset++)
    {
        for(i = 0; i < nLoop; i++)
        {
            keyCandidat = data_80[offset][i][1];
            plaintextCandidat = data_80[offset][i][0];

            /*printBlock16(plaintextCandidat, "[+] plaincandidat");*/
            /*printBlock16(keyCandidat, "[+] keyCandidat");*/
            
            memset(keyMaterial, 0, sizeof(keyMaterial));
            memcpy(keyMaterial, keyCandidat, 16);

            r = makeKey(&keyInst, ENCRYPT, AES_KEY_LEN_128, keyMaterial);
            if(r != TRUE)
            {
                printf("[+] makeKey error %d\n", r);
                return;
            }

            r = cipherInit(&cipherInst, MODE_ECB, NULL);
            if(r != TRUE)
            {
                printf("[+] cipherInit error %d\n", r);
                return;
            }

            t1 = Op(fd, plaintextCandidat, &cipherInst, &keyInst, output, k, j, i, keyCandidat, fp);
            
        }
    }



}

void Test1(){
    int i, j, r, k, l;
    BYTE block[BLOCK_SIZE];
    BYTE output[BLOCK_SIZE];
    BYTE output2[BLOCK_SIZE];
    char keyMaterial[320];
    keyInstance keyInst;
    cipherInstance cipherInst;

    FILE* fd = cyclecounter_open();
    int nLoop = 1000;
    uint32_t t1;
    FILE *fp = fopen ("/data/local/tmp/trace", "w+");


    BYTE* keyCandidat;
    BYTE* plaintextCandidat;


    /*keyCandidat = data[0][60][1];*/
    /*plaintextCandidat = data[0][60][0];*/

    /*printBlock16(plaintextCandidat, "[+] plaincandidat");*/
    /*printBlock16(keyCandidat, "[+] keyCandidat");*/
    k = 0;
    j = 0;

    /*u32 Times[8][53][nLoop][1];*/
    for(k = 0; k < 8 ; k++)
    {
        for(j = 65; j <= 118; j++)
        {
            keyCandidat = data[k][j][1];
            plaintextCandidat = data[k][j][0];

            /*keyCandidat = data[0][118][1];*/
            /*plaintextCandidat = data[0][118][0];*/

            /*printBlock16(plaintextCandidat, "[+] plaincandidat");*/
            /*printBlock16(keyCandidat, "[+] keyCandidat");*/
            memset(keyMaterial, 0, sizeof(keyMaterial));
            memcpy(keyMaterial, keyCandidat, 16);
            /*printf("\n");*/

            r = makeKey(&keyInst, ENCRYPT, AES_KEY_LEN_128, keyMaterial);
            if(r != TRUE)
            {
                printf("[+] makeKey error %d\n", r);
                return;
            }

            r = cipherInit(&cipherInst, MODE_ECB, NULL);
            if(r != TRUE)
            {
                printf("[+] cipherInit error %d\n", r);
                return;
            }

            //===== ENCRYPT
            /*r = blockEncrypt2(&cipherInst, &keyInst, plaintextCandidat, AES_KEY_LEN_128, output);*/
            /*if(r != AES_KEY_LEN_128)*/
            /*{*/
                /*printf("Encrypt error\n");*/
                /*return;*/
            /*}*/
            //===========================

            //==== DECRYPT
            /*makeKey(&keyInst, DECRYPT, AES_KEY_LEN_128, keyMaterial);*/
            /*blockDecrypt(&cipherInst, &keyInst, output, 128, output2);*/
            /*printBlock16(output2, "[+} Deciphered ==== ");*/
            //===========================



            for(i = 0; i < nLoop; i++)
            {
                t1 = Op(fd, plaintextCandidat, &cipherInst, &keyInst, output, k, j, i, keyCandidat, fp);

                /*t2 = Op(fd, plaintextCandidat, &cipherInst, &keyInst, output, k, j, i, keyCandidat);*/
                /*ch2 = read_pmn(0), cm2 = read_pmn(1), o2 = read_pmn(2);*/

                /*t3 = Op(fd, plaintextCandidat, &cipherInst, &keyInst, output, k, j, i, keyCandidat);*/
                /*ch3 = read_pmn(0), cm3 = read_pmn(1), o3 = read_pmn(2);*/


                /*fprintf(fp, "%u %u %u %u [%s] [%s] %d %d\n", t1, t2, t3, t4, toString(plaintextCandidat), toString(keyCandidat), k, j);*/
                /*Time[k][j][i][0] = t1;
                Time[k][j][i][1] = t2;
                Time[k][j][i][2] = t3;*/
                /*fprintf(fp, "%u %u %u [%s] [%s] %u %u %u %u %u %u 0x%X 0x%X 0x%X %d %d\n", */
                            /*t1, t2, t3, toString(plaintextCandidat), toString(keyCandidat), */
                            /*ch1, ch2, ch3,*/
                            /*cm1, cm2, cm3,*/
                            /*o1, o2, o3, */
                            /*k, j);*/
            }
        }
    }
    fclose(fp);
    cyclecounter_close(fd);
    return;
}


