//
//  skewImage.cpp
//  EraseLine
//
//  Created by George Godas on 1/27/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "skewImage.hpp"


Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}


double compute_skew(Mat src)
{
    // Load in grayscale.
    //cv::Mat src = cv::imread(filename, 0);
    cv::Size size = src.size();
    
    //cv::bitwise_not(src, src);
    //cv::imshow("Bitwise not", src);
    
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
    
    std::cout << "File " << ": " << angle * 180 / CV_PI << std::endl;
    Mat dst;
    cv::bitwise_not(src, src);
    double final_angle = ((angle * 180) / CV_PI) *-1;
    dst = rotate(src,  final_angle);
    
    return final_angle;
    //cv::destroyWindow(filename);
}


Mat correctAngle(Mat src, Mat image){
    Mat horizontal = image;
    double angle = compute_skew(horizontal);
    //horizontal = rotate(horizontal, angle * -1);
    src = rotate(src, angle * -1);
    
    
    return src;
    
}


Mat skew(Mat src){
    Mat horizontal = prepare(src.clone(), 15, -3);

    Mat correctedAngle = correctAngle(src.clone(), horizontal.clone());
    
    // Mescla linhas apagadas com imagem com angulo corrigido
    addWeighted(src, 1.0, horizontal, 1.0, 0.0, horizontal);

    return horizontal;
    
}