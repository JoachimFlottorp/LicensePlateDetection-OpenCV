#ifndef WORD_HPP
#define WORD_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

#include <vector>


class word {
public:
	word();
	std::size_t plate_size();
	std::size_t plate_length();
	std::string get_plate();
	bool word_status();
	void not_found();
	void found(const bool B);
	void set_plate(std::string s);
private:
	std::string plate;
	bool word_found;
};



#endif // !WORD_HPP