#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include <math.h>


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
    uchar *yOld = new uchar[xSize * ySize]();
    uchar *yNew = new uchar[newXSize * newYSize]();
    char *vOld = new char[xSize * ySize / 4]();
    char *uOld = new char[xSize * ySize / 4]();
    char *vNew = new char[newXSize * newYSize / 4]();
    char *uNew = new char[newXSize * newYSize / 4]();

    RGBtoYUV420(input, xSize, ySize, yOld, uOld, vOld);
    const double sfx = (double)newXSize / xSize;
    const double sfy = (double)newYSize / ySize;
    //int a, b;
    int xIndex, yIndex;

    for(int i = 0; i < newXSize; i++)
    {
        xIndex = i / sfx - 0.5;
        for(int j = 0; j < newYSize; j++)
        {
            yIndex = j / sfy - 0.5;
            yNew[j*newXSize+i] = yOld[yIndex*xSize + xIndex];
            if(!(j%2) && !(i%2))
            {
                uNew[j / 2 * newXSize / 2 + i / 2] = uOld[yIndex / 2 * xSize / 2 + xIndex/2];
                vNew[j / 2 * newXSize / 2 + i / 2] = vOld[yIndex / 2 * xSize / 2 + xIndex/2];
            }
        }
    }
    YUV420toRGB(yNew, uNew, vNew, newXSize, newYSize, output);

    delete[] yOld;
    delete[] yNew;
    delete[] uOld;
    delete[] uNew;
    delete[] vOld;
    delete[] vNew;
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
    uchar *yOld = new uchar[xSize * ySize]();
    uchar *yNew = new uchar[newXSize * newYSize]();
    char *vOld = new char[xSize * ySize / 4]();
    char *uOld = new char[xSize * ySize / 4]();
    char *vNew = new char[newXSize * newYSize / 4]();
    char *uNew = new char[newXSize * newYSize / 4]();

    RGBtoYUV420(input, xSize, ySize, yOld, uOld, vOld);
    const double sfx = (double)newXSize / xSize;
    const double sfy = (double)newYSize / ySize;
    double a,b;
    int ah,al,bh,bl;
    for(int i = 0; i < newXSize; i++)
    {
        a = i/sfx - (int)(i/sfx);
        for(int j = 0; j < newYSize; j++)
        {
            b = j/sfy - (int)(j/sfy);
            al = i/sfx;
            bl = j/sfy;
            bh = bl;
            ah = al;
            if (al < xSize - 1)
                ah = al + 1;
            if (bl < ySize - 1)
                bh = bl + 1;
            yNew[j*newXSize+i] = (1-a)*(1-b)*yOld[bl*xSize + al] + (1-a)*b*yOld[bh*xSize + al]
                    + a*(1-b)*yOld[bl*xSize +  ah] + a*b*yOld[bh*xSize + ah];
            if (!(j%2) && !(i%2))
            {
                uNew[j / 2 * newXSize / 2 + i / 2] = (1-a)*(1-b)*uOld[bl / 2 * xSize / 2 + al / 2] + (1-a)*b*uOld[bh / 2 * xSize / 2 + al / 2]
                    + a*(1-b)*uOld[bl / 2 * xSize / 2 +  ah / 2] + a*b*uOld[bh / 2 * xSize / 2 + ah / 2];
                vNew[j / 2 * newXSize / 2 + i / 2] = (1-a)*(1-b)*vOld[bl / 2 * xSize / 2 + al / 2] + (1-a)*b*vOld[bh / 2 * xSize / 2 + al / 2]
                    + a*(1-b)*vOld[bl / 2 * xSize / 2 +  ah / 2] + a*b*vOld[bh / 2 * xSize / 2 + ah / 2];
            }
        }
    }
    YUV420toRGB(yNew, uNew, vNew, newXSize, newYSize, output);

    delete[] yOld;
    delete[] yNew;
    delete[] uOld;
    delete[] uNew;
    delete[] vOld;
    delete[] vNew;
}

void bicubicInterpolate(uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
    uchar *yOld = new uchar[xSize * ySize]();
    uchar *yNew = new uchar[xSize * xSize]();
    char *vOld = new char[xSize * ySize / 4]();
    char *uOld = new char[xSize * ySize / 4]();
    char *vNew = new char[xSize * xSize / 4]();
    char *uNew = new char[xSize * xSize / 4]();

    RGBtoYUV420(input, xSize, ySize, yOld, uOld, vOld);

    angle = angle * M_PI / 180;
    for(int i = 0; i < xSize; i++)
    {
        for(int j = 0; j < ySize; j++)
        {
            int xCoord = i*cos(angle) - j*sin(angle)-m*cos(angle)+n*sin(angle)+m + 0.5;
            int yCoord = j*cos(angle) + i*sin(angle)-m*sin(angle)-n*cos(angle)+n+0.5;
            if(xCoord < xSize && yCoord < ySize && xCoord >= 0 && yCoord >= 0)
            {
                yNew[j*xSize+i] = yOld[yCoord*xSize+xCoord];
                if (!(j%2) && !(i%2))
                {
                    uNew[j/2*xSize/2+i/2] = uOld[yCoord/2*xSize/2+xCoord/2];
                    vNew[j/2*xSize/2+i/2] = vOld[yCoord/2*xSize/2+xCoord/2];
                }
            }else
            {
                yNew[j*xSize+i] = 0;
                if (!(j%2) && !(i%2))
                {
                    uNew[j/2*xSize/2+i/2] = 0;
                    vNew[j/2*xSize/2+i/2] = 0;
                }
            }
        }
    }
    YUV420toRGB(yNew, uNew, vNew, xSize, ySize, output);

    delete[] yOld;
    delete[] yNew;
    delete[] uOld;
    delete[] uNew;
    delete[] vOld;
    delete[] vNew;
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}
