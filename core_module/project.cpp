#include "stdafx.h"

#include <experimental/filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>


namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[]) {

	// The cv::Mat class. OpenCV's basic data-type.

	// Here are some common ways to instantiate the cv::Mat class.
	cv::Mat A;
	cv::Mat B = cv::Mat(cv::Size(3, 3), CV_8UC1); //8 bit, unsigned, 1 channel.
	cv::Mat C = cv::Mat::ones(cv::Size(3, 3), CV_8UC3); //8 bit, unsigned, 3 channels.
	cv::Mat D = (cv::Mat_<int>(3, 3) << 
		1, 0, 0,
		0, 2, 0,
		0, 0, 3);

	std::cout << D << std::endl;

	// Here's some common ways to access a cv::Mat object's pixel at point (x,y) and channel [z].
	int x = 1;
	int y = 1;
	int c = C.at<cv::Vec3b>(cv::Point(x, y))[0]; // accessing an 8UC3 image.
	int d = D.at<int>(cv::Point(x, y)); // accessing an 8UC1 image. 

	std::cout << d << std::endl;

	// Here are some common operations that can be applied to cv::Mat objects.

	A = 255 * C;
	B = D / 0.1;

	std::cout << A << std::endl;

	// To copy or clone cv::Mat objects.

	C.copyTo(A); // Soft-copy (if you change A, C is also changed).
	B = D.clone(); // Hard-copy (if you change D, B is not changed).

	std::cout << B << std::endl;
	
	system("pause");

	return 0;

}