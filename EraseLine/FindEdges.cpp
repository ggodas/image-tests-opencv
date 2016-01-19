//
//  FindEdges.cpp
//  EraseLine
//
//  Created by George Godas on 1/19/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "FindEdges.hpp"

using namespace cv;


void appy_k_means(cv::Mat src){
    int lenght = (src.rows * src.cols);
    cv::Mat samples(lenght, 3, CV_32F);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int z = 0; z < 3; z++) {
                samples.at<cv::Vec3b>(x + y * src.cols, z, src.at<cv::Vec3b>(y, x, z));
            }
        }
    }
    
    int clusterCount = 2;
    cv::Mat labels;
    int attempts = 5;
    cv::Mat centers;
    
    cv::kmeans(samples, clusterCount, labels, TermCriteria(cv::TermCriteria::MAX_ITER +
                                                           cv::TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
    cv::imwrite("/Users/georgegodas/Downloads/bla.jpg", labels);
    cv::imshow("ver", centers);
    cv::waitKey();
    
}