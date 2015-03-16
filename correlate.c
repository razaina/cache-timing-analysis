#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double t[16][256];
double tdev[16][256];
double u[16][256];
double udev[16][256];
double c[256];
double v[256];
int cpos[256];


void readdata()
{
    double t[16][256];
    double tdev[16][256];
    double u[16][256];
    double udev[16][256];
    int lines = -1;
    int b;
    int size;
    int j;
    long long packets;
    double cycles;
    double deviation;
    double aboveaverage;
    double avdev;
    char line[200];
    FILE *fp = fopen("/data/local/tmp/plop", "rt");
    /* elapsed.dta is the name of the file */
    /* "rt" means open the file for reading text */
    if(!fp)
    {
        printf("ERROR problem opening /data/local/tmp/plop");
        return;
    }

    while(fgets(line, 200, fp) != NULL)
    {
        ++lines;
        /* get a line, up to 80 chars from fr.  done if NULL */
        /*sscanf (line, "%u", &t);*/
        if(sscanf( line,
                    "%d%d%d%lld%lf%lf%lf%lf"
                    ,&b
                    ,&size
                    ,&j
                    ,&packets
                    ,&cycles
                    ,&deviation
                    ,&aboveaverage
                    ,&avdev
                    ) != 8)
        {
            printf("ERROR, should have read 8 values!");
            exit(100);
        }

        b &= 15;
        j &= 255;
        if (lines < 4096)
        {
            t[b][j] = aboveaverage;
            tdev[b][j] = avdev;
        }
        else
        {
            u[b][j] = aboveaverage;
            udev[b][j] = avdev;
        }
    }
    fclose(fp);  /* close the file prior to exiting the routine */
}


int cposcmp(const void *v1,const void *v2)
{
    int *i1 = (int *) v1;
    int *i2 = (int *) v2;
    if (c[255 & *i1] < c[255 & *i2]) return 1;
    if (c[255 & *i1] > c[255 & *i2]) return -1;
    return 0;
}

void processdata(void)
{
    int b;
    int i;
    int j;
    int numok;
    double z;
    for (b = 0;b < 16;++b)
    {
        for (i = 0;i < 256;++i)
        {
            c[i] = v[i] = 0;
            cpos[i] = i;
            for (j = 0;j < 256;++j)
            {
                c[i] += t[b][j] * u[b][i ^ j];
                z = tdev[b][j] * u[b][i ^ j];
                v[i] += z * z;
                z = t[b][j] * udev[b][i ^ j];
                v[i] += z * z;
            }
        }
        qsort(cpos,256,sizeof(int),cposcmp);
        numok = 0;
        for (i = 0;i < 256;++i)
            if (c[cpos[0]] - c[cpos[i]] < 10 * sqrt(v[cpos[i]]))
                ++numok;

        printf("%3d %2d ",numok,b);
        for (i = 0;i < 256;++i)
            if (c[cpos[0]] - c[cpos[i]] < 10 * sqrt(v[cpos[i]]))
                printf(" %02x",cpos[i]);
        printf("\n");
    }
}

int main()
{
    readdata();
    processdata();
    return 0;
}