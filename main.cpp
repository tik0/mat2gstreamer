#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define SHOW
#define DEVICE  0
// #define BGR2RGB
// #define SINK_UDP
#define SINK_SHM

int main(int argc, char *argv[]){
  VideoCapture cap;    // Camera
  VideoWriter writer;  // gstreamer
  cout << "Press ESC to quit" << endl;
   cout << getBuildInformation() << endl;
//    return 0;
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

  // GSTREAMER-1.0 SETUP
#ifdef SINK_UDP
  writer.open("appsrc ! rtpvrawpay ! udpsink host=localhost port=5000", 0, fps, cv::Size(width, height), true);
#endif
#ifdef SINK_SHM
  writer.open("appsrc ! shmsink socket-path=/tmp/foo sync=true wait-for-connection=false shm-size=10000000", 0, fps, cv::Size(width, height), true);
#endif
  if (!writer.isOpened()) {
    cap.release();
    cerr << "Can't create gstreamer writer. Do you have the correct version installed?" << endl;
    cerr << "Print out OpenCV build information" << endl;
    cout << getBuildInformation() << endl;
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
#ifdef BGR2RGB
      cvtColor(frame, frame, cv::COLOR_BGR2RGB);
#endif
      writer << frame;
#ifdef SHOW
      cv::imshow("Source image", frame);
#endif
    } catch (...) {
      cout << "Something went wrong" << endl;
      break;
    }
    key = cv::waitKey(1);
  }

  // CLEANUP
  writer.release();
  cap.release();
  return 0;
}
