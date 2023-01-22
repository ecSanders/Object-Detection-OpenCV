/*******************************************************************************************************************************
* Author: Erik Sanders
* Date: January 21, 2023
* File: main.cpp
* 
* NOTE: This project was heavily influenced by the 
* https://github.com/spmallick/learnopencv/blob/master/Object-Detection-using-YOLOv5-and-OpenCV-DNN-in-CPP-and-Python/yolov5.cpp
* repository. I used their work as a template as well as others. Many of the same steps are taken in this repository as others
*
*********************************************************************************************************************************/


#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2;
const float NMS_THRESHOLD = 0.4;
const float CONFIDENCE_THRESHOLD = 0.4;

struct Bbox{
    int confidence;
    int classID;
    cv::Rect bbox;
};

void drawLabel(cv::Mat& input, std::string classNames, int left, int top){
    // Display the label at the top of the bounding box.
    int baseLine;
    cv::Size label_size = cv::getTextSize(classNames, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);
    top = cv::max(top, label_size.height);

    cv::Point tlc = cv::Point(left, top);
    cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);

    // Draw black rectangle and write on it
    cv::rectangle(input, tlc, brc, cv::Scalar(0,0,0), cv::FILLED);
    cv::putText(input, classNames, cv::Point(left, top + label_size.height), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 1);
}

cv::Mat augmentImage(const cv::Mat& source) {
    int col = source.cols;
    int row = source.rows;
    int _max = MAX(col, row);
    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(cv::Rect(0, 0, col, row)));
    return result;
}

void getPredictions(cv::Mat &image, cv::dnn::Net &net, std::vector<Bbox> &output, const std::vector<std::string> &classNames) {
    cv::Mat blob;
    cv::Mat augmentedImage = augmentImage(image);

    cv::dnn::blobFromImage(augmentedImage, blob, 1.0 / 255.0, cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);
    net.setInput(blob);
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    float x_factor = image.cols / INPUT_WIDTH;
    float y_factor = image.rows / INPUT_HEIGHT;

    float* data = (float*)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; i++) {

        float confidence = data[4];

        if (confidence >= CONFIDENCE_THRESHOLD) {
            float* classes_scores = data + 5;
            cv::Mat scores(1, classNames.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

            if (max_class_score > SCORE_THRESHOLD) {

                confidences.push_back(confidence);

                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];
                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                boxes.push_back(cv::Rect(left, top, width, height));
            }

        }

        data += 85;

    }

    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, nms_result);
    for (int i = 0; i < nms_result.size(); i++) {
        int idx = nms_result[i];
        Bbox result;
        result.classID = class_ids[idx];
        result.confidence = confidences[idx];
        result.bbox = boxes[idx];
        output.push_back(result);
    }

     
    }


int main() {
    int id = 0;
    std::cout << "Video(1)/ Image(0): ";
    std::cin >> id;

    // Load weights and class names
	cv::dnn::Net net = cv::dnn::readNet("models/yolov5n.onnx");
    std::vector<std::string> classNames;
    std::ifstream ifs("COCO Names.txt");
    std::string line;

    // Initialize a vector of predictions
    std::vector<Bbox> predictions;
    cv::Mat input;

    while (std::getline(ifs, line)) {
        classNames.push_back(line);
    }


    // Run on image
    if (id == 0) {
        // Load test image
        std::string windowName = "Test Image";
        std::string img = "images\\nathan-birch.png";
        input = cv::imread(img);

        if (input.empty()) {
            std::cerr << "Image is empty\n";
            return -1;
        }

        getPredictions(input, net, predictions, classNames);

        int numBoxes = predictions.size();
        
        for (int i = 0; i < numBoxes; i++){
            auto detection = predictions[i];
            auto box = detection.bbox;
            auto classId = detection.classID;
            const auto color = cv::Scalar(255,255,0);
            cv::rectangle(input, box, color, 3);

            cv::rectangle(input, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color, cv::FILLED);
            cv::putText(input, classNames[classId].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

        }

        cv::imshow(windowName, input);
        cv::waitKey(0);
        cv::destroyWindow(windowName);

    }
    // Run on video feed
    else {
        std::string windowName = "Video Feed";
        cv::VideoCapture vStream(0);

        if (!vStream.isOpened()) {
            std::cerr << "Failed to run video feed\n";
            return -1;
        }

        while (true) {
            vStream.read(input);

            if (input.empty()) {
                std::cout << "End of stream\n";
                break;
            }

            getPredictions(input, net, predictions, classNames);

            int numBoxes = predictions.size();

            for (int i = 0; i < numBoxes; i++) {
                auto detection = predictions[i];
                auto box = detection.bbox;
                auto classId = detection.classID;
                const auto color = cv::Scalar(255, 255, 0);
                cv::rectangle(input, box, color, 3);

                cv::rectangle(input, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color, cv::FILLED);
                cv::putText(input, classNames[classId].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

            }

            cv::imshow(windowName, input);

            if (cv::waitKey(1) != -1) {
                vStream.release();
                std::cout << "finished by user\n";
                break;
            }
        }
    }



	return 0;
}