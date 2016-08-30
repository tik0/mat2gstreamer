
# Debugging

- Set debug output for current executed application (http://docs.gstreamer.com/display/GstSDK/gst-launch):
- `export GST_DEBUG=2` : Like `-v`in the console
- `export GST_DEBUG=5 2>&1 | grep caps`: Get the capabilities of the pipeline
- `export GST_DEBUG=5` : Ultra verbose

# testsource

## gstreamer-0.10

- `gst-launch-0.10 videotestsrc ! video/x-raw-rgb,bpp=24,width=1000,height=1000 ! autoconvert ! ximagesink`
- `gst-launch-0.10 videotestsrc ! autoconvert ! ximagesink`
- `gst-launch-0.10 videotestsrc ! ximagesink`

## gstreamer-1.0

- `gst-launch-1.0 videotestsrc ! video/x-raw, format="I420" ,width=640,height=480,framerate=30/1 ! identity name=artoolkit sync=true ! autovideosink`

# Video Playback

Using H.264 testsamples from `http://jell.yfish.us/`: `http://jell.yfish.us/media/jellyfish-3-mbps-hd-h264.mkv` as `video.mkv`

## gstreamer-0.10

- `./simpleTest "filesrc location=video.mkv ! decodebin2 ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! fakesink"`
- `gst-launch-0.10 -v playbin uri=file:////path/video.mkv`
- `gst-launch filesrc location=video.mkv ! decodebin2 name=dec ! ffmpegcolorspace ! autovideosink`
- don't know why this does not work: `./simpleTest "playbin uri=file:////opt/repositories/artoolkit5/bin/video.mkv ! video/x-raw-rgb,bpp=24 ! fakesink"`
- Hints
  - http://stackoverflow.com/questions/1576750/how-to-display-avi-video-with-gstreamer
  - Using Ubuntu14.04, first the missing ffmpeg decode/encoder needs to be installed, because ffmpeg was replaced by libav (https://askubuntu.com/questions/575869/how-do-i-install-gstreamer0-10-ffmpeg-on-ubuntu-14-10/575888)
    - `sudo add-apt-repository ppa:mc3man/gstffmpeg-keep`
    - `sudo apt-get update`
    - `sudo apt-get install gstreamer0.10-ffmpeg`

## gstreamer-1.0

- `gst-launch-1.0 -v playbin uri=file:///path/to/somefile.mp4`
- `gst-launch-1.0 -v playbin uri=file:///path/video.mkv`
- `gst-launch-1.0 -v filesrc location=/path/video.mkv ! matroskademux ! avdec_h264 ! videoconvert ! autovideosink`
- Hints
  - GSTREAMER 1.0 Playback a file: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-playbin.html
  - matroska: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good-plugins/html/gst-plugins-good-plugins-matroskademux.html
  - RAW H264: http://blog.petehouston.com/2015/12/28/play-raw-h264-files-using-gstreamer/
  - RAW YUV: http://stackoverflow.com/questions/27419113/playing-a-raw-video-using-gst-launch

# Camera Playback

## gstreamer-0.10

- `gst-launch v4l2src ! xvimagesink`
- `gst-launch v4l2src ! video/x-raw-yuv,width=320,height=240,framerate=30/1 ! xvimagesink`
- `gst-launch v4l2src ! video/x-raw-yuv,format=\(fourcc\)YUY2,width=320,height=240 ! xvimagesink`

## gstreamer-1.0

- `gst-launch-1.0 v4l2src device="/dev/video0" ! video/x-raw,width=640,height=480 ! autovideosink`

# UDP Playback

## gstreamer-0.10

- UDP Theora
  - First start the client: `gst-launch-0.10 -v udpsrc port=5000 ! application/x-gdp ! gdpdepay ! video/x-theora ! theoradec ! video/x-raw-yuv,format=\(fourcc\)I420,width=352,height=288 ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=32,endianness=4321,red_mask=65280,green_mask=16711680,blue_mask=-16777216 ! autoconvert ! ximagesink`
  - Then start the server: `gst-launch-0.10 -v videotestsrc ! video/x-raw-yuv,format=\(fourcc\)I420,width=352,height=288 ! theoraenc ! video/x-theora ! gdppay ! application/x-gdp ! udpsink host=127.0.0.1 port=5000`
- RTP/UDP with testsource
  - Server: `gst-launch -v videotestsrc ! video/x-raw-rgb, framerate=25/1, width=100, height=100 ! rtpvrawpay ! udpsink host=localhost port=5000`
  - Client: `gst-launch-0.10 -v udpsrc port=5000 caps=" application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)RGB, depth=(string)8, width=(string)100, height=(string)100, colorimetry=(string)SMPTE240M, payload=(int)96, ssrc=(uint)3496538899, clock-base=(uint)2820015588, seqnum-base=(uint)5902" ! rtpvrawdepay ! autoconvert ! ximagesink`
- UDP with testsource
  - Server: `gst-launch -v videotestsrc ! video/x-raw-rgb, framerate=25/1, width=100, height=100 ! udpsink host=localhost port=5000`
  - Client: `gst-launch-0.10 -v udpsrc port=5000 caps=" application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)BGR, depth=(string)8, width=(string)640, height=(string)480, framerate=30/1" ! rtpvrawdepay ! autoconvert ! ximagesink`
  - Hint
    - in the UDP case (without RTP fragmentation) the packages can be only as big as one TCP frame

## gstreamer-1.0

# ARtoolkit

NOTE:
The `identity` plugin just pipes the source to the sink as it is.
But it has the feature, the any program (here it is ARToolkit) which calls this gstreamer pipeline, can grab the data via the given `name`

## testsource

### gstreamer-0.10

- ./simpleTest "videotestsrc ! video/x-raw-rgb,bpp=24,width=1000,height=1000 ! identity name=artoolkit sync=true ! fakesink"

### gstreamer-1.0

- ./simpleTest "videotestsrc ! video/x-raw, format=RGB,width=640,height=480,framerate=30/1 ! identity name=artoolkit sync=true ! fakesink"

## Video

### gstreamer-0.10

## Camera

- ./simpleTest -device=GStreamer  "v4l2src ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink"

## UDP

### gstreamer-0.10

- testsource
  - SERVER: `gst-launch -v videotestsrc ! video/x-raw-rgb, framerate=25/1, width=100, height=100 ! udpsink host=localhost port=5000`
  - CLIENT: `./simpleTest -device=GStreamer  'udpsrc port=5000 caps="video/x-raw-rgb, framerate=(fraction)25/1, width=(int)100, height=(int)100, bpp=(int)32, endianness=(int)4321, depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255" ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink'`
- camer
  Client: `simpleTest: ./simpleTest -device=GStreamer  'udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)BGR, depth=(string)8, width=(string)640, height=(string)480, framerate=30/1" ! rtpvrawdepay ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink'`

Non-WORKING SETUP WITH UDP STREAM from OpenCV Webcam:
- ./simpleTest -device=GStreamer  'udpsrc port=5000 caps="video/x-raw-rgb, framerate=(fraction)30/1, width=(int)640, height=(int)480, bpp=(int)24, endianness=(int)4321, depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255" ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink'

###  gstreamer-1.0

- gst-launch-1.0 -v udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)BGR, depth=(string)8, width=(string)640, height=(string)480, colorimetry=(string)SMPTE240M, payload=(int)96, ssrc=(uint)1978155436, timestamp-offset=(uint)3180518504, seqnum-offset=(uint)28674" ! rtpvrawdepay ! videoconvert ! ximagesink



#### Video

### gstreamer-1.0

#### Testsource

- gst-launch-1.0 -v videotestsrc ! video/x-raw, format=BGR, framerate=25/1, width=100, height=100 ! rtpvrawpay ! udpsink host=localhost port=5000
- ./simpleTest -device=GStreamer 'udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=(string)BGR, depth=(string)8, width=(string)100, height=(string)100, colorimetry=(string)SMPTE240M, payload=(int)96, ssrc=(uint)1744359127, timestamp-offset=(uint)1879147832, seqnum-offset=(uint)3344" ! rtpvrawdepay ! videoconvert video/x-raw, format=RGB ,bpp=24 ! identity name=artoolkit sync=true ! fakesink'

# Other stuff

Building test applications: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/pwg/html/chapter-building-testapp.html
