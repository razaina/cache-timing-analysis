#ifndef ARMPMU_LIB_H
#define ARMPMU_LIB_H
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define L1_INS_CACHE_MISS 0x01
#define L1_CACHE_MISS 0x03
#define L1_CACHE_HIT 0x04
#define DATA_READ 0x06
#define DATA_WRITE 0x07

 
static inline void enable_counters();
static inline void disable_coutners();

// Returns the number of progammable counters
unsigned int getPMN(void);

// Sets the event for a programmable counter to record
// counter = r0 = Which counter to program  (e.g. 0 for PMN0, 1 for PMN1)
// event   = r1 = The event code (from appropiate TRM or ARM Architecture Reference Manual)
void pmn_config(unsigned int counter, unsigned int event);

// Enables/disables the divider (1/64) on CCNT
// divider = r0 = If 0 disable divider, else enable dvider
void ccnt_divider(int divider);

//
// Enables and disables
//

// Global PMU enable
// On ARM11 this enables the PMU, and the counters start immediately
// On Cortex this enables the PMU, there are individual enables for the counters
void enable_pmu(void);

// Global PMU disable
// On Cortex, this overrides the enable state of the individual counters
void disable_pmu(void);

// Enable the CCNT
void enable_ccnt(void);

// Disable the CCNT
void disable_ccnt(void);

// Enable PMN{n}
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void enable_pmn(unsigned int counter);

// Enable PMN{n}
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void disable_pmn(unsigned int counter);

//
// Read counter values
//

// Returns the value of CCNT
unsigned int read_ccnt(void);

// Returns the value of PMN{n}
// counter = The counter to read (e.g. 0 for PMN0, 1 for PMN1)
unsigned int read_pmn(unsigned int counter);

//
// Overflow and interrupts
//

// Returns the value of the overflow flags
unsigned int read_flags(void);

// Writes the overflow flags
void write_flags(unsigned int flags);

// Enables interrupt generation on overflow of the CCNT
void enable_ccnt_irq(void);

// Disables interrupt generation on overflow of the CCNT
void disable_ccnt_irq(void);

// Enables interrupt generation on overflow of PMN{x}
// counter = The counter to enable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void enable_pmn_irq(unsigned int counter);

// Disables interrupt generation on overflow of PMN{x}
// counter = r0 =  The counter to disable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void disable_pmn_irq(unsigned int counter);

//
// Counter reset functions
//

// Resets the programmable counters
void reset_pmn(void);

// Resets the CCNT
void reset_ccnt(void);

//
// Software Increment

// Writes to software increment register
// counter = The counter to increment (e.g. 0 for PMN0, 1 for PMN1)
void pmu_software_increment(unsigned int counter);

//
// User mode access
//

// Enables User mode access to the PMU (must be called in a priviledged mode)
void enable_pmu_user_access(void);

// Disables User mode access to the PMU (must be called in a priviledged mode)
void disable_pmu_user_access(void);


static inline uint32_t
get_cycle_count(void)
{
    uint32_t r = 0;
    asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
    return r;
}

static inline void enable_counters()
{
    enable_pmu();
    reset_ccnt();
    reset_pmn();
    pmn_config(0, L1_CACHE_HIT);
    pmn_config(1, L1_CACHE_MISS);
    pmn_config(2, DATA_READ);
    pmn_config(3, DATA_WRITE);

    enable_ccnt();
    enable_pmn(0);
    enable_pmn(1);
    enable_pmn(2);
    enable_pmn(3);
}

static inline void disable_counters()
{
    disable_ccnt();
    disable_pmn(0);
    disable_pmn(1);
    disable_pmn(2);
    disable_pmn(3);
}

FILE* cyclecounter_open(void)
{
    FILE* fd = fopen("/dev/cyclecounter", "w");
    return fd;
}


void cyclecounter_close(FILE* fd)
{
    fclose(fd);
}

int cyclecounter_flushall(FILE* fd)
{
    int SIZE = 1;
    int NUMELEM = 1;
    int ret = 0;
    char action[] = "3";

    ret = fwrite(action, SIZE, strlen(action), fd);
    fflush(fd);

    if(SIZE*NUMELEM != ret)
    {
        printf("fwrite() failed => ret = %d!\n", ret);
        fclose(fd);
        exit(-1);
    }
    return ret;
}


#endif /* ARMPMU_LIB_H */
