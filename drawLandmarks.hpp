// Summary:  draw Landmarks and polylines
// Author:  Yanxia
// Date:    2020-01-02

#ifndef drawLandmarks_hpp_
#define drawLandmarks_hpp_

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv; 
using namespace std; 

#define COLOR Scalar(255, 200,0)

// draw the poly line by connecting the start points and the end points 
void drawPolyline( Mat &im,const vector<Point2f> &landmarks,const int start,const int end,bool isClosed);


// draw the key points of the faces 
void drawLandmarks(Mat &im, vector<Point2f> &landmarks);


#endif // drawLandmarks_hpp_
