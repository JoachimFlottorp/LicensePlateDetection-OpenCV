#include "word.hpp"

word::word() {
	word_found = false;
}

std::size_t word::plate_size() {
	return plate.size();
}

std::size_t word::plate_length() {
	return plate.length();
}

std::string word::get_plate() {
	return plate;
}

bool word::word_status() {
	return word_found;
}

void word::found(const bool B) {
	if (B)
		word_found = true;
	else
		word_found = false;
}

void word::set_plate(std::string s) {
	plate = s;
}

void word::not_found() {
	word_found = false;
}
