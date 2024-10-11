#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "Filters.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);

    string path = "3.png";

    //cin >> path;

    int width, height, channels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data) {
        cout << "Изображение загружено: " << width << "x" << height
            << ", Channels: " << channels << endl;
    }
    else {
        cerr << "Не удалось загрузить изображение!" << endl;
    }

    vector<unsigned char> img_data(data, data + (width * height * channels));
    stbi_image_free(data);

    MyImg img;
    img.img = img_data;
    img.channels = channels;
    img.width = width;
    img.height = height;

    MyImg result;


    int apert = 2;
    int alpha = 100;

    int n = 2 * apert + 1;
    int len = n * n;

    double* kernel = new double[len];

    MyImg bordered;

    bordered = MakeImgWithBordersCopy(img, apert);



    if (stbi_write_png("result.jpg", width, height, channels, result.img.data(), width * channels)) {
        cout << "Изображение сохранено как result.jpg" << endl;
    }
    else {
        cerr << "Ошибка при сохранении изображения!" << endl;
    }

    return 0;
}
