#ifndef QUERY_PLATES_HPP
#define QUERY_PLATES_HPP


#include <string>
#include <vector>

class query_plates {
public:
	query_plates();


	void append(const std::string& plate);
	size_t size();
	std::vector<std::string>& get_vector();
private:
	std::vector<std::string> plates;

};


#endif // !QUERY_PLATES_HPP



