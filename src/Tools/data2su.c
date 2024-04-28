#include "main.h"
#include "par.h"
#include "segy.h"

typedef struct headInformation
{
    int is;
    float isp;
    float firec;
    int ntr;
    int totaltr;
} headI;
int xargc;
char **xargv;

int main(int argc, char *argv[])
{
    int i, j, k, is, ir;
    int ntr;
    float dtr, dt;
    int nt;
    int nshot;
    int scale;
    float temp;
    SEGY *tr;
    tr = (SEGY *)malloc(sizeof(SEGY));
    printf("sizeof(segy)=%lu\n", sizeof(SEGY));

    char *headInfo, *shotfile, *shotfilesu;

    initargs(argc, argv);
    if (!getparint("nt", &nt))
        nt = 750;
    if (!getparint("nshot", &nshot))
        nshot = 1;
    if (!getparfloat("dtr", &dtr))
        dtr = 12.5;
    if (!getparfloat("dt", &dt))
        dt = 0.004;
    if (!getparstring("shotfile", &shotfile))
        shotfile = "shot.bin";
    if (!getparstring("headInfo", &headInfo))
        headInfo = "headInfo";
    if (!getparstring("shotfilesu", &shotfilesu))
        shotfilesu = "shot.su";

    FILE *headInfofp, *shotfp, *shotfilesufp;
    headInfofp = fopen(headInfo, "r");
    shotfp = fopen(shotfile, "rb");
    shotfilesufp = fopen(shotfilesu, "wb");

    headI *headInformation;
    headInformation = (headI *)malloc(sizeof(headI) * nshot);
    for (i = 0; i < nshot; i++)
    {
        fscanf(headInfofp, "%d,%f,%f,%d,%d\n", &headInformation[i].is, &headInformation[i].isp, &headInformation[i].firec, &headInformation[i].ntr, &headInformation[i].totaltr);
        // printf("%d %f %f %d\n",headInformation[i].is,headInformation[i].isp,headInformation[i].firec,headInformation[i].ntr);
    }
    fclose(headInfofp);

    for (is = 0; is < nshot; is++)
    {
        for (ir = 0; ir < headInformation[is].ntr; ir++)
        {
            tr->trid = 11;
            tr->sx = headInformation[is].isp;
            tr->gx = headInformation[is].firec + dtr * ir;
            tr->scalco = 1;
            tr->scalel = 1;
            tr->fldr = headInformation[is].is;
            tr->tracf = ir + 1;
            tr->cdp = tr->gx / dtr;
            tr->ep = headInformation[is].is;
            tr->ns = nt;
            tr->dt = 1000 * 1000 * dt;
            tr->tracl = ir + 1+headInformation[i].firec;
            tr->tracr = ir + 1+headInformation[i].firec;
            tr->fldr = headInformation[is].is;
            tr->counit=1;
            fwrite(tr, sizeof(SEGY), 1, shotfilesufp);
            
            for (i = 0; i < nt; i++)
            {
                fread(&temp, sizeof(float), 1, shotfp);
                fwrite(&temp, sizeof(float), 1, shotfilesufp);
            }
        }
    }

    fclose(shotfp);
    fclose(shotfilesufp);
    free(tr);

    return 0;
}