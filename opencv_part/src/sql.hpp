#ifndef SQL_HPP
#define SQL_HPP

#include <mysql/mysql.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>

#include <stdio.h>
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>

#define DEBUG_PRINT fprintf(stderr, "CHECKPOINT REACHED @  %s:%i\n", __FILE__, __LINE__);

struct query_s {
	unsigned long query_len;
	std::string query;
	bool write_confirmation = true;
};

class mariasql
{
public:
	static void mysql_execute_query(MYSQL* conn, std::string sql_query, unsigned long len);

	static bool WRITE_LICENSE_PLATE(cv::Mat& scene, cv::Mat& car, cv::Mat& plate, std::string plate_text);
};




#endif


