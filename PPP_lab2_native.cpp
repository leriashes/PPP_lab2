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

    string path = "bad.jpg";

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

    vector<unsigned char> img(data, data + (width * height * channels));
    stbi_image_free(data);

    vector<unsigned char> result(width * height * channels);


    int apert = 2;
    int alpha = 100;

    int n = 2 * apert + 1;
    int len = n * n;

    double* kernel = new double[len];

    vector<unsigned char> bordered;

    bordered = MakeImgWithBordersCopy(img, apert);



    if (stbi_write_png("result.jpg", width, height, channels, result.data(), width * channels)) {
        cout << "Изображение сохранено как result.jpg" << endl;
    }
    else {
        cerr << "Ошибка при сохранении изображения!" << endl;
    }

    return 0;
}
