#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(){


  Ptr<BackgroundSubtractor> pBackSub;
  pBackSub = createBackgroundSubtractorMOG2();

  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap(0);

  // Check if camera opened successfully
  if(!cap.open(0)){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
  Mat frame, fgMask, fgMaskNoShad, writeFrame, show, background;
  
	int choice;
	choice = 1;
	
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
	
	VideoWriter video("output.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));
	
	namedWindow("Frame", 1);
	
	int iSliderValue1 = 50;
     createTrackbar("Brightness", "Frame", &iSliderValue1, 100);

     //Create trackbar to change contrast
     int iSliderValue2 = 50;
     createTrackbar("Contrast", "Frame", &iSliderValue2, 100);
	
	bool flag = true;
	
  while(1){
    // Capture frame-by-frame
    cap >> frame;
    // If the frame is empty, break immediately
    if (frame.empty())
      break;
    pBackSub->apply(frame, fgMask);
    rectangle(frame, Point(10, 2), Point(100, 2), Scalar(255, 255, 255), -1);
	
    
	stringstream ss;
    ss << cap.get(CV_CAP_PROP_FPS);
    string frameNumberString = ss.str();
    putText(frame, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5 , Scalar(0,0,0));
	
	
    //Proximas 4 linhas apagam a sombra (parte cinza do fgMask)
    blur(fgMask, fgMaskNoShad, Size(15, 15), Point(-1, -1));
    threshold(fgMask, fgMaskNoShad, 128, 255, THRESH_BINARY);
    dilate(fgMaskNoShad, fgMaskNoShad, Mat(), Point(-1, -1), 2, 1, 1);
    erode(fgMaskNoShad, fgMaskNoShad, Mat(), Point(-1, -1), 2, 1, 1);
    // Display the resulting frame
	
	
	
	
	switch(choice) {
		case 1:
			//imshow( "Frame", dst );
			writeFrame = frame;
			break;
		case 2:
			//imshow("Frame", fgMask);
			writeFrame = fgMask;
			break;
		case 3:
			//imshow("Frame", fgMaskNoShad);
			writeFrame = fgMaskNoShad;
			break;
	}
	
	Mat dst;
	int iBrightness  = iSliderValue1 - 50;
	double dContrast = iSliderValue2 / 50.0;
	
	writeFrame.convertTo(dst, -1, dContrast, iBrightness); 
	
	if(flag){
		background = dst;
		flag = false;
	}
	
	imshow( "Frame", dst );
	imshow( "Frame2", background );
	
	//printf("%d\n",writeFrame.channels());
	video.write(dst);
	
    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
	if(c==49 && choice != 1){
		//destroyAllWindows();
		choice = 1;
	}
	if(c==50 && choice != 2){
		//destroyAllWindows();
		choice = 2;
	}
	if(c==51 && choice != 3){
		//destroyAllWindows();
		choice = 3;
	}
	
  }

  // When everything done, release the video capture object
  cap.release();
  video.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}