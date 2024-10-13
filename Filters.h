#pragma once
#include <vector>
using namespace std;

struct MyPixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	MyPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: r(r), g(g), b(b), a(a){}

	MyPixel() = default;
};

struct MyImg {
	vector<unsigned char> img;
	int width; 
	int height;
	int channels;

	void SetPixel(int x, int y, MyPixel pixel);
	MyPixel GetPixel(int x, int y);
	MyImg(const vector<unsigned char>& img, int width, int height, int channels)
		: img(img), width(width), height(height), channels(channels) {}

	MyImg(int width, int height, int channels)
		: width(width), height(height), channels(channels) {
		this->img = vector<unsigned char>(width * height * channels);
	}

	MyImg() = default;
};

int NormalizeColor(int color);
MyImg MakeGrey(MyImg colored);
MyImg MakeImgWithBordersCopy(MyImg img, int apert);
void CountKernelGauss(vector<double>& kernel, int apert);
void GaussFilter(MyImg img, int startRow, int endRow, MyImg& result, vector<double> kernel, int apert);
void SobelFilter(MyImg img, int startRow, int endRow, MyImg& result, int alpha);