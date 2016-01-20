//
//  FindEdges.cpp
//  EraseLine
//
//  Created by George Godas on 1/19/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "FindEdges.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

double calcWhiteDistance(double r, double g, double b){
    return sqrt(pow(255 - r, 2) + pow(255 - g, 2) + pow(255 - b, 2));
}

Point findIntersetctions(Vec3b line1, Vec3b line2){
    double start_x1 = line1[0];
    double start_y1 = line1[1];
    double end_x1 = line1[2];
    double end_y1 = line1[3];

    double start_x2 = line2[0];
    double start_y2 = line2[1];
    double end_x2 = line2[2];
    double end_y2 = line2[3];

    double denominator = ((start_x1 - end_x1) * (start_y2 - end_y2)) - ((start_y1 - end_y2) * (start_x2 - end_x2));
    
    if (denominator != 0) {
        Point pt;
        pt.x = ((start_x1 * end_y1 - start_y1 * end_x1) * (start_x2 - end_x2) - (start_x1 - end_x1) * (start_x2 * end_y2 - start_y2 * end_x2)) / denominator;
        pt.y = ((start_x1 * end_y1 - start_y1 * end_x1) * (start_y2 - end_y2) - (start_y1 - end_y1) * (start_x2 * end_y2 - start_y2 * end_x2)) / denominator;
        return pt;
    } else{
        return Point(-1, -1);
    }
    
 
}

bool exists(vector<Point> corners, Point pt){
    for (int i = 0; i < corners.size(); i++) {
        int pow1 = pow(corners[i].x - pt.x, 2);
        int pow2 = pow(corners[i].y - pt.y, 2);
        if (sqrt(pow1 + pow2) < 10) {
            return true;
        }
    }
    return false;
}

void appy_k_means(cv::Mat src){
    
    int lenght = (src.rows * src.cols);
    cv::Mat samples(lenght, 3, CV_32F);
    cv::imwrite("/Users/georgegodas/Downloads/vazio.jpg", samples);

    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int z = 0; z < 3; z++) {
                //int b = src.at<uchar>(y, x, z);
                //samples.at<uchar>(x + y * src.cols, z) = b;
                Vec3b color = src.at<Vec3b>(Point(x,y));
                samples.at<cv::Vec3b>(Point(x + y * src.cols, z)) = color;
            }
        }
    }
    
 
    int clusterCount = 2;
    cv::Mat labels;
    int attempts = 5;
    cv::Mat centers(8, 1, CV_32FC1);
    
    cv::kmeans(samples, clusterCount, labels, TermCriteria(cv::TermCriteria::MAX_ITER +
                                                           cv::TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

    double distanceCenter0 = calcWhiteDistance(centers.at<uchar>(0, 0, 0), centers.at<uchar>(0, 1, 0), centers.at<uchar>(0, 2, 0));
    double distanceCenter1 = calcWhiteDistance(centers.at<uchar>(1, 0, 0), centers.at<uchar>(1, 1, 0), centers.at<uchar>(1, 2, 0));
    int paperCluster = (distanceCenter0 < distanceCenter1) ? 0 : 1;
    
    cv::Mat srcRes(src.size(), src.type());
    
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int cluster_idx = labels.at<uint>(x + y * src.cols, 0, 0);
            if (cluster_idx != paperCluster) {
                Vec6d vec(0, 0, 0, 255);
                srcRes.at<Vec6d>(Point(y, x)) = vec;
                //srcRes.at
                //srcRes.at<uint>(y, x, 0, 0, 0, 255);
            } else{
                Vec6d vec(255, 255, 255, 255);
                srcRes.at<Vec6d>(Point(y, x)) = vec;
            }
        }
    }
    
    
    cv::Mat srcGray;
    cv::Mat hierarchy;


    if (src.channels() == 3) {
        cv::cvtColor(src, srcGray, CV_BGR2GRAY);
    } else {
        srcGray = src;
    }
    cv::Canny(srcGray, srcGray, 50, 100);
    vector<vector<Point> > contours;

    cv::findContours(srcGray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    cv::imshow("contours", srcGray);

    
    
    int index = 0;
    double maxim = cv::contourArea(contours[0]);
    for (int contourIdx = 0; contourIdx < contours.size(); contourIdx++) {
        double tmp;
        tmp = cv::contourArea(contours[contourIdx]);
        if(maxim < tmp){
            maxim = tmp;
            index = contourIdx;
        }
    }
    
    cv::Mat drawing = cv::Mat::zeros(srcRes.size(), CV_8UC1);
    
    cv:drawContours(drawing, contours, -1, Scalar(255));
    
    cv::imshow("ver", drawing);
    cv::waitKey();

    
    //vector<Vec4i> lines;
    Mat lines;
    vector<Point> corners;
    
    HoughLinesP(drawing, lines, 1, M_PI/180, 70, 30, 10);
    
    for(int i = 0; i < lines.cols; i++){
        for (int j = i + 1; j < lines.cols; j++) {
            Vec3b line1 = lines.at<Vec3b>(Point(0, i));
            Vec3b line2 = lines.at<Vec3b>(Point(0, j));
            
            Point pt = findIntersetctions(line1, line2);
            if (pt.x > 0 && pt.y >= 0 && pt.x <= drawing.cols && pt.y <= drawing.rows) {
                if (!exists(corners, pt)) {
                    corners.push_back(pt);
                }
            }
        }
    }

    if(corners.size() != 4){
        char* errorMessage = "Nao detectei a borda";
    }
    
    
    
    cv::imwrite("/Users/georgegodas/Downloads/bla.jpg", drawing);
    cv::imshow("ver", drawing);
    cv::waitKey();
    
}

