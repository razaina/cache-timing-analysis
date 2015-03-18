#include <stdio.h>
#include <stdlib.h>
#include "armpmu_lib.h"
#include <time.h>

typedef unsigned char	u8;	
typedef unsigned short	u16;	
typedef unsigned int	u32;

#define L1_CACHE_SIZE 32*1024 


char arr[L1_CACHE_SIZE];
FILE* fd;
time_t t;

int Test1();

int main(int argc, char *argv[])
{

    int i;
    fd = cyclecounter_open();
    srand((unsigned) time(&t));

    for(i = 0; i < L1_CACHE_SIZE; i++)
    {
        arr[i] = (char)rand()%256;
    }

    Test1();

    cyclecounter_close(fd);
}


        /*ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);*/

        /*fprintf(fp, "%u %u %u 0x%X %u %u\n", */
                    /*(time2 - time1), */
                    /*ch1, cm1, o1,*/
                    /*dr, dw);*/
int Test1()
{
    u32 time1, time2, t, ch1, cm1, o1, dr, dw;
    FILE* fp = fopen("/data/local/tmp/trace", "w+");
    int i = -1;
    int tmp, tmp2;
    int index;
    int limit = L1_CACHE_SIZE/6;
    /*u32 nbcycles[limit];*/
    /*u32 nbcachehits[limit];*/
    /*u32 nbcachemisses[limit];*/
    /*u32 nbdatareads[limit];*/
    /*u32 nbdatawrites[limit];*/


    int j, k = 0;
    cyclecounter_flushall(fd);
    char *dummyArr;

    for(;;)
    {
        index = ++i;
        /*if(k > 0 && index%limit == 0)*/
        /*{*/
            /*for(j = 0; j < limit; j++)*/
            /*{*/
                /*fprintf(fp, "%u %u %u %u %u\n", */
                    /*nbcycles[j], */
                    /*nbcachehits[j], nbcachemisses[j],*/
                    /*nbdatareads[j], nbdatawrites[j]);*/
            /*}*/
            /*fflush(fp);*/
            /*cyclecounter_flushall(fd);*/
        /*}*/

        if(i >= L1_CACHE_SIZE) break;

        enable_counters();
        time1 = get_cycle_count();

        /*tmp = arr[i] + 1;*/
        /*asm volatile("ldr r3, [%[array]]" :: [array] "r" (arr));*/
        asm volatile("ldr %[result], [%[array]]" : [result] "=r"(dummyArr) : [array] "r" (arr));
        /*asm volatile("ldr r3, [r4,r3]");*/
        /*asm volatile("add r3, r3, #1");*/
         /*asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));*/


        time2 = get_cycle_count();
        disable_counters();

        ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);

        fprintf(fp, "%u %u %u 0x%X %u %u\n", 
                    (time2 - time1), 
                    ch1, cm1, o1,
                    dr, dw);
        /*fflush(fp);*/
        cyclecounter_flushall(fd);
        /*nbcycles[index%limit] = time2 - time1;*/
        /*nbcachehits[index%limit] = read_pmn(0);*/
        /*nbcachemisses[index%limit] = read_pmn(1);*/
        /*nbdatareads[index%limit] = read_pmn(2);*/
        /*nbdatawrites[index%limit] = read_pmn(3);*/
        /*++k;*/
    }

    return tmp;


    /*i = -1;*/

    /*for(;;)*/
    /*{*/
        /*index = ++i + 32;*/
        /*if(index >= L1_CACHE_SIZE || i >= L1_CACHE_SIZE) break;*/

        /*enable_counters();*/
        /*time1 = get_cycle_count();*/

        /*tmp = arr[index] + 1;*/

        /*time2 = get_cycle_count();*/
        /*disable_counters();*/
        /*ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);*/

        /*fprintf(fp, "%u %u %u 0x%X %u %u\n", */
                    /*(time2 - time1), */
                    /*ch1, cm1, o1,*/
                    /*dr, dw);*/
        /*fflush(fp);*/
    /*}*/
}
