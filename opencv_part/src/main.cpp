#include "main.hpp"

bool load_picture(tesseract::TessBaseAPI& tess_api) {
	bool isFirst = true;
	if (!g_licenseplate_lp_classifier.load(g_license_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the haarcascade_russian_plate_number.xml CascadeClassifier file!\n");
		return false;
	}
	if (!g_car_lp_classifier.load(g_car_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the cars_cv.xml CascadeClassifier file!\n");
		return false;
	}
	cv::Mat image = cv::imread(g_img_arg, cv::IMREAD_COLOR);
	if (image.empty()) {
		fprintf(stderr, "There has been an error loading the image!\n");
		return false;
	}
	while (!image.empty()) {
		// Only need to process once. Saves processing and frame does not get replaced so it gets filled with drawings
		if (isFirst) {
			cv::Rect query_store;
			detectAndDisplay::display_screen_find(image, g_car_lp_classifier, g_licenseplate_lp_classifier, 1, tess_api, conn);
		}
		isFirst = false;
		// ESC
		if (cv::waitKey(30) == 27) {
			break;
		}
	}
	cv::destroyAllWindows();
	return true;
}

bool load_video_second(cv::VideoCapture& capture, tesseract::TessBaseAPI& tess_api) {
	double fps = capture.get(cv::CAP_PROP_FPS);
	if (!capture.isOpened()) {
		fprintf(stderr, "Error opening video stream or file!\n");
		capture.release();
		return false;
	}
	// ESC
	while (cv::waitKey(30) != 27) {
		cv::Mat frame;
		capture >> frame;
		if (frame.empty())
			break;
		detectAndDisplay::display_screen_find(frame, g_car_lp_classifier, g_licenseplate_lp_classifier, fps, tess_api, conn);
	}
	capture.release();
	return true;
}

bool load_video(tesseract::TessBaseAPI& tess_api, bool ifCamera) {
	if (!g_licenseplate_lp_classifier.load(g_license_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the haarcascade_russian_plate_number.xml CascadeClassifier file!\n");
		return false;
	}
	if (!g_car_lp_classifier.load(g_car_class_arg)) {
		fprintf(stderr, "\nThere has been an error loading the cars_cv.xml CascadeClassifier file!\n");
		return false;
	}

	if (ifCamera) {
		// Impossible with my skill level to do constructor shit on runtime. if it even is possible.
		cv::VideoCapture capture(g_camera_arg);
		if (!load_video_second(capture, tess_api))
			return false;
	}
	else {
		cv::VideoCapture capture(g_vid_arg, cv::CAP_FFMPEG);
		if (!load_video_second(capture, tess_api))
			return false;
	}
	cv::destroyAllWindows();
	return true;
}

int main(int argc, char *argv[]) {
	// Create sql connection
	if (!(conn = mysql_init(0))) {
		fprintf(stderr, "Unable to intitalize a connection!\n");
		exit(1);
	}
	if (!mysql_real_connect(conn, "172.17.0.1", "root", "root", "license", 3306, nullptr, 0)) {
		fprintf(stderr, "Error connecting to SQL server!: %s\n", mysql_error(conn));
		mysql_close(conn);
		exit(1);
	} else printf("Connected to SQL server!\nServer Version: %s\nHost Info: %s\n", mysql_get_server_info(conn), mysql_get_host_info(conn));

	tesseract::TessBaseAPI* tess_api = new tesseract::TessBaseAPI();
	if (argc == 3) {
		tess_api->Init(NULL, "nor", tesseract::OEM_LSTM_ONLY);
		tess_api->ReadConfigFile("/usr/local/share/tessdata/configs/opencv_config");
		// https://tesseract-ocr.github.io/tessdoc/ImproveQuality.html#page-segmentation-method // Sounds the best in my head. TODO: Test this V
		tess_api->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		cv::namedWindow("License", cv::WINDOW_NORMAL);
		cv::namedWindow("Rotated", cv::WINDOW_NORMAL);
		cv::namedWindow("Capture", cv::WINDOW_NORMAL);
		cv::namedWindow("Car", cv::WINDOW_NORMAL);
		if (atoi(argv[1]) == 0) {
			// Use picture if specified
			g_img_arg = argv[2];
			if (!load_picture(*tess_api))
				return -1;
		}
		else if (atoi(argv[1]) == 1) {
			// Use video if specified
			g_vid_arg = argv[2];
			if (!load_video(*tess_api, false))
				return -1;
		}
		// Input box has to contain 0, 1, -1. Depending on what camera you use. Check docs. https://docs.opencv.org/4.5.2/dd/d43/tutorial_py_video_display.html
		else if (atoi(argv[1]) == 2) {
			g_vid_arg = argv[2];
			// Convert to int
			g_camera_arg = stoi(g_vid_arg);
			if (!load_video(*tess_api, true))
				return -1;
		}
		else {
			fprintf(stderr, "\n Not correct args. Please run the LicensePlateDetection.out instead\n");
		}
	}
	else
		fprintf(stderr, "\nNot enough args. Please run the LicensePlateDetection.out instead: argc: %i\n", argc);
	
	tess_api->End();
	delete tess_api;
	mysql_close(conn);
	printf("\n------------\n| Quitting |\n------------\n");
	return 0;
}