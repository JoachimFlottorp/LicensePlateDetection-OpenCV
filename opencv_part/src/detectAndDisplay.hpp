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

#include "main.hpp"
#include "sql.hpp"

#define DEBUG_PRINT fprintf(stderr, "CHECKPOINT REACHED @  %s:%i\n", __FILE__, __LINE__);
#define BRIGHT_BLUE_SCALAR cv::Scalar(186, 82, 15)
#define BRIGHT_GREEN_SCALAR cv::Scalar(54, 182, 43)

struct word {
	std::string plate;
	bool query_confirmation = false;
};

class detectAndDisplay {
public:
	static bool main_loop(struct opencv_configuration &opencv_config, const bool is_picture, const bool is_camera, tesseract::TessBaseAPI& tess_api);
};

#endif // !DETECTANDDISPLAY_HPP