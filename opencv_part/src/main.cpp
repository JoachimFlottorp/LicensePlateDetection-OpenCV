#include "main.hpp"

int main(int argc, char *argv[]) {
	if (argc == 3) {
		tesseract::TessBaseAPI* tess_api = new tesseract::TessBaseAPI();
		tess_api->Init("/usr/local/share/tessdata", "nor", tesseract::OEM_LSTM_ONLY);
		tess_api->ReadConfigFile("/usr/local/share/tessdata/configs/opencv_config");
		// https://tesseract-ocr.github.io/tessdoc/ImproveQuality.html#page-segmentation-method // Sounds the best in my head. TODO: Test this.
		tess_api->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
		cv::namedWindow("License", cv::WINDOW_NORMAL);
		cv::namedWindow("Rotated", cv::WINDOW_NORMAL);
		cv::namedWindow("Capture", cv::WINDOW_NORMAL);
		cv::namedWindow("Car", cv::WINDOW_NORMAL);
		opencv_configuration cv_config;
		if (atoi(argv[1]) == 0) {
			// Use picture if specified
			cv_config.img_arg = std::string(argv[2]);
			if (!detectAndDisplay::main_loop(cv_config, true, false, *tess_api)) {
				tess_api->End();
				delete tess_api;
				return -1;
			}
		}
		else if (atoi(argv[1]) == 1) {
			// Use video if specified
			cv_config.vid_arg.append(argv[2]);
			if (!detectAndDisplay::main_loop(cv_config, false, false, *tess_api)) {
				tess_api->End();
				delete tess_api;
				return -1;
			}
		}
		// Input box has to contain 0, 1, -1. Depending on what camera you use. Check docs. https://docs.opencv.org/4.5.2/dd/d43/tutorial_py_video_display.html
		else if (atoi(argv[1]) == 2) {
			cv_config.camera_arg = stoi(static_cast<std::string>(argv[2]));
			if (!detectAndDisplay::main_loop(cv_config, false, true, *tess_api)) {
				tess_api->End();
				delete tess_api;
				return -1;
			}
		}
		else {
			fprintf(stderr, "\nRun file with two args\nExample: opencv_part.out 1 file.mp4\n");
		}
	tess_api->End();
	delete tess_api;
	}
	else
		fprintf(stderr, "\nRun file with two args\nExample: opencv_part.out 1 file.mp4\n");
	printf("\n------------\n| Quitting |\n------------\n");
	return 0;
}