//
//  prepareImage.cpp
//  EraseLine
//
//  Created by George Godas on 1/27/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "prepareImage.hpp"

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

Mat prepare(Mat input, int blockSize, double C ){
    //Mat gray = convertToGray(input);
    Mat gray = input;
    Mat bw;
    
    adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, blockSize, C);
    
    
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


