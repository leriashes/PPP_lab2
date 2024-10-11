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

    vector<unsigned char> img_data(data, data + (width * height * channels));
    stbi_image_free(data);

    MyImg img;
    img.img = img_data;
    img.channels = channels;
    img.width = width;
    img.height = height;

    MyImg result;
    result.img = img_data;
    result.channels = channels;
    result.width = width;
    result.height = height;

    int apert = 2;
    int alpha = 100;

    int n = 2 * apert + 1;
    int len = n * n;

    vector<double> kernel(len);

    MyImg bordered;


    auto start = chrono::high_resolution_clock::now();

    bordered = MakeImgWithBordersCopy(img, apert);
    CountKernelGauss(kernel, apert);
    GaussFilter(bordered, 0, img.height - 1, result, kernel, apert);

    bordered = MakeImgWithBordersCopy(result, apert);
    MedianFilter(bordered, 0, img.height - 1, result, apert);

    bordered = MakeGrey(MakeImgWithBordersCopy(result, 1));
    SobelFilter(bordered, 0, img.height - 1, result, alpha);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "\n\nВремя выполнения комбинированной фильтрации (последовательный алгоритм): " << elapsed.count() << " секунд" << endl;

    if (stbi_write_png("result.jpg", width, height, channels, result.img.data(), width * channels)) {
        cout << "Изображение сохранено как result.jpg" << endl;
    }
    else {
        cerr << "Ошибка при сохранении изображения!" << endl;
    }

    return 0;
}
