#include "Filters.h"

int NormalizeColor(int color)
{
    if (color < 0)
    {
        color = 0;
    }
    else if (color > 255)
    {
        color = 255;
    }

    return color;
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

void CountKernelGauss(vector<double>& kernel, int apert)
{
    double sigma = round((double)apert * 10.0 / 3) / 10;

    int n = 2 * apert + 1;
    int len = int(pow(n, 2));

    for (int i = -apert; i < apert + 1; i++)
    {
        for (int j = -apert; j < apert + 1; j++)
        {
            kernel[(i + apert) * n + j + apert] = exp(-(i * i + j * j) / (2 * sigma * sigma));
        }
    }

    double A = 0;

    for (int i = 0; i < len; i++)
    {
        A += kernel[i];
    }

    for (int i = -apert; i < apert + 1; i++)
    {
        for (int j = -apert; j < apert + 1; j++)
        {

            kernel[(i + apert) * n + j + apert] /= A;
        }
    }
}

void GaussFilter(MyImg img, int startRow, int endRow, MyImg* result, vector<double> kernel, int apert)
{
    int n = apert * 2 + 1;

    for (int i = 0; i < result->width; i++)
    {
        for (int j = startRow; j <= endRow; j++)
        {
            double R = 0;
            double G = 0;
            double B = 0;

            for (int k = 0; k < n; k++)
            {
                for (int l = 0; l < n; l++)
                {
                    MyPixel pixel = img.GetPixel(i + l, j + k);

                    R += pixel.r * kernel[k * n + l];
                    G += pixel.g * kernel[k * n + l];
                    B += pixel.b * kernel[k * n + l];
                }
            }

            MyPixel respix;
            respix.r = NormalizeColor(int(round(R)));
            respix.g = NormalizeColor(int(round(G)));
            respix.b = NormalizeColor(int(round(B)));
            respix.a = result->GetPixel(i, j).a;

            result->SetPixel(i, j, respix);
        }
    }
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
