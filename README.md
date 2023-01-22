# Overview

> This is a first time attempt at object detection in C++. The main file is where the object detection is written. Within main it loads a YOLOv5 nano model using `cv::dnn::readNet(...)`. Then runs inference on either a single image or on video feed, the user is given the option to pick. Once the inference is run, it maps the bounding boxes on the feed/input and then displays it. 

> I wrote this software to improve my general C++ skills and learn more about object detection inside this language. I learned about NMS, object detection workflow, some new OpenCV functions, and how to work with video feed.



[Object Detection in C++](https://www.loom.com/share/3baf60891ba14b4a9c1a75cb7e7dc219)

# Development Environment

This project used Visual Studio and OpenCV to complete.

# Useful Websites

- [Model Repository](https://github.com/doleron/yolov5-opencv-cpp-python/blob/main/cpp/yolo.cpp)
- [Empty Image CV](https://stackoverflow.com/questions/25587912/unhandled-exception-microsoft-c-exception-cvexception-at-memory-location)
- [Medium Article](https://medium.com/mlearning-ai/detecting-objects-with-yolov5-opencv-python-and-c-c7cf13d1483c)


# Future Work

These items need to be fixed in future updates.

- Video feed needs to update overtime.
- Implement this into command line arguments.
- Add more comments to code explaining each step.
