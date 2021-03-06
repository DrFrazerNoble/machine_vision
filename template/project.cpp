#include "stdafx.h"

#include <experimental\filesystem>
#include <opencv2\opencv.hpp>

// Here, we use a "shortcut" to the filesystem namespace.
namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{

	// Using the filesystem's current_path(), we can get our program's current working directory (and convert it to a string).
	std::string current_directory = fs::current_path().string();

	// Using OpenCV's imread(), we can open an image. Here, we open a file from the current directory's data subdirectory.
	cv::Mat image = cv::imread(current_directory + "/data/data.jpg");

	///	Defines the minimum hue value to search from in the HSV color space. 
	int min_hue = 0;

	/// Defines the maximum hue value to search up to in the HSV color space.
	int max_hue = 180;

	// Using OpenCV's fileStorage class, we can read an XML file created using OpenCV. Here, we're trying to open an existing file, while checking if it exsits.
	cv::FileStorage file;

	if (file.open(current_directory + "/data/settings.xml", cv::FileStorage::READ)) {

		file["min_hue"] >> min_hue;
		file["max_hue"] >> max_hue;

		file.release();
	}

	// Using OpenCV's namedWindow(), we can create a window, which can be used to display an image.
	cv::namedWindow("Color Detection", CV_WINDOW_AUTOSIZE);

	// Using OpenCV's createTrackbar(), we can create slider bars, which can interactively change a variable's value.
	cv::createTrackbar("Minimum Hue", "Color Detection", &min_hue, 179, NULL);
	cv::createTrackbar("Maximum Hue", "Color Detection", &max_hue, 179, NULL);

	cv::Mat image_hsv;

	// Using OpenCV's cvtColor(), we can convert an image from the RGB (or in OpenCV's case, BGR) color space into the HSV color space.
	cv::cvtColor(image, image_hsv, CV_BGR2HSV);

	cv::Mat threshold;
	cv::Mat upper_threshold;
	cv::Mat lower_threshold;

	while (true) {

		// Using OpenCV's inRange(), we can search for pixels, which are in the range defined by the lower bound and upper bound. Here, the hue values used are the ones defined using the min_hue and max_hue variables' vlaues (which in turn are set using the trackbars created earlier).
		cv::inRange(image_hsv, cv::Scalar(min_hue, 25, 25), cv::Scalar(max_hue, 255, 255), upper_threshold);
		cv::inRange(image_hsv, cv::Scalar(180-max_hue, 25, 25), cv::Scalar(180, 255, 255), lower_threshold);

		// Since red in HSV is, roughly, between 160-180 and 0-20, we can detect hues in two regions, then add the results together to get a single thresholded image.
		cv::addWeighted(lower_threshold, 1.0, upper_threshold, 1.0, 0.0, threshold);

		// Contours are detected in the thresholded image.
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;

		// Retrieves all of the contours and reconstructs a full hierarchy of nested contours. 
		//Compresses horizontal, vertical, and diagonal segments and leaves only their end points. For example, an up-right rectangular contour is encoded with 4 points.
		cv::findContours(threshold, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

		cv::Mat drawing = image.clone();
		
		for (int i = 0; i < contours.size(); i++) {
			cv::drawContours(drawing, contours, i, cv::Scalar(255, 0, 0), 3);

			cv::Rect box = cv::boundingRect(contours[i]);
			cv::rectangle(drawing, box, cv::Scalar(0, 255, 0), 3);

			std::cout << box << std::endl;
		}

		cv::imshow("Contours", drawing);

		// Using OpenCV's imshow(), an image can be displayed (either in a new window, or in a window created using namedWindow()).
		cv::imshow("Threshold", threshold);

		// Using OpenCV's waitKey(), we can wait t ms for user input.
		char c = cv::waitKey(1);
		if (c == 27) {

			break;
		}
	}

	// Using OpenCV's imwrite(), we can write an image to a directory. Here, we write a file to the current directory's data subdirectory.
	cv::imwrite(current_directory + "/data/thresholded.jpeg", threshold);

	// Using OpenCV's fileStorage class, we can create an XML file.
	file.open(current_directory + "/data/settings.xml", cv::FileStorage::WRITE);

	file << "min_hue" << min_hue;
	file << "max_hue" << max_hue;

	// It's good practice to clean up at the end of our program, e.g. by using OpenCV's destroyAllWindows() or release().
	cv::destroyAllWindows();

	return 0;
}

