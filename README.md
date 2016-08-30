# mat2gstreamer
Open camera by OpenCV and send it to UDP/RTP

- Preliminars for Ubuntu 14.04
  - Install gstreamer1.0: `sudo apt-get install libgstreamer1.0* gstreamer1.0* libgstreamer0.10* gstreamer0.10*``
  - Build opencv without gstreamer 0.10 support: `-D WITH_GSTREAMER=ON -D WITH_GSTREAMER_0_10=OFF`
- Configure the source code: Maybe you have another cameradevice, etc.
- Build: `cmake . && make`
- Run a standalone gstreamer `udtsrc` and display the video strea (sampling, depth, width, and height might differ): `gst-launch-1.0 -v udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)BGR, depth=(string)8, width=(string)640, height=(string)480" ! rtpvrawdepay ! videoconvert ! ximagesink`
- Notes
  - If the caps string is not correct, just open up a gstreamer `udpsink` for you webcam and investigate the `caps` section of the verbose output. This string needs to be put into the `caps` section of the `udpsrc`
  - Do not use gstreamer0.10

## Hints

- Webcam example: `gst-launch-1.0 v4l2src ! xvimagesink`
- Get debug messages of `gst-launch-1.0` with argument `--gst-debug-level=5`
- List available plugins: `gst-inspect-1.0`
- List features of a plugin: `gst-inspect-1.0 <PLUGIN_NAME>`
- videoconvert feature:
  - https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-videoconvert.html
  - This will output a test video (generated in YUY2 format) in a video window. If the video sink selected does not support YUY2 videoconvert will automatically convert the video to a format understood by the video sink: `gst-launch-1.0 -v videotestsrc ! video/x-raw,format=YUY2 ! videoconvert ! autovideosink`
- Plugin description: https://gstreamer.freedesktop.org/documentation/

## Links
- HOT: http://www.einarsundgren.se/gstreamer-basic-real-time-streaming-tutorial/
- http://wiki.oz9aec.net/index.php/Gstreamer_cheat_sheet
- http://labs.isee.biz/index.php/Example_GStreamer_Pipelines
- http://processors.wiki.ti.com/index.php/Example_GStreamer_Pipelines
- https://en.wikipedia.org/wiki/GStreamer
- https://wiki.ubuntuusers.de/Gstreamer-1.0/
- http://stackoverflow.com/questions/37339184/how-to-write-opencv-mat-to-gstreamer-pipeline
- https://www.youtube.com/watch?v=ShAuAgjwCds
- http://stackoverflow.com/questions/23570572/using-custom-camera-in-opencv-via-gstreamer
