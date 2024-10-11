#define STB_IMAGE_IMPLEMENTATION  // Определяем макрос для реализации функций
#include "stb_image.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
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
        cerr << "Не удалось загрузить изображение!" << std::endl;
    }

    stbi_image_free(data);

    return 0;
}
