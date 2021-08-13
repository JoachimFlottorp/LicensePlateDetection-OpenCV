#include "sql.hpp"

struct query_s string_format(const cv::Mat& scene, const cv::Mat& car, const cv::Mat& plate, std::string plate_text, MYSQL* conn);

void mariasql::mysql_execute_query(MYSQL* conn, std::string sql_query, unsigned long len) {
	if (mysql_real_query(conn, sql_query.c_str(), len)) {
		printf("MySQL Query error: %s\n", mysql_error(conn));
		mysql_close(conn);
		exit(1);
	}
}

bool mariasql::WRITE_LICENSE_PLATE(MYSQL* conn, cv::Mat& scene, cv::Mat& car, cv::Mat& plate, std::string plate_text) {
	printf("\nWriting to SQL.\n");
	query_s s;
	if (s.write_confirmation) {
		// Convert query
		s = string_format(scene, car, plate, plate_text, conn);
		// Print stuff
		printf("String made\n");
		printf("SQL Query Length: %ul\n", s.query_len);
		// Make the SQL query
		mariasql::mysql_execute_query(conn, s.query, s.query_len);
		return true;
	}
	else { fprintf(stderr, "\nError Creating SQL query\n"); return false; };
}

struct query_s string_format(const cv::Mat& scene, const cv::Mat& car, const cv::Mat& plate, std::string plate_text, MYSQL* conn) {
	query_s s;
	try
	{
		// Encode all three images to a char* 
		std::vector<uchar> scene_buffer;
		cv::imencode(".jpg", scene, scene_buffer);
		char* scene_char = reinterpret_cast<char*>(scene_buffer.data());

		std::vector<uchar> car_buffer;
		cv::imencode(".jpg", car, car_buffer);
		char* car_char = reinterpret_cast<char*>(car_buffer.data());

		std::vector<uchar> plate_buffer;
		cv::imencode(".jpg", plate, plate_buffer);
		char* plate_char = reinterpret_cast<char*>(plate_buffer.data());

		// SQL Does not understand plate text if not casted to a char*
		char* plate_text_char = const_cast<char*>(plate_text.c_str());
		
		// Create three char* buffers. This will be filled by the next function
		char* scene_chunk = new char[sizeof(char*) + (2 * scene_buffer.size()) + 1];
		char* car_chunk = new char[sizeof(char*) + (2 * car_buffer.size()) + 1];
		char* plate_chunk = new char[sizeof(char*) + (2 * plate_buffer.size()) + 1];
		
		// Pass our encoded images to mysql_real_escape_string. This removes certain escape characters from the string.
		printf("SQL Scene Length: %lu\n", mysql_real_escape_string(conn, scene_chunk, scene_char, scene_buffer.size()));
		printf("SQL Car Length: %lu\n", mysql_real_escape_string(conn, car_chunk, car_char, car_buffer.size()));
		printf("SQL Plate Length: %lu\n", mysql_real_escape_string(conn, plate_chunk, plate_char, plate_buffer.size()));

		// Our snprintf query
		const char* query_string = "INSERT INTO PLATES(scene, car, plate, plateText) VALUES('%s', '%s', '%s', '%s');";
		// Find the size of the sprintf
		unsigned long size = std::snprintf(nullptr, 0, query_string, scene_chunk, car_chunk, plate_chunk, plate_text_char) + 1;
		// Make sure it worked
		if (size <= 0) { s.write_confirmation = false; throw std::runtime_error("Error creating SQL Query!");};
		// Cast over to a char array buffer
		auto buffer = std::make_unique<char[]>(size);
		// Do the actual snprintf
		std:snprintf(buffer.get(), size, query_string, scene_chunk, car_chunk, plate_chunk, plate_text_char);
		// Convert to string and add query and query length to our struct
		s.query = std::string(buffer.get(), buffer.get() + size - 1);
		s.query_len = size - 1;
		// Delete our char
		delete[] scene_chunk;
		delete[] car_chunk;
		delete[] plate_chunk;
		delete[] plate_text_char;
		delete[] plate_char;
		delete[] car_char;
		delete[] scene_char;
	}
	catch (const std::runtime_error& e) {
		s.write_confirmation = false;
		std::cout << e.what() << std::endl;
	}
	return s;
}