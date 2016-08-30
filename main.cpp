#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define SHOW   true
#define DEVICE 0

int main(int argc, char *argv[]){
  VideoCapture cap;    // Camera
  VideoWriter writer;  // gstreamer
  cout << "Press ESC to quit" << endl;

  // CAMERA SETUP
  cap.open(DEVICE);
  if(!cap.isOpened()) {
    cerr << "Can't create camera reader" << endl;
    return -1;
  }

  const double fps = cap.get(CAP_PROP_FPS);
  const int width  = cap.get(CAP_PROP_FRAME_WIDTH);
  const int height = cap.get(CAP_PROP_FRAME_HEIGHT);
  cout << "Capture camera with " << fps << " fps, " << width << "x" << height << " px" << endl;

  // GSTREAMER SETUP
  writer.open("appsrc ! rtpvrawpay ! udpsink host=localhost port=5000", 0, fps, cv::Size(width, height), true);
  if (!writer.isOpened()) {
    cap.release();
    cerr << "Can't create gstreamer writer" << endl;
//    cout << getBuildInformation() << endl;
    return -1;
  }

  cv::Mat frame;
  int key = -1, cnt = 0;

  while (key != 27) { // 27 = ascii value of ESC
    try {
      cap >> frame;
      if (frame.empty()) {
        throw;
      }
      writer << frame;
//       cv::imshow("Source image", frame);
      key = cv::waitKey(1);
    } catch (...) {
      cout << "Something went wrong" << endl;
      break;
    }
  }

  // CLEANUP
  writer.release();
  cap.release();
  return 0;
}
