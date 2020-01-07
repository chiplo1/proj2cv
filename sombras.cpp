#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(){


  Ptr<BackgroundSubtractor> pBackSub;
  pBackSub = createBackgroundSubtractorMOG2();

  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap("shadow.mp4");

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
  Mat frame, fgMask, fgMaskNoShad, teste, background;
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
    ss << cap.get(CAP_PROP_POS_FRAMES);
    string frameNumberString = ss.str();
    putText(frame, frameNumberString.c_str(), Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5 , Scalar(0,0,0));

    //Proximas 4 linhas apagam a sombra (parte cinza do fgMask)
    blur(fgMask, fgMaskNoShad, Size(15, 15), Point(-1, -1));
    threshold(fgMask, fgMaskNoShad, 128, 255, THRESH_BINARY);
    dilate(fgMaskNoShad, fgMaskNoShad, Mat(), Point(-1, -1), 2, 1, 1);
    erode(fgMaskNoShad, fgMaskNoShad, Mat(), Point(-1, -1), 2, 1, 1);

    frame.copyTo(teste, fgMaskNoShad);
    // Display the resulting frame
    if(flag){
		    background = frame;
		    flag = false;
	  }
    imshow( "Frame", frame );
    //imshow("FG Mask", fgMask);
    //imshow("FG Mask No Shad", fgMaskNoShad);
    imshow("teste", teste);
    imshow("imagem", background);

    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}
