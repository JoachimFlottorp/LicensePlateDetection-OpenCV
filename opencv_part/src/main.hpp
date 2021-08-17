#ifndef MAIN_HPP
#define MAIN_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include "detectAndDisplay.hpp"
#include "sql.hpp"

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <utility>

struct opencv_configuration {
	cv::CascadeClassifier licenseplate_classifier;
	cv::CascadeClassifier car_classifier;
	std::string img_arg;
	std::string vid_arg;
	std::string car_class_arg = "cars_cv.xml";
	std::string license_class_arg = "haarcascade_russian_plate_number.xml";
	int camera_arg;
};





#endif // !MAIN_HPP