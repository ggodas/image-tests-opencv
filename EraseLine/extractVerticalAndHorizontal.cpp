//
//  extractVerticalAndHorizontal.cpp
//  EraseLine
//
//  Created by George Godas on 1/27/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "extractVerticalAndHorizontal.hpp"


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

