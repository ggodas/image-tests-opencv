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
#include "houghLinesTests.hpp"



using namespace std;
using namespace cv;



int main(int argc, const char * argv[]) {
    Mat src = cv::imread(argv[1]);
    
    tests(src);
    
    waitKey();
    
    return 0;
}
