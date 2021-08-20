NAME=opencv_part

CC=g++
FLAGS= -Wall -std=c++20 -g2 -gdwarf-2
# -Wextra -Wunused-value -Wunused-variable 
LIBS= -L/usr/lib/ -lmariadb -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_objdetect -lopencv_ml -ltesseract -lmariadb -lpthread
RM =rm -rf
INCLUDES= -I/usr/include/mysql -I/usr/include/mysql/mysql -I/usr/local/include/opencv4
OBJS= main.o detectAndDisplay.o sql.o word.o query_plates.o

output: $(OBJS)
	$(CC) $(FLAGS) $(INCLUDES) $(LIBS) $(OBJS) -o bin/opencv_part.out

main.o: main.cpp main.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c main.cpp

detectAndDisplay.o: detectAndDisplay.cpp detectAndDisplay.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c detectAndDisplay.cpp

sql.o: sql.cpp sql.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c sql.cpp

word.o: word.cpp word.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c word.cpp

query_plates.o: query_plates.cpp query_plates.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c query_plates.cpp

clean:
	rm *.o bin/opencv_part.out