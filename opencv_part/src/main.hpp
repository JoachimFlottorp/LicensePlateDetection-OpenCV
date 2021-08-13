#ifndef MAIN_HPP
#define MAIN_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <mysql/mysql.h>

#include "detectAndDisplay.hpp"
#include "sql.hpp"

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <utility>

MYSQL* conn;

cv::CascadeClassifier g_licenseplate_lp_classifier;
cv::CascadeClassifier g_car_lp_classifier;
std::string g_img_arg;
std::string g_vid_arg;
std::string g_car_class_arg = "cars_cv.xml";
std::string g_license_class_arg = "haarcascade_russian_plate_number.xml";

int g_camera_arg;

#endif // !MAIN_HPP