#include "Filters.h"

int NormalizeColor(int color)
{
	return 0;
}

MyImg MakeGrey(MyImg colored)
{
	return MyImg();
}

MyImg MakeImgWithBordersCopy(MyImg img, int apert)
{
    MyImg res;
    res.width = img.width + apert * 2;
    res.height = img.height + apert * 2;
    res.channels = img.channels;
    res.img = vector<unsigned char>(res.width * res.height * res.channels);

    for (int i = 0; i < res.width; i++)
    {
        for (int j = 0; j < res.height; j++)
        {
            int x, y;

            if (i < apert)
            {
                x = 0;
            }
            else if (i >= apert + img.width)
            {
                x = img.width - 1;
            }
            else
            {
                x = i - apert;
            }

            if (j < apert)
            {
                y = 0;
            }
            else if (j >= apert + img.height)
            {
                y = img.height - 1;
            }
            else
            {
                y = j - apert;
            }

            res.SetPixel(i, j, img.GetPixel(x, y));
        }
    }

    return res;
}

void CountKernelGauss(double* kernel, int apert)
{
}

void GaussFilter(MyImg img, int startRow, int endRow, MyImg result, double* kernel, int apert)
{
}

void MedianFilter(MyImg img, int startRow, int endRow, MyImg result, int apert)
{
}

void SobelFilter(MyImg img, int startRow, int endRow, MyImg result, int alpha)
{
}

void MyImg::SetPixel(int x, int y, MyPixel pixel)
{
    int index = (y * width + x) * channels;

    img[index] = pixel.r;
    img[index + 1] = pixel.g;
    img[index + 2] = pixel.b;

    if (channels == 4)
        img[index + 3] = pixel.a;
}

MyPixel MyImg::GetPixel(int x, int y)
{
    MyPixel res;

    int index = (y * width + x) * channels;

    res.r = img[index];
    res.g = img[index + 1];
    res.b = img[index + 2];

    if (channels == 4)
        res.a = img[index + 3];

    return res;
}
