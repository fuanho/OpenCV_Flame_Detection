#ifndef CONVERTER_H
#define CONVERTER_H

#include<opencv2/opencv.hpp>

namespace mycv{
enum ColorConversionCodes
{
    COLOR_BGR2HSI = 0,
    COLOR_HSI2BGR = 1
};
enum ThreSholdBoundary
{
    Rt = 115,
    St = 55,
    It = 205
};

class Converter
{
private:
    inline static void BGR2HSI(int R, int G, int B, int& H, int& S, int& I);
    inline static bool rule1(int R, int _Rt);
    inline static bool rule2(int R, int G, int B);
    inline static bool rule3(int R, int S, int _Rt, int _St);
    inline static bool rule4(int I, int _It);

public:
    static void cvtColor(cv::InputArray src, cv::OutputArray dst, int code);
    static void flameDetection(cv::InputArray src1, cv::InputArray src2, cv::OutputArray dst);
};
}
#endif // CONVERTER_H
