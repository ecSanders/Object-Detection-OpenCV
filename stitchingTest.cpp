#include <opencv2/opencv.hpp>
#include "opencv2/stitching.hpp"
#include <vector>
#include <iostream>
#include <chrono>

int stitch() {
	cv::Mat img1 = cv::imread("images\\stitch01_med.jpg");
	cv::Mat img2 = cv::imread("images\\stitch02_med.jpg");

	cv::Stitcher::Mode mode = cv::Stitcher::SCANS;
	std::vector<cv::Mat> imgs;

	imgs.push_back(img1);
	imgs.push_back(img2);

	cv::Mat output;

	if (img1.empty()) {
		std::cout << "Image was NULL\n";
		std::cout << "Image was NULL\n";
	}

	cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);

	auto start = std::chrono::high_resolution_clock::now();
	cv::Stitcher::Status status = stitcher->stitch(imgs, output);
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << duration.count() << std::endl;

	if (status != cv::Stitcher::OK)
	{
		std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
		return EXIT_FAILURE;
	}
	cv::imwrite("images/stitchResult.jpg", output);
	std::cout << "stitching completed successfully\n" << "images/stitchResult.jpg" << " saved!";

	cv::imshow("Stitch", output);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}