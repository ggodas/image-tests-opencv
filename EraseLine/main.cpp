//
//  main.cpp
//  EraseLine
//
//  Created by George Godas on 8/6/15.
//  Copyright (c) 2015 George Godas. All rights reserved.
//
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include "FindEdges.hpp"



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

Mat getHorizontal(Mat image){
    Mat horizontal = image.clone();
    // Specify size on horizontal axis
    int horizontalsize = horizontal.cols / 60;
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    
    return horizontal;
}

Mat correctAngle(Mat src, Mat image){
    Mat horizontal = image;
    double angle = compute_skew(horizontal);
    //horizontal = rotate(horizontal, angle * -1);
    src = rotate(src, angle * -1);
    
    
    return src;
    
}


Mat getVertical(Mat image){
    Mat vertical = image.clone();
    
    // Specify size on vertical axis
    int verticalsize = vertical.rows / 30;
    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size( 1,verticalsize));
    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    // Show extracted vertical lines
    //imshow("vertical", vertical);
    // Inverse vertical image
    bitwise_not(vertical, vertical);
    return vertical;
}

Mat convertToGray(Mat image){
    Mat gray;
    if (image.channels() == 3)
    {
        cvtColor(image, gray, CV_BGR2GRAY);
    }
    else
    {
        gray = image;
    }
    return gray;
}

Mat prepareImage(Mat input){
    //Mat gray = convertToGray(input);
    Mat gray = input;
    Mat bw;
    
    adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

    
    Mat horizontal = getHorizontal(bw);
    Mat vertical = getVertical(bw);
    
    
    
    Mat edges;
    adaptiveThreshold(horizontal, edges, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    
    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    
    Mat smooth;
    vertical.copyTo(smooth);
    
    blur(smooth, smooth, Size(2, 2));
    
    smooth.copyTo(horizontal, edges);
    return horizontal;
}


void find_horizontal_lines(const char* filename, const char* output){
    Mat src = cv::imread(filename, 0);
    
    if(!src.data)
        cerr << "Problem loading image!!!" << endl;
    
    //Mat bw;
    //adaptiveThreshold(src.clone(), bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);

    Mat horizontal = prepareImage(src.clone());
    
    
    
    //Mat correctedAngle = correctAngle(src.clone(), horizontal.clone());
    
    // Mescla linhas apagadas com imagem com angulo corrigido
    addWeighted(src, 1.0, horizontal, 1.0, 0.0, horizontal);

    imwrite(output, horizontal);
    
}

int main(int argc, const char * argv[]) {
    Mat src = cv::imread(argv[1]);

    appy_k_means(src);
    //find_horizontal_lines(argv[1], argv[2]);
    return 0;
}
