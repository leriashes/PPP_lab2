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

// Структура для передачи параметров в поток
struct FilterParam {
    MyImg* bordered;
    int startRow;
    int endRow;
    MyImg* result;
    vector<double>* kernel;
    int apert;
    int alpha;
};

static int k = 0;

// Функция потока для применения гауссова фильтра
DWORD WINAPI GaussFilterThread(LPVOID lpParam) {
    FilterParam* param = (FilterParam*)lpParam;
    GaussFilter(*param->bordered, param->startRow, param->endRow, *param->result, *param->kernel, param->apert);
    //cout << k++ << endl;
    return 0;
}

// Функция потока для применения медианного фильтра
DWORD WINAPI MedianFilterThread(LPVOID lpParam) {
    FilterParam* param = (FilterParam*)lpParam;
    MedianFilter(*param->bordered, param->startRow, param->endRow, *param->result, param->apert);

    //cout << 10 + k++ << endl;
    return 0;
}

// Функция потока для применения фильтра Собеля
DWORD WINAPI SobelFilterThread(LPVOID lpParam) {
    FilterParam* param = (FilterParam*)lpParam;
    SobelFilter(*param->bordered, param->startRow, param->endRow, *param->result, param->alpha);

    //cout << 100 + k++ << endl;
    return 0;
}

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

    //vector<unsigned char> img_data(data, data + (width * height * channels));

    width = 12800;
    height = 720;
    vector<unsigned char> img_data(width * height * 4);
    stbi_image_free(data);

    MyImg img(img_data, width, height, channels);
    MyImg result(img_data, width, height, channels);

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
        cout << "Изображение сохранено как result.jpg\n\n" << endl;
    }
    else {
        cerr << "Ошибка при сохранении изображения!" << endl;
    }


    int numThreads = 8;
    
    cout << "Введите количество потоков: ";
    cin >> numThreads;

    start = chrono::high_resolution_clock::now();

    bordered = MakeImgWithBordersCopy(img, apert);
    CountKernelGauss(kernel, apert);

    vector<HANDLE> threads(numThreads);
    vector<FilterParam> params(numThreads);

    int rowsPerThread = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        params[i] = { &bordered, i * rowsPerThread, (i == numThreads - 1) ? height - 1 : (i + 1) * rowsPerThread - 1, &result, &kernel, apert, alpha };
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = CreateThread(NULL, 0, GaussFilterThread, &params[i], 0, NULL);
    }

    for (HANDLE thread : threads) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }


    bordered = MakeImgWithBordersCopy(result, apert);

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = CreateThread(NULL, 0, MedianFilterThread, &params[i], 0, NULL);
    }

    //// Ожидание завершения потоков для медианного фильтра
    for (HANDLE thread : threads) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }

    // Фильтр Собеля
    bordered = MakeGrey(MakeImgWithBordersCopy(result, 1));

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = CreateThread(NULL, 0, SobelFilterThread, &params[i], 0, NULL);
    }

    // Ожидание завершения потоков для фильтра Собеля
    for (HANDLE thread : threads) {
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
    }

    end = chrono::high_resolution_clock::now();
    elapsed = end - start;

    cout << "\n\nВремя выполнения комбинированной фильтрации (многопоточный алгоритм): " << elapsed.count() << " секунд" << endl;

    if (stbi_write_png("result2.jpg", width, height, channels, result.img.data(), width * channels)) {
        cout << "Изображение сохранено как result.jpg\n\n" << endl;
    }
    else {
        cerr << "Ошибка при сохранении изображения!" << endl;
    }

    return 0;
}
