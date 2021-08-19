#include "query_plates.hpp"

query_plates::query_plates() {
	// Needs some size at start..
	plates.push_back("A");
}

void query_plates::append(const std::string& plate) {
	plates.push_back(plate);
}

size_t query_plates::size() {
	return plates.size();
}

std::vector<std::string>& query_plates::get_vector() {
	return plates;
}

