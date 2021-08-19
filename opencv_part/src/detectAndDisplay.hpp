#ifndef DETECTANDDISPLAY_HPP
#define DETECTANDDISPLAY_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <filesystem>

#include "sql.hpp"
#include "word.hpp"
#include "query_plates.hpp"

#define DEBUG_PRINT fprintf(stderr, "CHECKPOINT REACHED @  %s:%i\n", __FILE__, __LINE__);
#define BRIGHT_BLUE_SCALAR cv::Scalar(186, 82, 15)
#define BRIGHT_GREEN_SCALAR cv::Scalar(54, 182, 43)


struct opencv_configuration {
	cv::CascadeClassifier licenseplate_classifier;
	cv::CascadeClassifier car_classifier;
	std::string img_arg;
	std::string vid_arg;
	std::string car_class_arg = "cars_cv.xml";
	std::string license_class_arg = "haarcascade_russian_plate_number.xml";
	int camera_arg;
};

class detectAndDisplay {
public:

	static bool main_loop(struct opencv_configuration &opencv_config, const bool is_picture, const bool is_camera, tesseract::TessBaseAPI& tess_api);


};

#endif // !DETECTANDDISPLAY_HPP