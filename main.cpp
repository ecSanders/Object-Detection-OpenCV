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

int main() {
	std::string path = "images\\nathan-birch.png";
	
	displayImage(path);

	return 0;
}