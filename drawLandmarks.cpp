#include "drawLandmarks.hpp"

void drawPolyline( Mat &im,const vector<Point2f> &landmarks,const int start,const int end,bool isClosed)
{
    // connecting all the points between the start and end index 
    vector <Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }

    // draw the poly lines
    polylines(im, points, isClosed, COLOR, 2, 16);
    
}

// draw the key points of the faces 
void drawLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    // Draw 68 points and outline on the face
    if (landmarks.size() == 68)
    {
      drawPolyline(im, landmarks, 0, 16, false);           // Jaw line
      drawPolyline(im, landmarks, 17, 21, false);          // Left eyebrow
      drawPolyline(im, landmarks, 22, 26, false);          // Right eyebrow
      drawPolyline(im, landmarks, 27, 30, false);          // Nose bridge
      drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
      drawPolyline(im, landmarks, 36, 41, true);    // Left eye
      drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
      drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
      drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
    }
    else 
    { 
		// if the number of the key points are smaller than 68, then we don't know which points belong to which facial parts and we draw a circle for each landmarks. 
		for(int i = 0; i < landmarks.size(); i++)
		{
			circle(im,landmarks[i],3, COLOR, FILLED);
		}
    }
    
}
