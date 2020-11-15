
#include "DetectTrackFace.hpp"

void DetectTrackFace::createFacemark(){	
	// load lbpcascade_frontalface.xml to detect face, load haarcascade_fullbody to detect full human body	
	faceDetector.load("../lbpcascade_frontalface.xml");
	//faceDetector.load("../haarcascade_fullbody.xml");
	// create facemark
	facemark = FacemarkLBF::create();
	// load face detector model
	// source: https://github.com/kurnianggoro/GSOC2017
	facemark->loadModel("../lbfmodel.yaml");
}

void DetectTrackFace::createFullBodyDetector(){	
	// load haarcascade_fullbody to detect full human body	
	//cout << "start to load model haarcascade_fullbody.xml" <<endl;
	faceDetector.load("../haarcascade_upperbody.xml");
	cout << "load haarcascade_fullbody.xml finish" <<endl;
}

Mat DetectTrackFace::detectBody(Step &ste, bool bodyTrackEnabled, bool &bodydetected){
	faceDetector.detectMultiScale(graypic, bodies, 1.1, 3);

	if (bodies.size() > 0)
	{   bodydetected = true;
		for (int gg = 0; gg < bodies.size(); gg++)
		{
			rectangle(dstpic, bodies[gg].tl(), bodies[gg].br(), Scalar(0, 0, 255), 2, 8, 0);
		}

		if (bodyTrackEnabled){
			positionX = bodies[0].x + bodies[0].width / 2.0;
			positionY = bodies[0].y + bodies[0].height / 2.0;

			cout << "positionX: " << positionX << endl;
			cout << "positionY: " << positionY << endl;

			cout << "FACE_POSITION_MIDDLE_X: " << FACE_POSITION_MIDDLE_X << endl;
			cout << "FACE_POSITION_MIDDLE_Y: " << FACE_POSITION_MIDDLE_Y << endl;

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

	}
	else {
		ste.control_active = false;
		bodydetected = false;
		
	}

	return dstpic;
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
}
