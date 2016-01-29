//
//  prepareImage.hpp
//  EraseLine
//
//  Created by George Godas on 1/27/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#ifndef prepareImage_hpp
#define prepareImage_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "extractVerticalAndHorizontal.hpp"

using namespace cv;

#endif /* prepareImage_hpp */


Mat convertToGray(Mat image);
Mat prepare(Mat input, int blockSize, double C );
