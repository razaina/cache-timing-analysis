#include <stdio.h>
#include <stdlib.h>
#include "armpmu_lib.h"
#include <time.h>

typedef unsigned char	u8;	
typedef unsigned short	u16;	
typedef unsigned int	u32;

#define L1_CACHE_SIZE 32000 


char arr[L1_CACHE_SIZE];
FILE* fd;
time_t t;

void Test1();

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


void Test1()
{
    u32 time1, time2, t, ch1, cm1, o1, dr, dw;
    FILE* fp = fopen("/data/local/tmp/trace", "w+");
    int i = -1;
    int tmp;
    int index;

    cyclecounter_flushall(fd);

    for(;;)
    {
        index = ++i;
        if(index >= L1_CACHE_SIZE || i >= L1_CACHE_SIZE) break;

        enable_counters();
        time1 = get_cycle_count();

        tmp = arr[index] + 1;

        time2 = get_cycle_count();
        disable_counters();
        ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);

        fprintf(fp, "%u %u %u 0x%X %u %u\n", 
                    (time2 - time1), 
                    ch1, cm1, o1,
                    dr, dw);
    }

    i = -1;

    for(;;)
    {
        index = ++i + 32;
        if(index >= L1_CACHE_SIZE || i >= L1_CACHE_SIZE) break;

        enable_counters();
        time1 = get_cycle_count();

        tmp = arr[index] + 1;

        time2 = get_cycle_count();
        disable_counters();
        ch1 = read_pmn(0), cm1 = read_pmn(1), o1 = read_flags(), dr = read_pmn(2), dw = read_pmn(3);

        fprintf(fp, "%u %u %u 0x%X %u %u\n", 
                    (time2 - time1), 
                    ch1, cm1, o1,
                    dr, dw);
    }
}
