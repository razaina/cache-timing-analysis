#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "armpmu_lib.h"
#include <string.h>

unsigned char key[16];
AES_KEY expanded;
unsigned char zero[16];
unsigned char scrambledzero[16];
struct sockaddr_in server;
struct sockaddr_in client; socklen_t clientlen;
int s;
char in[1537];
int r;
char out[40];

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

    if(SIZE*NUMELEM != ret)
    {
        printf("fwrite() failed => ret = %d!\n", ret);
        fclose(fd);
        exit(EXIT_FAILURE);
    }
    return ret;
}


/*static inline uint32_t*/
/*get_cycle_count(void)*/
/*{*/
        /*uint32_t r = 0;*/
        /*asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );*/
        /*return r;*/
/*}*/

void handle(char out[40],char in[],int len)
{
    int workareaLen = len * 3;
    unsigned char workarea[workareaLen];
    int i;
    for (i = 0;i < 40;++i) out[i] = 0;
    *(unsigned int *) (out + 32) = get_cycle_count();
    if (len < 16) return;
    for (i = 0;i < 16;++i) out[i] = in[i];
    for (i = 16;i < len;++i) workarea[i] = in[i];
    /*printf("(");*/
    /*for(i = 0; i < len; ++i) printf("%x", in[i]);*/
    /*printf(") ==> [AES_encrypt] ==> ");*/
    AES_encrypt(in,workarea,&expanded);
    /*printf("[");*/
    /*for(i = 0; i < 16; ++i) printf("%x", workarea[i]);*/
    /*printf("] [");*/
    /*for(i = 16; i < len; ++i) printf("%x", workarea[i]);*/
    /*printf("] [");*/
    /*for(i = len; i < len * 2; ++i) printf("%x", workarea[i]);*/
    /*printf("]\n");*/

    /* a real server would now check AES-based authenticator, */
    /* process legitimate packets, and generate useful output */
    for (i = 0;i < 16;++i) out[16 + i] = scrambledzero[i];
    *(unsigned int *) (out + 36) = get_cycle_count();
    /*printf("RESPONSE ==> [");*/
    /*for(i = 0; i < 16; ++i) printf("%x", out[i]);*/
    /*printf("] [");*/
    /*for(i = 16; i < 32; ++i) printf("%x", out[i]);*/
    /*printf("] [");*/
    /*for(i = 32; i < 36; ++i) printf("%x", out[i]);*/
    /*printf("] [");*/
    /*for(i = 36; i < 40; ++i) printf("%x", out[i]);*/
    /*printf("]\n");*/

}

int main(int argc, char **argv)
{
    int i;
    if (read(0,key,sizeof key) < sizeof key) return 111;
    else {
        printf("\nKEY = ");
        for (i = 0; i < 16; i++) {
            printf("%x ", key[i]);
        }
        printf("\n");
    }
    AES_set_encrypt_key(key,128,&expanded);
    AES_encrypt(zero,scrambledzero,&expanded);
    printf("\nSCRAMBLED ZERO = ");
    for (i = 0; i < 16; i++) {
        printf("%x ", scrambledzero[i]);
    }
    printf("\n");

    if (!argv[1]) return 100;
    if (!inet_aton(argv[1],&server.sin_addr)) return 100;

    server.sin_family = AF_INET;
    server.sin_port = htons(10000);
    s = socket(AF_INET,SOCK_DGRAM,0);

    if (s == -1) return 111;
    if (bind(s,(struct sockaddr *) &server,sizeof server) == -1)
        return 111;
    FILE *fd = cyclecounter_open();
    for (;;) {
        clientlen = sizeof client;
        r = recvfrom(s,in,sizeof in,0
        ,(struct sockaddr *) &client,&clientlen);
        /*printf("\nRECEIVED %d PACKETS\n", r);*/
        if (r < 16) continue;
        if (r >= sizeof in) continue;
        cyclecounter_flushall(fd);
        handle(out,in,r);
        sendto(s,out,40,0,(struct sockaddr *) &client,clientlen);
    }
    cyclecounter_close(fd);
}
