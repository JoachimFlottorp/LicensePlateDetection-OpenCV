#include "detectAndDisplay.hpp"
#define DEBUG

// https://setosa.io/ev/image-kernels/
cv::Mat kernel = (cv::Mat_<double>(3, 3) << 0, -0.5, 0,
											-0.5, 3, 0.5,
											0, -0.5, 0);
// https://stackoverflow.com/a/26221725
std::string float_format(const char* format, float& conf) {
	int size_s = std::snprintf(nullptr, 0, format, conf) + 1;
	if (size_s <= 0) {return "00.00"; };
	size_t size = static_cast<size_t>(size_s);
	auto buffer = std::make_unique<char[]>(size);
	std:snprintf(buffer.get(), size, format, conf);
	return std::string(buffer.get(), buffer.get() + size - 1);
}

bool eq(float& a) {
	// Float comparison on '85.00' just does not work in my case. So a quick workaround is needed...
	int number = static_cast<int>(round(a));
	if (number >= 85) { return true; };
	return false;
}

struct word find_word_and_confidence(tesseract::TessBaseAPI& tess_api, cv::Mat& frame, cv::Mat carsColor, cv::Mat licenseColor) {
	struct word w;
	tesseract::ResultIterator* ri = tess_api.GetIterator();
	tesseract::PageIteratorLevel pi_level = tesseract::RIL_WORD;
	// Counting with a simple int counter does not work....
	std::vector<int> count = {};
	char* word;
	float conf;
	std::stringstream ss;
	if (ri != 0) {
		do {
			word = ri->GetUTF8Text(pi_level);
			conf = ri->Confidence(pi_level);
			std::string text_output = float_format("%.2f", conf);
			ss << word;
			ss << " ";
			if (word != NULL) {
				printf("word: '%s';  \tconf: %.2f\n", word, conf);
				cv::putText(frame, text_output, cv::Point(170, 330), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
				// If the confidence is 85.00 or higher increase total_conf
				if (eq(conf)) {
					count.push_back(0);
				}
			}
			delete[] word;
		} while (ri->Next(pi_level));
	}
	ss.seekg(0, std::ios::end);
	int ss_length = ss.tellg();
	cv::putText(frame, ss.str(), cv::Point(190, 260), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	cv::putText(frame, std::to_string(ss_length), cv::Point(150, 190), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	tess_api.Clear();
	// We make sure all the words we find are over 85% confidence. And that there are atleast two words.
	try {
		if (count.size() >= 2)
			printf("asd");
			//w.query_confirmation = mariasql::WRITE_LICENSE_PLATE(frame, carsColor, licenseColor, ss.str());
		else { w.query_confirmation = false; };
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	w.plate = ss.str();
	return w;
}

// http://felix.abecassis.me/2011/09/opencv-detect-skew-angle/
double compute_skew(cv::Mat& frame) {
	cv::Size size = frame.size();
	cv::bitwise_not(frame, frame);
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(frame, lines, 1, CV_PI / 180, 100, size.width / 2.f, 20);

	cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
	double angle = 0.;
	unsigned nb_lines = lines.size();
	// wow
	for (unsigned i = 0; i < nb_lines; ++i)
	{
		cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0, 0));
		angle += atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0]);
	}
	angle /= nb_lines; // Mean angle
	int output = angle * 180 / CV_PI;
	printf("Deskew Angle:  %i\n", output);

	#ifdef DEBUG
		cv::namedWindow("Disp", cv::WINDOW_NORMAL);
		cv::resizeWindow("Disp", (disp_lines.cols / 2), (disp_lines.rows / 2));
		cv::imshow("Disp", disp_lines);
	#endif // DEBUG

	return output;
}

// http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
void deskew(cv::Mat& frame, double angle) {
	cv::bitwise_not(frame, frame);

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = frame.begin<uchar>();
	cv::Mat_<uchar>::iterator end = frame.end<uchar>();
	for (; it != end; ++it)
		if (*it)
			points.push_back(it.pos());

	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
	cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
	cv::warpAffine(frame, frame, rot_mat, frame.size(), cv::INTER_CUBIC);
	cv::bitwise_not(frame, frame);
}

void print_all_the_things(cv::Mat frame) {
	cv::putText(frame, "Cars:", cv::Point(5, 50), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	cv::putText(frame, "Plates:", cv::Point(5, 120), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	cv::putText(frame, "Len:", cv::Point(5, 190), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	cv::putText(frame, "Word:", cv::Point(5, 260), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
	cv::putText(frame, "Conf:", cv::Point(5, 330), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
}

void display_screen_find(cv::Mat& frame, opencv_configuration& opencv_config, double fps, tesseract::TessBaseAPI& tess_api) {
	cv::Mat frame_grey;
	cv::Mat licenseROI;
	cv::Mat licenseColor;
	cv::Mat carsROI;
	cv::Mat carsColor;
	cv::Mat license_output_mat;
	// Convert frame to graysacle
	cv::cvtColor(frame, frame_grey, cv::COLOR_BGR2GRAY);
	// Equalize histogram
	cv::equalizeHist(frame_grey, frame_grey);

	std::vector<cv::Rect> license;
	std::vector<cv::Rect> cars;
	// This si the main part. Looks for "cars"
	opencv_config.car_classifier.detectMultiScale(frame_grey, cars, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100));
	print_all_the_things(frame);
	printf("CARS FOUND: : %i : FPS : %f\n", cars.size(), fps);
	for (size_t i = 0; i < cars.size(); i++) {
		frame_grey(cars[i]).copyTo(carsROI);
		frame(cars[i]).copyTo(carsColor);
		size_t cars_amount;
		cars_amount = cars.size();

		cv::rectangle(frame, cv::Point(cars[i].tl()), cv::Point(cars[i].br()), BRIGHT_BLUE_SCALAR, 3);
		cv::putText(frame, std::to_string(cars_amount), cv::Point(190, 50), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);

		cv::resizeWindow("Car", (carsColor.cols / 2), (carsColor.rows / 2));
		cv::imshow("Car", carsColor);

		opencv_config.licenseplate_classifier.detectMultiScale(carsROI, license, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		// If found we run a loop on every one of them
		for (size_t j = 0; j < license.size(); j++) {
			printf("LICENSE PLATES FOUND: : %i\n", license.size());
			carsROI(license[j]).copyTo(licenseROI);
			carsColor(license[j]).copyTo(licenseColor);
			struct word word;
			// Strinsstream for drawing to screen
			size_t license_amount;
			// Create a window and show the image.
			cv::resizeWindow("License", (licenseColor.cols / 2), (licenseColor.rows / 2));
			cv::imshow("License", licenseColor);
			// Draw where the car is in green.
			cv::rectangle(frame, cv::Point(cars[i].tl()), cv::Point(cars[i].br()), BRIGHT_GREEN_SCALAR, 3);
			// Resize the license plate, for tesseract.
			cv::resize(licenseColor, licenseColor, cv::Size(), 1.4, 1.4, cv::INTER_CUBIC);
			cv::filter2D(licenseColor, licenseColor, -1, kernel);
			// Tell tesseract to look for text on our colored image. 
			tess_api.Clear();
			tess_api.SetImage(licenseColor.data, licenseColor.cols, licenseColor.rows, 3, licenseColor.step);
			// Unsure what to put here, defaults to 70 if not set. But removes the error, for now.
			tess_api.SetSourceResolution(70);
			// This function does the actual recognition and fills data needed for the next function.
			tess_api.GetUTF8Text();
			// Check confidence of every word find. If there are two words. Example: "KB 51561" With 85% confidence per word
			// We send the image over to our SQL server. 
			word = find_word_and_confidence(tess_api, frame, carsColor, licenseColor);
			license_amount = word.plate.size();
			// Draw amount of licenses found on the left corner
			cv::putText(frame, std::to_string(license_amount), cv::Point(230, 120), cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 0, 255), 3);
			// If no word was found we run it through the deskewer
			if ((!word.query_confirmation)) {
				// Compute the skew of text. If we don't fix the image tesseract is unable to read the screen. 
				// TODO: Deskew will break text a bit. Test warp affine flags cv::INTER_CUBIC
				deskew(licenseColor, compute_skew(licenseROI));
				cv::resizeWindow("Rotated", (licenseColor.cols / 2), (licenseColor.rows / 2));
				cv::imshow("Rotated", licenseColor);
				word = find_word_and_confidence(tess_api, frame, carsColor, licenseColor);
				if ((!word.plate.length()) > 0) {
					printf("No recognized text!\n");
					cv::putText(frame, "?", cv::Point(cars[i].tl()), cv::FONT_HERSHEY_SIMPLEX, 2.0, BRIGHT_GREEN_SCALAR, 3);
				}
				else {
					printf("Recognized text: %s\n", word.plate);
					cv::putText(frame, word.plate, cv::Point(cars[i].tl()), cv::FONT_HERSHEY_SIMPLEX, 2.0, BRIGHT_GREEN_SCALAR, 3);
				}
			}
			else {
				printf("Recognized text: %s\n", word.plate);
				cv::putText(frame, word.plate, cv::Point(cars[i].tl()), cv::FONT_HERSHEY_SIMPLEX, 2.0, BRIGHT_GREEN_SCALAR, 3);
			}
			tess_api.Clear();
		}
	}
	cv::resizeWindow("Capture", (frame.cols / 4), (frame.rows / 4));
	cv::imshow("Capture", frame);
}

bool vid_exists(const std::string& file) {
	std::ifstream f(file.c_str());
	return f.good();
}

std::string full_path(std::string& video) {
	return std::filesystem::absolute(std::filesystem::path(video));
}

bool detectAndDisplay::main_loop(opencv_configuration &opencv_config, const bool is_picture, const bool is_camera, tesseract::TessBaseAPI& tess_api) {
	// Load haarcascade
	if (!opencv_config.licenseplate_classifier.load(opencv_config.license_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the haarcascade_russian_plate_number.xml CascadeClassifier file!\n");
		return false;
	}
	if (!opencv_config.car_classifier.load(opencv_config.car_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the cars_cv.xml CascadeClassifier file!\n");
		return false;
	}
	// Picture
	if (is_picture && !is_camera) {
		cv::Mat image = cv::imread(opencv_config.img_arg, cv::IMREAD_COLOR);
		if (image.empty()) {
			fprintf(stderr, "There has been an error loading the image!\n");
			return false;
		}
		bool is_first = true;
		while (!image.empty()) {
			// Only need to process once. Saves processing and frame does not get replaced so it gets filled with drawings
			if (is_first) {
				display_screen_find(image, opencv_config, 1, tess_api);
			}
			is_first = false;
			// ESC
			if (cv::waitKey(30) == 27) {
				break;
			}
		}
	}
	else {
		try {
			cv::VideoCapture capture;
			if (!is_camera) {
				// If vid or protocol
				if (vid_exists(opencv_config.vid_arg)) {
				capture.open(full_path(opencv_config.vid_arg), cv::CAP_FFMPEG);
				}
				else {
					capture.open(opencv_config.vid_arg, cv::CAP_FFMPEG);
				}
			}
			if (is_camera)
				capture.open(opencv_config.camera_arg);
			if (!capture.isOpened()) {
				fprintf(stderr, "Error opening video stream or file!\n");
				capture.release();
				return false;
			}
			// ESC
			while (cv::waitKey(30) != 27) {
				double fps = capture.get(cv::CAP_PROP_FPS);
				cv::Mat frame;
				capture >> frame;
				if (frame.empty()) break;
				display_screen_find(frame, opencv_config, fps, tess_api);
			}
			capture.release();
		}
		catch (const std::exception& e) {
			std::cout << "\n\n" << e.what() << '\n' << std::endl;
		}
		
	}
	cv::destroyAllWindows();
	return 0;
}
