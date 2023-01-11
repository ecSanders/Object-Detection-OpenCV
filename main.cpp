#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>

bool displayImage(std::string& path) {
	cv::Mat img = cv::imread(path);

	// Check image loading process
	if (img.empty()) {
		std::cout << "Image failed to load\n";
		std::cin.get();
		return false;
	}
	else {
		std::cout << "Image successfully loaded\n";
	}

	std::string windowName = "Nathan Birch";

	// Create window
	cv::namedWindow(windowName);
	cv::imshow(windowName, img);

	// Kill window
	cv::waitKey(0);
	cv::destroyWindow(windowName);

	return true;
}

void testImageStream() {
	cv::Mat img;
	std::string windowName = "Video Feed";

	cv::namedWindow(windowName);
	cv::VideoCapture vStream(0);

	if (!vStream.isOpened()) {
		std::cout << "Failed to open video stream.\n";
	}
	
	while (true) {
		vStream >> img;

		cv::imshow(windowName, img);
		cv::waitKey(25);	
	}

}

int main() {
	std::string path = "images\\nathan-birch.png";
	
	displayImage(path);
	testImageStream();

	return 0;
}