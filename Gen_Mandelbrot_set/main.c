#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "time.h"
#define uchar unsigned char
 
/* Generate image (bmp fromat) Mandelbrot set*/

typedef struct Pixel {
    uchar r;
    uchar g;
    uchar b;
} Pixel;
 
#pragma pack(push, 1)
typedef struct BmpFileHeader {
    unsigned short Signature; // must be 'BM'
    unsigned int SizeofFile;
    unsigned short Reserved1;
    unsigned short Reserved2;
    unsigned int OffsetPixels;
} BmpFileHeader;
#pragma pack(pop)
 
typedef struct BmpInfoHeader {
    unsigned int SizeofBmpInfoHeader; // must be 40
    int Width;
    int Height;
    short ColorPlanes; // must be 1
    short BitsPerPixel;
    int CompressionMethod; // we will use BI_RGB == 0, that is no compression
    int SizeofPixels;
    int HorisontalPixelsPerMeter;
    int VerticalPixelsPerMeter;
    unsigned int Colors;
    unsigned int ImportantColors;
} BmpInfoHeader;
 
typedef struct Escape {
    int Iters;
    int MaxIters;
    double r;
    double R;
} Escape;
 
 
Pixel RandomPixel() {
    srand(time(NULL));
    uchar r = 150 + rand() % 106;
    uchar g = 150 + rand() % 106;
    uchar b = 150 + rand() % 106;
    Pixel res = { r, g, b };
    return res;
}
 
Escape CalcEscape(double x, double y, double R, int maxIters) {
double a = x, b = y, r = 0;
int i;
for (i = 0; i < maxIters && r < R; ++i) {
double na = a;
double nb = b;
a = na*na-nb*nb+x;
b = 2*nb*na+y;
r = a*a+b*b;
}
Escape res = {i, maxIters, r, R};
return res;
}
 
Pixel ColorForEscape(Escape e, Pixel* palette) {
    if (e.Iters >= e.MaxIters) {
        Pixel black = { 255, 0, 0 };
        return black;
    }
    else {
        return palette[e.Iters];
    }
}
 
Pixel* MakePalette(int maxIters) {
    Pixel* palette = (Pixel*)calloc(maxIters, sizeof(Pixel));
    Pixel a = RandomPixel();
    Pixel b = RandomPixel();
    for (int i = 0; i < maxIters; ++i) {
        double f = 1.0 * i / maxIters;
        palette[i].r = (uchar)((1 - f) * a.r + f * b.r + 0.5);
        palette[i].g = (uchar)((1 - f) * a.g + f * b.g + 0.5);
        palette[i].b = (uchar)((1 - f) * a.b + f * b.b + 0.5);
    }
    return palette;
}
 
 
 
 
 
void CreatePixels(int width, int height, Pixel* image, short bitsPerPixel,
    uchar** ppixels, int* sizeofPixels) {
    int rowSize = (bitsPerPixel * width + 31) / 32 * 4;
    *sizeofPixels = rowSize * height * sizeof(uchar);
    uchar* pixels = (uchar*)calloc(rowSize * height, sizeof(uchar));
 
    int ucharsPerPixel = bitsPerPixel / 8;
 
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            Pixel* d = &image[row * width + col];
            d->r = 0xff & (int)(d->r);
            d->g = 0xff & (int)(d->g);
            d->b = 0xff & (int)(d->b);
            uchar* p = &pixels[row * rowSize + col * ucharsPerPixel];
            int r, g, b;
            switch (bitsPerPixel) {
            case 24:
            case 32:
                p[0] = (uchar)b;
                p[1] = (uchar)g;
                p[2] = (uchar)r;
                break;
            default:
                assert(0 && "Unsupported bitsPerPixel");
            }
        }
    }
  /*  for (int i = 0; i < 50; i++) {
        image[i + 200 * i + 100 + 25].b = 0;
        image[i + 200 * i + 100 + 25].g = 0;
        image[i + 200 * i + 100 + 25].r = 0;
    }*/
 
    *ppixels = pixels;
}
 
 
void WriteImage(const char* filename, int width, int height, Pixel* image) {
    short bitsPerPixel = 24;
    uchar** ppixels;
    ppixels = (uchar**)calloc(1, sizeof(uchar*));
    int* sizeofPixels;
    sizeofPixels = (int*)calloc(1, sizeof(int));
    CreatePixels(width, height, image, bitsPerPixel, ppixels, sizeofPixels);
 
    BmpFileHeader bmpFileHeader = { 0 };
    bmpFileHeader.Signature = 'B' + ('M' << 8);
    bmpFileHeader.OffsetPixels = sizeof(bmpFileHeader) + sizeof(BmpInfoHeader);
    bmpFileHeader.SizeofFile = bmpFileHeader.OffsetPixels + *sizeofPixels;
 
    BmpInfoHeader bmpInfoHeader = { 0 };
    bmpInfoHeader.BitsPerPixel = bitsPerPixel;
    bmpInfoHeader.ColorPlanes = 1;
    bmpInfoHeader.Width = width;
    bmpInfoHeader.Height = height;
    bmpInfoHeader.SizeofPixels = *sizeofPixels;
    bmpInfoHeader.SizeofBmpInfoHeader = sizeof(bmpInfoHeader);
    assert(sizeof(bmpInfoHeader) == 40);
    assert(sizeof(bmpFileHeader) == 14);
    FILE* f = fopen(filename, "wb");
   
 
 
    fwrite(&bmpFileHeader, sizeof(bmpFileHeader), 1, f);
    fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, f);
    fwrite(image, *sizeofPixels, 1, f);
    fclose(f);
}
 
int main(int argc, char* argv[]) {
    int width = argc > 1 ? atoi(argv[1]) : 800;
    int height = argc > 2 ? atoi(argv[2]) : 600;
    Pixel* image = (Pixel*)calloc(height * width, sizeof(*image));
srand(time(NULL));
   
     double x0=-2.4, y0=-1.3;
     double x1=0.7, y1=1.3;
 
    int maxIters = 1000;
    double escapeRadius = 2;
    Pixel *palette = MakePalette(maxIters);
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double x = x0 + (x1 - x0) * col / (width - 1);
            double y = y0 + (y1 - y0) * row / (height - 1);
            Escape e = CalcEscape(x, y, escapeRadius, maxIters);
            Pixel color = ColorForEscape(e, palette);
            image[row * width + col] = color;
        }
    }
    WriteImage("image.bmp", width, height, image);
    free(image);
}
