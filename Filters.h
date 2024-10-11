#pragma once
#include <vector>
using namespace std;

int NormalizeColor(int color);
vector<unsigned char> MakeGrey(vector<unsigned char> colored);
vector<unsigned char> MakeImgWithBordersCopy(vector<unsigned char> img, int apert);
void CountKernelGauss(double* kernel, int apert);
void GaussFilter(vector<unsigned char> img, int startRow, int endRow, vector<unsigned char> result, double* kernel, int apert);
void MedianFilter(vector<unsigned char> img, int startRow, int endRow, vector<unsigned char> result, int apert);
void SobelFilter(vector<unsigned char> img, int startRow, int endRow, vector<unsigned char> result, int alpha);