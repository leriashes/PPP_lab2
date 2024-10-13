#include "Filters.h"
#include <algorithm>

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
    MyImg img(colored.width, colored.height, colored.channels);

    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            MyPixel pixel = colored.GetPixel(i, j);

            int brightness = NormalizeColor(int(round(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b)));

            MyPixel respix(brightness, brightness, brightness, pixel.a);
            img.SetPixel(i, j, respix);
        }
    }

    return img;
}

MyImg MakeImgWithBordersCopy(MyImg img, int apert)
{
    MyImg res(img.width + apert * 2, img.height + apert * 2, img.channels);

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

void GaussFilter(MyImg img, int startRow, int endRow, MyImg& result, vector<double> kernel, int apert)
{
    int n = apert * 2 + 1;

    for (int i = 0; i < result.width; i++)
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

            MyPixel respix(NormalizeColor(int(round(R))), NormalizeColor(int(round(G))), NormalizeColor(int(round(B))), result.GetPixel(i, j).a);
            result.SetPixel(i, j, respix);
        }
    }
}

void MedianFilter(MyImg img, int startRow, int endRow, MyImg& result, int apert)
{
    int n = apert * 2 + 1;
    int size = n * n;

    for (int i = 0; i < result.width; i++)
    {
        for (int j = startRow; j <= endRow; j++)
        {
            vector<int> rmas(size, 0);
            vector<int> gmas(size, 0);
            vector<int> bmas(size, 0);

            for (int k = 0; k < n; k++)
            {
                for (int l = 0; l < n; l++)
                {
                    MyPixel pixel = img.GetPixel(i + l, j + k);

                    rmas[k * n + l] = pixel.r;
                    gmas[k * n + l] = pixel.g;
                    bmas[k * n + l] = pixel.b;
                }
            }

            sort(rmas.begin(), rmas.end());
            sort(gmas.begin(), gmas.end());
            sort(bmas.begin(), bmas.end());

            MyPixel respix(rmas[apert], gmas[apert], bmas[apert], result.GetPixel(i, j).a);
            result.SetPixel(i, j, respix);
        }
    }
}

void SobelFilter(MyImg img, int startRow, int endRow, MyImg& result, int alpha)
{
    int n = 3;

    int* matrix = new int[9] { -1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < result.width; i++)
    {
        for (int j = startRow; j <= endRow; j++)
        {
            int Y = 0;

            for (int k = 0; k < n; k++)
            {
                for (int l = 0; l < n; l++)
                {
                    MyPixel pixel = img.GetPixel(i + l, j + k);

                    Y += pixel.r * (matrix[k * n + l] + matrix[l * n + k]);
                }
            }

            Y = NormalizeColor(Y);

            if (Y > alpha)
            {
                Y = 255;
            }
            else
            {
                Y = 0;
            }

            MyPixel respix(Y, Y, Y, result.GetPixel(i, j).a);
            result.SetPixel(i, j, respix);
        }
    }
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