//
//  main.cpp
//  EraseLine
//
//  Created by George Godas on 8/6/15.
//  Copyright (c) 2015 George Godas. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}


void compute_skew(const char* filename)
{
    // Load in grayscale.
    cv::Mat src = cv::imread(filename, 0);
    cv::Size size = src.size();
    
    cv::bitwise_not(src, src);

    std::vector<cv::Vec4i> lines;
    
    cv::HoughLinesP(src, lines, 1, CV_PI/180, 100, size.width / 2.f, 10);
    
    cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = lines.size();
    for (unsigned i = 0; i < nb_lines; ++i)
    {
        cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
                 cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));
        angle += atan2((double)lines[i][3] - lines[i][1],
                       (double)lines[i][2] - lines[i][0]);
    }
    angle /= nb_lines; // mean angle, in radians.
    
    std::cout << "File " << filename << ": " << angle * 180 / CV_PI << std::endl;
    Mat dst;
     cv::bitwise_not(src, src);
    dst = rotate(src, (angle * 180 / CV_PI) *-1 );
    
    cv::imshow(filename, dst);
    cv::waitKey(0);
    cv::destroyWindow(filename);
}

const char* files[] = { "/Users/georgegodas/Documents/bem.png"};

int main(int argc, const char * argv[]) {

    
   	unsigned nb_files = sizeof(files) / sizeof(const char*);
    for (unsigned i = 0; i < nb_files; ++i)
        compute_skew(files[i]);
    
    return 0;
}
