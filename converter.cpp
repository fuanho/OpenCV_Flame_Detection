#include "converter.h"

void mycv::Converter::BGR2HSI(int R, int G, int B, int &H, int &S, int &I)
{
    double rn = (double)R/(R+G+B);
    double gn = (double)G/(R+G+B);
    double bn = (double)B/(R+G+B);
    I = (R+G+B)/3;
    S = (1 - 3 * std::min(rn, std::min(gn, bn))) * 255;
    H = std::acos((0.5 * ((rn - gn) + (rn - bn))) / (std::sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
    //if(bn <= gn) H = h/2;
    //else H = (360 - h)/2;
}

bool mycv::Converter::rule1(int R, int _Rt)
{
    if(R > _Rt)
        return true;
    else
        return false;
}

bool mycv::Converter::rule2(int R, int G, int B)
{
    if(R >= G && G > B)
        return true;
    else
        return false;
}

bool mycv::Converter::rule3(int R, int S, int _Rt, int _St)
{
    double value = (255-R)*_St/_Rt;
    if(S >= value)
        return true;
    else
        return false;
}

bool mycv::Converter::rule4(int I, int _It)
{
    if(I > _It)
        return true;
    else
        return false;
}

void mycv::Converter::cvtColor(cv::InputArray src, cv::OutputArray dst, int code)
{
    cv::Mat input = src.getMat();
    cv::Mat output = input.clone();
    auto inputData = input.data;
    auto outputData = output.data;
    int R, G, B;
    int H=0, S=0, I=0;

    switch (code) {
    case 0:
        inputData = input.data;
        outputData = output.data;
        for(auto y=0; y < input.cols; y++){
            for(auto x=0; x < input.rows; x++){
                B = inputData[0]; G = inputData[1]; R = inputData[2];
                BGR2HSI(B, G, R, H, S, I);
                outputData[0] = (uchar)H; outputData[1] = (uchar)S; outputData[2] = (uchar)I;
                inputData += 3;
                outputData += 3;
            }
        }
        break;
    }
    output.copyTo(dst);
}

void mycv::Converter::flameDetection(cv::InputArray src1, cv::InputArray src2, cv::OutputArray dst)
{
    cv::Mat frameColor = src1.getMat();
    cv::Mat frameHSI = src2.getMat();
    cv::Mat output = cv::Mat(frameColor.rows, frameColor.cols, CV_8UC1);
    auto frameColorData = frameColor.data;
    auto frameHSIData = frameHSI.data;
    auto outputData = output.data;
    int R, G, B, H, S, I;
    for(auto y=0; y < frameColor.cols; y++){
        for(auto x=0; x < frameColor.rows; x++){
            B = frameColorData[0]; G = frameColorData[1]; R = frameColorData[2];
            H = frameHSIData[0]; S = frameColorData[1]; I = frameColorData[2];
            if(rule1(R, Rt) && rule2(R, G, B) && rule3(R, S, Rt, St))
                outputData[0] = 255;
            else
                outputData[0] = 0;
            frameColorData += 3;
            frameHSIData += 3;
            outputData += 1;
        }
    }

    output.copyTo(dst);
}
