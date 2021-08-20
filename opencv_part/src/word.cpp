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
	B ? word_found = true : word_found = false;
}

void word::set_plate(std::string s) {
	plate = s;
}
