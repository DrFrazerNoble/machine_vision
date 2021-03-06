#include "stdafx.h"

#include <experimental\filesystem>
#include <opencv2\opencv.hpp>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{

	//In the first half of this program, we'll look at properties associated with a camera; in the second half, we'll look at properties associated with videos.

	// Here, we open the default device using an overloaded constructor for a VideoCapture object. 0 for the default, 1 for the next device, etc. Try -1; in the past, I've been able to select a camera from a drop down list.
	cv::VideoCapture camera(0);

	// If camera did not open, e.g. there's no default device, then exit the program.
	if (!camera.isOpened()) {
		return -1;
	}

	// Here, we'll look at some common properties. Note, not all cameras support all the available options. I've had to do some experimentation to find out which properties can be changed for my given cameras, e.g. Logitech c930.
	int brightness = static_cast<int>(camera.get(CV_CAP_PROP_BRIGHTNESS));
	int contrast = static_cast<int>(camera.get(CV_CAP_PROP_BRIGHTNESS));
	int saturation = static_cast<int>(camera.get(CV_CAP_PROP_SATURATION));
	//int hue = static_cast<int>(camera.get(CV_CAP_PROP_HUE)); //logitech's c930 doesn't support this option.
	int gain = static_cast<int>(camera.get(CV_CAP_PROP_GAIN));
	int exposure = static_cast<int>(camera.get(CV_CAP_PROP_EXPOSURE));
	int auto_focus = static_cast<int>(camera.get(CV_CAP_PROP_AUTOFOCUS));
	int focus = static_cast<int>(camera.get(CV_CAP_PROP_FOCUS));
	int width = static_cast<int>(camera.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(camera.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Here, the initial values are displayed on the console.
	std::cout << "Brightness: " << brightness << std::endl;
	std::cout << "Contrast: " << contrast << std::endl;
	std::cout << "Saturation: " << saturation << std::endl;
	//std::cout << "Hue: " << hue << std::endl;
	std::cout << "Gain: " << gain << std::endl;
	std::cout << "Exposure: " << exposure << std::endl;
	//std::cout << "Auto-focus: " << auto_focus << std::endl;
	std::cout << "Focus: " << focus << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;

	// Because we want to see the effects of changing properties using trackbars, we'll need a window to draw them in.
	cv::namedWindow("Settings", CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("Brightness", "Settings", &brightness, 256, NULL);
	cv::createTrackbar("Contrast", "Settings", &contrast, 256, NULL);
	cv::createTrackbar("Saturation", "Settings", &saturation, 256, NULL);
	//cv::createTrackbar("Hue", "Settings", &hue, 256, NULL);
	cv::createTrackbar("Gain", "Settings", &gain, 256, NULL);
	cv::createTrackbar("Exposure", "Settings", &exposure, 256, NULL);
	//cv::createTrackbar("Auto-focus", "Settings", &auto_focus, 1, NULL);
	cv::createTrackbar("Focus", "Settings", &focus, 256, NULL);
	//cv::createTrackbar("Width", "Settings", &width, 1920, NULL); //don't want to be changing resolution continuously.
	//cv::createTrackbar("Height", "Settings", &height, 1080, NULL);

	camera.set(CV_CAP_PROP_FRAME_WIDTH, static_cast<double>(1280));
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, static_cast<double>(720));

	// Here, we loop continuously, displaying an image taken from the camera. Alternatively, we could create individual callback functions for each trackbar.
	while (true) {

		cv::Mat frame;

		camera >> frame;
		if (frame.empty()) {
			continue;
		}

		cv::imshow("Frame", frame);

		camera.set(CV_CAP_PROP_BRIGHTNESS, static_cast<double>(brightness));
		camera.set(CV_CAP_PROP_CONTRAST, static_cast<double>(contrast));
		camera.set(CV_CAP_PROP_SATURATION, static_cast<double>(saturation));
		//camera.set(CV_CAP_PROP_HUE, static_cast<double>(hue));
		camera.set(CV_CAP_PROP_GAIN, static_cast<double>(gain));
		camera.set(CV_CAP_PROP_EXPOSURE, static_cast<double>(exposure));
		//camera.set(CV_CAP_PROP_AUTOFOCUS, static_cast<double>(0));
		camera.set(CV_CAP_PROP_FOCUS, static_cast<double>(focus));
		//camera.set(CV_CAP_PROP_FRAME_WIDTH, static_cast<double>(width));
		//camera.set(CV_CAP_PROP_FRAME_HEIGHT, static_cast<double>(height));
		
		// If the ESC key is pressed, break out of the loop and end this half of the program.
		char c = cv::waitKey(1);
		if (c == 27) {
			break;
		}
	}

	camera.release();

	cv::destroyAllWindows();

	// Here, we'll look at the second part of the program.

	// I've included a video in the project's sub-directory, data, called "video.mp4". It's one I downloaded from YouTube.
	std::string current_path = fs::current_path().string();

	// Before, we opened a VideoCapture device by overloading an instance's constructor; here, we're using the open method.
	cv::VideoCapture video;

	video.open(current_path + "/data/video.mp4");

	// If the video can't be opened, then close the program.
	if (!video.isOpened()) {
		return -1;
	}

	// Here, we get some common properties for a video. We'll need the FPS, at least, in order to display the frames at the correct speeds.
	int current_frame = 0;
	int frame_count = static_cast<int>(video.get(CV_CAP_PROP_FRAME_COUNT));
	int frame_width = static_cast<int>(video.get(CV_CAP_PROP_FRAME_WIDTH));
	int frame_height = static_cast<int>(video.get(CV_CAP_PROP_FRAME_HEIGHT));
	int fps = static_cast<int>(video.get(CV_CAP_PROP_FPS));

	std::cout << std::endl;
	std::cout << "Frame count: " << frame_count << std::endl;
	std::cout << "Frame width: " << frame_width << std::endl;
	std::cout << "Frame height: " << frame_height << std::endl;
	std::cout << "FPS: " << fps << std::endl;

	while (true) {
		
		cv::Mat frame;

		video >> frame;
		
		if (frame.empty()) {
			continue;
		}
					
		cv::imshow("Frame", frame);

		current_frame = static_cast<int>(video.get(CV_CAP_PROP_POS_FRAMES));
		std::cout << current_frame << std::endl;

		// Here, we delay the video by 1000ms/fps and break if the user presses the ESC key.
		char c = cv::waitKey(1000/fps);
		if (c == 27) {
			break;
		}
	}

	// Here, we tidy up; releasing any VideoCapture objects and closing any open windows.

	video.release();

	cv::destroyAllWindows();

	return 0;
}

