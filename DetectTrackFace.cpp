
#include "DetectTrackFace.hpp"


void DetectTrackFace::createFacemark(){
		
	 faceDetector.load("../lbpcascade_frontalface.xml");
	// create facemark
	 facemark = FacemarkLBF::create();

	// load face detector model
	// source: https://github.com/kurnianggoro/GSOC2017
	facemark->loadModel("../lbfmodel.yaml");
	
}

void DetectTrackFace::loadGrafic(Mat gray, Mat dst)
{
	
	graypic=gray;
	dstpic=dst;
}
 
Mat DetectTrackFace::FaceTrack(Step &ste)
{
	    
		faceDetector.detectMultiScale(graypic, faces);
				
		// face marks
		vector< vector<Point2f> > landmarks;
		 
		// check whether face is detected 
		bool success = facemark->fit(dstpic,faces,landmarks);
		if(success)
		{	
			for(int i = 0; i < landmarks.size(); i++)
			{
				// draw face landmarks
				drawLandmarks(dstpic, landmarks[i]);
			}
			float sumx=0;//the sum of x coordinate of all the points of landmarks
			float sumy=0;// the sum of y coordinate of all the points of landmarks
			for(int i = 0; i < landmarks[0].size(); i++ )
			{
				sumx = sumx + landmarks[0][i].x;
				sumy = sumy + landmarks[0][i].y;
			}
			positionX = sumx / landmarks[0].size();// the average of x coordinate of all the points of landmarks
			positionY = sumy / landmarks[0].size();// the average of y coordinate of all the points of landmarks
			cout << "facex = "<<positionX<<endl;
			cout << "facey = "<<positionY<<endl;
			// the camera follows the detected first face so that the middle point of the face is in the middle of the camera 
			if (positionX < FACE_POSITION_MIDDLE_X - FACE_POSITION_THRESHOLD_X ){
				ste.step_down = 1;
				
			}
			else if (positionX > FACE_POSITION_MIDDLE_X + FACE_POSITION_THRESHOLD_X){
				ste.step_down = -1;
			}
			else 
			{
				ste.step_down = 0;
			}
			
			if (positionY < FACE_POSITION_MIDDLE_Y - FACE_POSITION_THRESHOLD_Y)
			{
				ste.step_up = -1;
			}
			else if (positionY > FACE_POSITION_MIDDLE_Y + FACE_POSITION_THRESHOLD_Y)
			{
				ste.step_up = 1 ;
			}
			else 
			{
				ste.step_up = 0;	
			}
			ste.control_active = true;
			
		}
		else 
		{
			cout<<"no face detected"<<endl;
			ste.control_active = false;
		}
		return dstpic;
		//imshow("face detection",dst);
		
		//if(waitKey(10)>=0)break;
		//delay(DELAY_TIME_MS);
	
}
