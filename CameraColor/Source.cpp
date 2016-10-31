//objectTrackingTutorial.cpp

//Written by  Kyle Hounslow 2015

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <sstream>
#include <string>
#include <iostream>
#include "opencv\highgui.h"
#include "opencv\cv.h"
#include <time.h>
#include <conio.h>
#include <algorithm>
#include <functional>

using namespace cv;
using namespace std;
//initial min and max HSV filter values.
//these will be changed using trackbars

char k;

int c = 0;

int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;

//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";
const string backgroundWindowName = "Background";

string timeString;
string pointString;
string scoreString;
string timetextString = "Time:";
string pointtextString = "Level:";
string scoreTextString = "Best Time:";
string oneString = "1.";
string twoString = "2.";
string threeString = "3.";

int scores[4] = { 999, 999, 999, 999 };

string scores0 = "999";
string scores1 = "999";
string scores3 = "999";

//variables for ingame timer
int64 start_time = 0;
int64 end_time = 0;
int64 difference_time = 0;
int64 timer_output = 0;

//used for testing if tracking was lost for more than a few secs
int64 start_time2 = 0;
int64 end_time2 = 0;
int64 difference_time2 = 0;
int64 timer_output2 = 0;
bool delay_result = false;

//used for determining the overall completion time (for "best time")
int64 start_time3 = 0;
int64 end_time3 = 0;
int64 difference_time3 = 0;
int64 timer_output3 = 0;

//used for counting points gained in the current game
int64 points_counter = 0;


//used for showing debugging windows, trackbars etc.
bool calibrationMode;

//used for showing a rectangle on screen as user clicks and drags mouse
bool mouseIsDragging;
bool mouseMove;
bool rectangleSelected;
bool objectFound;

//used for checking if a tracking color was set
bool start_tracker = false;


//used for controlling game flow
int64 state = 0;

//used for setting the time the player has to complete the level
int64 level_time = 0;


/*
Mat map11Text = imread("map11.png", CV_LOAD_IMAGE_COLOR);
Mat map12Text = imread("map12.png", CV_LOAD_IMAGE_COLOR);
Mat map13Text = imread("map13.png", CV_LOAD_IMAGE_COLOR);

Mat map21Text = imread("map21.png", CV_LOAD_IMAGE_COLOR);
Mat map22Text = imread("map22.png", CV_LOAD_IMAGE_COLOR);
Mat map23Text = imread("map23.png", CV_LOAD_IMAGE_COLOR);

Mat map31Text = imread("map31.png", CV_LOAD_IMAGE_COLOR);
Mat map32Text = imread("map32.png", CV_LOAD_IMAGE_COLOR);
Mat map33Text = imread("map33.png", CV_LOAD_IMAGE_COLOR);

Mat map41Text = imread("map41.png", CV_LOAD_IMAGE_COLOR);
Mat map42Text = imread("map42.png", CV_LOAD_IMAGE_COLOR);
Mat map43Text = imread("map43.png", CV_LOAD_IMAGE_COLOR);

Mat map51Text = imread("map51.png", CV_LOAD_IMAGE_COLOR);
Mat map52Text = imread("map52.png", CV_LOAD_IMAGE_COLOR);
Mat map53Text = imread("map53.png", CV_LOAD_IMAGE_COLOR);

Mat map61Text = imread("map61.png", CV_LOAD_IMAGE_COLOR);
Mat map62Text = imread("map62.png", CV_LOAD_IMAGE_COLOR);
Mat map63Text = imread("map63.png", CV_LOAD_IMAGE_COLOR);

Mat map71Text = imread("map71.png", CV_LOAD_IMAGE_COLOR);
Mat map72Text = imread("map72.png", CV_LOAD_IMAGE_COLOR);
Mat map73Text = imread("map73.png", CV_LOAD_IMAGE_COLOR);

Mat map81Text = imread("map81.png", CV_LOAD_IMAGE_COLOR);
Mat map82Text = imread("map82.png", CV_LOAD_IMAGE_COLOR);
Mat map83Text = imread("map83.png", CV_LOAD_IMAGE_COLOR);

Mat map91Text = imread("map91.png", CV_LOAD_IMAGE_COLOR);
Mat map92Text = imread("map92.png", CV_LOAD_IMAGE_COLOR);
Mat map93Text = imread("map93.png", CV_LOAD_IMAGE_COLOR);





Mat map11 = imread("map11.png", CV_LOAD_IMAGE_COLOR);
Mat map12 = imread("map12.png", CV_LOAD_IMAGE_COLOR);
Mat map13 = imread("map13.png", CV_LOAD_IMAGE_COLOR);

Mat map21 = imread("map21.png", CV_LOAD_IMAGE_COLOR);
Mat map22 = imread("map22.png", CV_LOAD_IMAGE_COLOR);
Mat map23 = imread("map23.png", CV_LOAD_IMAGE_COLOR);

Mat map31 = imread("map31.png", CV_LOAD_IMAGE_COLOR);
Mat map32 = imread("map32.png", CV_LOAD_IMAGE_COLOR);
Mat map33 = imread("map33.png", CV_LOAD_IMAGE_COLOR);

Mat map41 = imread("map41.png", CV_LOAD_IMAGE_COLOR);
Mat map42 = imread("map42.png", CV_LOAD_IMAGE_COLOR);
Mat map43 = imread("map43.png", CV_LOAD_IMAGE_COLOR);

Mat map51 = imread("map51.png", CV_LOAD_IMAGE_COLOR);
Mat map52 = imread("map52.png", CV_LOAD_IMAGE_COLOR);
Mat map53 = imread("map53.png", CV_LOAD_IMAGE_COLOR);

Mat map61 = imread("map61.png", CV_LOAD_IMAGE_COLOR);
Mat map62 = imread("map62.png", CV_LOAD_IMAGE_COLOR);
Mat map63 = imread("map63.png", CV_LOAD_IMAGE_COLOR);

Mat map71 = imread("map71.png", CV_LOAD_IMAGE_COLOR);
Mat map72 = imread("map72.png", CV_LOAD_IMAGE_COLOR);
Mat map73 = imread("map73.png", CV_LOAD_IMAGE_COLOR);

Mat map81 = imread("map81.png", CV_LOAD_IMAGE_COLOR);
Mat map82 = imread("map82.png", CV_LOAD_IMAGE_COLOR);
Mat map83 = imread("map83.png", CV_LOAD_IMAGE_COLOR);

Mat map91 = imread("map91.png", CV_LOAD_IMAGE_COLOR);
Mat map92 = imread("map92.png", CV_LOAD_IMAGE_COLOR);
Mat map93 = imread("map93.png", CV_LOAD_IMAGE_COLOR);
*/

//pictures for mapbackgrounds
Mat	mapStart = imread("mapStart.png", CV_LOAD_IMAGE_COLOR);

Mat imageBackground;

//Mat mapWon = imread("mapWon.png", CV_LOAD_IMAGE_COLOR);

//Mat mapWonText = imread("mapWon.png", CV_LOAD_IMAGE_COLOR);

Mat mapLost = imread("mapLost.png", CV_LOAD_IMAGE_COLOR);

//keep track of initial point clicked and current position of mouse
cv::Point initialClickPoint, currentMousePoint; 

//this is the ROI that the user has selected
cv::Rect rectangleROI; 

//HSV values from the click/drag ROI region stored in separate vectors so that we can sort them easily
vector<int> H_ROI, S_ROI, V_ROI;

void on_trackbar(int, void*)
{//This function gets called whenever a
	// trackbar position is changed

	//for now, this does nothing.



}

void startScoreTimer(){
	time_t t1 = time(0);   // get time now
	start_time3 = t1;
	struct tm * now = localtime(&t1);
	//std::cout << t1 << std::endl;
}


//stops level timer and checks if time is over
void stopScoreTimer(){
	time_t t2 = time(0);   // get time now
	end_time3 = t2;
	struct tm * now = localtime(&t2);
	//std::cout << t2 << std::end2;
	if (end_time3 > start_time3) {
		difference_time3++;
	}
}

//creates level timer
void createStartTimer(){
	time_t t1 = time(0);   // get time now
	start_time = t1;
	struct tm * now = localtime(&t1);
	//std::cout << t1 << std::endl;
}


//stops level timer and checks if time is over
void createStopTimer(){
	time_t t2 = time(0);   // get time now
	end_time = t2;
	struct tm * now = localtime(&t2);
	//std::cout << t2 << std::end2;
	if (end_time > start_time) {
		difference_time++;
		timer_output = level_time - difference_time;
	}

}

void checkTimeOut() {
	createStopTimer();
	if (timer_output < 0) {
		state = 3;
		delay_result = false;
		difference_time = 0;
		difference_time2 = 0;
	}
}



//controls which picture is loaded as backgound
void pointSwitch(){

	/*
	switch (points_counter)
	{
	case 0:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map11Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map11Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map11Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map11Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map11Text);
	//waitkey(30);
	level_time = 23;
	map11Text = map11.clone();
	stopScoreTimer();
	break;

	}
	case 1:
	{
	//state = 2;
	//delay_result = false;
	//level_time = 0;

	//stopScoreTimer();
	//break;

	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map12Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map12Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map12Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map12Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map12Text);
	//waitkey(30);
	level_time = 23;
	map12Text = map12.clone();
	stopScoreTimer();
	break;
	}
	case 2:
	{

	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map13Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map13Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map13Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map13Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map13Text);
	//waitkey(30);
	level_time = 23;
	map13Text = map13.clone();
	stopScoreTimer();
	break;
	}
	case 3:
	{
	state = 2;
	delay_result = false;
	level_time = 0;

	//stopScoreTimer();
	break;

	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map21Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map21Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map21Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map21Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map21Text);
	//waitkey(30);
	level_time = 23;
	map21Text = map21.clone();
	stopScoreTimer();
	break;
	}
	case 4:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map22Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map22Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map22Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map22Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map22Text);
	//waitkey(30);
	level_time = 23;
	map22Text = map22.clone();
	stopScoreTimer();
	break;
	}
	case 5:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map23Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map23Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map23Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map23Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map23Text);
	//waitkey(30);
	level_time = 23;
	map23Text = map23.clone();
	stopScoreTimer();
	break;
	}
	case 6:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map31Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map31Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map31Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map31Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map31Text);
	//waitkey(30);
	level_time = 23;
	map31Text = map31.clone();
	stopScoreTimer();
	break;
	}
	case 7:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map32Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map32Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map32Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map32Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map32Text);
	//waitkey(30);
	level_time = 23;
	map32Text = map32.clone();
	stopScoreTimer();
	break;
	}
	case 8:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map33Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map33Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map33Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map33Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map33Text);
	//waitkey(30);
	level_time = 23;
	map33Text = map33.clone();
	stopScoreTimer();
	break;
	}
	case 9:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map41Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map41Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map41Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map41Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map41Text);
	//waitkey(30);
	level_time = 23;
	map41Text = map41.clone();
	stopScoreTimer();
	break;
	}
	case 10:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map42Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map42Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map42Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map42Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map42Text);
	//waitkey(30);
	level_time = 23;
	map42Text = map42.clone();
	stopScoreTimer();
	break;
	}
	case 11:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map43Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map43Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map43Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map43Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map43Text);
	//waitkey(30);
	level_time = 23;
	map43Text = map43.clone();
	stopScoreTimer();
	break;
	}
	case 12:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map51Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map51Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map51Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map51Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map51Text);
	//waitkey(30);
	level_time = 23;
	map51Text = map51.clone();
	stopScoreTimer();
	break;
	}
	case 13:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map52Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map52Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map52Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map52Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map52Text);
	//waitkey(30);
	level_time = 23;
	map52Text = map52.clone();
	stopScoreTimer();
	break;
	}
	case 14:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map53Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map53Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map53Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map53Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map53Text);
	//waitkey(30);
	level_time = 23;
	map53Text = map53.clone();
	stopScoreTimer();
	break;
	}
	case 15:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map61Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map61Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map61Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map61Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map61Text);
	//waitkey(30);
	level_time = 23;
	map61Text = map61.clone();
	stopScoreTimer();
	break;
	}
	case 16:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map62Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map62Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map62Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map62Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map62Text);
	//waitkey(30);
	level_time = 23;
	map62Text = map62.clone();
	stopScoreTimer();
	break;
	}
	case 17:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map63Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map63Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map63Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map63Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map63Text);
	//waitkey(30);
	level_time = 23;
	map63Text = map63.clone();
	stopScoreTimer();
	break;
	}
	case 18:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map71Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map71Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map71Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map71Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map71Text);
	//waitkey(30);
	level_time = 23;
	map71Text = map71.clone();
	stopScoreTimer();
	break;
	}
	case 19:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map72Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map72Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map72Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map72Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map72Text);
	//waitkey(30);
	level_time = 23;
	map72Text = map72.clone();
	stopScoreTimer();
	break;
	}
	case 20:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map73Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map73Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map73Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map73Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map73Text);
	//waitkey(30);
	level_time = 23;
	map73Text = map73.clone();
	stopScoreTimer();
	break;
	}
	case 21:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map81Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map81Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map81Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map81Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map81Text);
	//waitkey(30);
	level_time = 23;
	map81Text = map81.clone();
	stopScoreTimer();
	break;
	}
	case 22:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map82Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map82Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map82Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map82Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map82Text);
	//waitkey(30);
	level_time = 23;
	map82Text = map82.clone();
	stopScoreTimer();
	break;
	}
	case 23:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map83Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map83Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map83Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map83Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map83Text);
	//waitkey(30);
	level_time = 23;
	map83Text = map83.clone();
	stopScoreTimer();
	break;
	}
	case 24:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map91Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map91Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map91Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map91Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map91Text);
	//waitkey(30);
	level_time = 23;
	map91Text = map91.clone();
	stopScoreTimer();
	break;
	}
	case 25:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map92Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map92Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map92Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map92Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map92Text);
	//waitkey(30);
	level_time = 23;
	map92Text = map92.clone();
	stopScoreTimer();
	break;
	}
	case 26:
	{
	std::string timeString = std::to_string(timer_output);
	std::string pointString = std::to_string(points_counter);
	putText(map93Text, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map93Text, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
	putText(map93Text, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	putText(map93Text, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
	imshow(backgroundWindowName, map93Text);
	//waitkey(30);
	level_time = 45;
	map93Text = map93.clone();
	stopScoreTimer();
	break;
	}
	case 27:
	{
	state = 2;
	delay_result = false;
	level_time = 0;
	stopScoreTimer();
	break;
	}

	default:
	{
	cout << "PointSwitch went to default" << endl;
	break;
	}
	*/


	switch (points_counter)
	{
	case 0:
	{
		Mat map11 = imread("map11.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map11, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map11, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map11, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map11, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map11);
		//waitkey(30);
		level_time = 60;
		stopScoreTimer();
		break;

	}
	case 1:
	{
		//state = 2;
		//delay_result = false;
		//level_time = 0;
		//stopScoreTimer();
		//break;
		
		
		Mat map12 = imread("map12.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map12, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map12, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map12, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map12, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map12);
		//waitkey(30);
		level_time = 55;
		stopScoreTimer();
		break;
		
	}
	case 2:
	{
		Mat map13 = imread("map13.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map13, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map13, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map13, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map13, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map13);
		//waitkey(30);
		level_time = 55;
		stopScoreTimer();
		break;
	}
	case 3:
	{
		//state = 2;
		//delay_result = false;
		//level_time = 0;

		//stopScoreTimer();
		//break;
		Mat map21 = imread("map21.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map21, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map21, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map21, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map21, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map21);
		//waitkey(30);
		level_time = 50;
		stopScoreTimer();
		break;
	}
	case 4:
	{
		Mat map22 = imread("map22.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map22, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map22, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map22, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map22, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map22);
		//waitkey(30);
		level_time = 50;
		stopScoreTimer();
		break;
	}
	case 5:
	{
		Mat map23 = imread("map23.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map23, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map23, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map23, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map23, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map23);
		//waitkey(30);
		level_time = 45;
		stopScoreTimer();
		break;
	}
	case 6:
	{
		Mat map31 = imread("map31.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map31, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map31, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map31, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map31, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map31);
		//waitkey(30);
		level_time = 45;
		stopScoreTimer();
		break;
	}
	case 7:
	{
		Mat map32 = imread("map32.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map32, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map32, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map32, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map32, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map32);
		//waitkey(30);
		level_time = 40;
		stopScoreTimer();
		break;
	}
	case 8:
	{
		Mat map33 = imread("map33.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map33, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map33, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map33, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map33, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map33);
		//waitkey(30);
		level_time = 40;
		stopScoreTimer();
		break;
	}
	case 9:
	{
		Mat map41 = imread("map41.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map41, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map41, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map41, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map41, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map41);
		//waitkey(30);
		level_time = 40;
		stopScoreTimer();
		break;
	}
	case 10:
	{
		Mat map42 = imread("map42.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map42, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map42, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map42, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map42, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map42);
		//waitkey(30);
		level_time = 35;
		stopScoreTimer();
		break;
	}
	case 11:
	{
		Mat map43 = imread("map43.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map43, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map43, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map43, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map43, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map43);
		//waitkey(30);
		level_time = 35;
		stopScoreTimer();
		break;
	}
	case 12:
	{
		Mat map51 = imread("map51.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map51, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map51, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map51, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map51, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map51);
		//waitkey(30);
		level_time = 35;
		stopScoreTimer();
		break;
	}
	case 13:
	{
		Mat map52 = imread("map52.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map52, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map52, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map52, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map52, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map52);
		//waitkey(30);
		level_time = 30;
		stopScoreTimer();
		break;
	}
	case 14:
	{
		Mat map53 = imread("map53.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map53, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map53, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map53, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map53, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map53);
		//waitkey(30);
		level_time = 25;
		stopScoreTimer();
		break;
	}
	case 15:
	{
		Mat map61 = imread("map61.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map61, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map61, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map61, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map61, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map61);
		//waitkey(30);
		level_time = 25;
		stopScoreTimer();
		break;
	}
	case 16:
	{
		Mat map62 = imread("map62.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map62, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map62, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map62, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map62, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map62);
		//waitkey(30);
		level_time = 25;
		stopScoreTimer();
		break;
	}
	case 17:
	{
		Mat map63 = imread("map63.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map63, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map63, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map63, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map63, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map63);
		//waitkey(30);
		level_time = 20;
		stopScoreTimer();
		break;
	}
	case 18:
	{
		Mat map71 = imread("map71.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map71, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map71, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map71, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map71, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map71);
		//waitkey(30);
		level_time = 20;
		stopScoreTimer();
		break;
	}
	case 19:
	{
		Mat map72 = imread("map72.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map72, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map72, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map72, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map72, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map72);
		//waitkey(30);
		level_time = 20;
		stopScoreTimer();
		break;
	}
	case 20:
	{
		Mat map73 = imread("map73.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map73, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map73, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map73, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map73, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map73);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 21:
	{
		Mat map81 = imread("map81.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map81, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map81, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map81, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map81, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map81);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 22:
	{
		Mat map82 = imread("map82.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map82, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map82, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map82, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map82, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map82);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 23:
	{
		Mat map83 = imread("map83.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map83, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map83, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map83, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map83, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map83);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 24:
	{
		Mat map91 = imread("map91.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map91, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map91, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map91, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map91, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map91);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 25:
	{
		Mat map92 = imread("map92.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map92, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map92, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map92, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map92, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map92);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 26:
	{
		Mat map93 = imread("map93.png", CV_LOAD_IMAGE_COLOR);
		std::string timeString = std::to_string(timer_output);
		std::string pointString = std::to_string(points_counter);
		putText(map93, timeString, Point(150, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map93, pointString, Point(950, 50), 2, 1.25, Scalar(138, 17, 21), 1, CV_AA);
		putText(map93, timetextString, Point(25, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		putText(map93, pointtextString, Point(825, 50), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, map93);
		//waitkey(30);
		level_time = 15;
		stopScoreTimer();
		break;
	}
	case 27:
	{
		state = 2;
		delay_result = false;
		level_time = 0;
		stopScoreTimer();
		difference_time = 0;
		difference_time2 = 0;
		break;
	}

	default:
	{
		cout << "PointSwitch went to default" << endl;
		break;
	}

	}

}
	





//starts test if the tracking was lost for more than a few secs
void checkDelayStart(){
	time_t t1 = time(0);   // get time now
	start_time2 = t1;
	struct tm * now = localtime(&t1);

}

//ends test
void checkDelayEnd() {
	time_t t2 = time(0);   // get time now
	end_time2 = t2;
	struct tm * now = localtime(&t2);
	//std::cout << t2 << std::end2;
	if (end_time2 > start_time2) {
		difference_time2++;
		
	}
	if (difference_time2 <= 3) {
		delay_result = false;
		cout << "delay_result = false" << endl;
	}
	else {
		delay_result = true;
		cout << "delay_result = true" << endl;
		difference_time2 = 0;
	}
}

//Function for resetting delay if a tracking was found. This is not used right now.
/*void resetDelay(){
	if (objectFound == true) {
		difference_time2 = 0;
		cout << "objectFound true, delay reset" << endl;
	}
	else {
		cout << "objectFound false, counting delay" << endl;
	}
}*/

void createBackground(){
	//create window for background
	//Mat winBackground;
	/*if (objectFound == true)
	{*/
	namedWindow(backgroundWindowName, 0);
	cout << "Background created" << endl;
	char BackgroundName[50];
}
		/*std::string timeString = std::to_string(timer_output);
		
		imageBackground = imread("test.png", CV_LOAD_IMAGE_COLOR);
		circle(imageBackground, Point(50, 50), 50, Scalar(255, 255, 255), CV_FILLED, 8, 0);
		putText(imageBackground, timeString, Point(100, 100), 1, 2, Scalar(0, 0, 255), 2);
		imshow(backgroundWindowName, imageBackground);
		waitKey(10);

	}
	else
	{
		namedWindow(backgroundWindowName, 0);
		winBackground = imread("testwin.png", CV_LOAD_IMAGE_COLOR);
		imshow(backgroundWindowName, winBackground);
		waitKey(10);
	}

	char BackgroundName[50];
	*/








//general gameplay function
void gameState(){
	cout << "gameState started" << endl;
	if (state == 0){ //game start
		cout << "went to state 0" << endl;
		//createStartTimer();
		checkDelayStart();
//		Mat mapStart;

		//start delaycheck
		
		//set HSVs
		/*H_MIN = 35;
		H_MAX = 55;
		S_MIN = 75;
		S_MAX = 90;
		V_MIN = 50;
		V_MAX = 88;*/

		//load starting map 
//		mapStart = imread("mapStart.png", CV_LOAD_IMAGE_COLOR);
		//putText(mapStart, "Hi all...", Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4);
		imshow(backgroundWindowName, mapStart);
		waitKey(30);

		//load texts for explanations and headlines (watch anti aliasing)
		
		


		//check if tracking lost with delaycheck, if yes move to state 1 and start scoretimer, if no do nothing (exept check delay)
		if (objectFound == false) {
			cout << "state = 0: objectFound is false" << endl;
			
			if (delay_result == true && start_tracker == true){
				cout << "state = 0: delay_result is true and start_tracker is true" << endl;
				state = 1;
				delay_result = false;
				level_time = 0;
				difference_time2 = 0;
				//startScoreTimer();
			}
			else if (delay_result == false && start_tracker == false) {
				cout << "state = 0: delay_result is false and start_tracker is false" << endl;
			}
			else if (delay_result == true && start_tracker == false) {
				cout << "state = 0: delay_result is true and start_tracker is false" << endl;
			}
			else if (delay_result == false && start_tracker == true) {
				cout << difference_time2 << endl;
				cout << "state = 0: delay_result is false and start_tracker is true" << endl;
			}
		}
		else if (objectFound == true) {
			cout << "state = 0: objectFound is true" << endl;
			difference_time2 = 0;
			delay_result = false;

			if (start_tracker == false){
			cout << "state = 0: start_tracker is false" << endl;
			start_tracker = true;
		}

			
		}

		//stop delaycheck
		checkDelayEnd();
		cout << "delayend started" << endl;
		//createStopTimer();
		cout << "delayend is done" << endl;
	}
	else if (state == 1){ //game running
		cout << "went to state 1" << endl;
		//start delaycheck
		checkDelayStart();
		createStartTimer();
		
		std::string timeString = std::to_string(timer_output);
		startScoreTimer();
		//load map and +time according to pointswitch, in p-s set state 3 if over ggggggggggggg
		pointSwitch();

		//load texts for points, timer, level-stage
		
		

		//check if tracking lost with delaycheck, if yes pointscounter++, if no timer--
		if (objectFound == false) {
			cout << "state 1: objectFound is false" << endl;
			if (delay_result == true){
				cout << "state = 1: delay_result is true" << endl;
				points_counter++;
				difference_time = 0;
				std::string pointString = std::to_string(points_counter);
			}
			else {
				cout << "state = 1: delay_result is false" << endl;
			}
		}
		else if (objectFound = true) {
			cout << "state = 1: objectFound is true" << endl;
			difference_time2 = 0;
		}
		//check if timer over, if yes set state 3
		checkTimeOut();

		//stop delaycheck
		checkDelayEnd();
	}
	else if (state == 2){ //game won
		cout << "went to state 2" << endl;
		
		//start delaycheck
		checkDelayStart();
		
		//waitkey(30);
		//stop scoretimer
		if (scores[3] != difference_time3 && scores[2] != difference_time3 && scores[1] != difference_time3 && scores[0] != difference_time3) {
			scores[3] = difference_time3;
			std::sort(scores, scores + 4);//, std::greater<int>()); //n number of elements i want to sort, in asc remove after +n
		} //COMPARE HERE
			
		//array exp
		
		std::string scores0 = std::to_string(scores[0]);
		std::string scores1 = std::to_string(scores[1]);
		std::string scores2 = std::to_string(scores[2]);

		/*putText(mapWonText, scoreTextString, Point(3000, 1000), 2, 1, Scalar(138, 21, 17), 1, CV_AA);

		putText(mapWonText, oneString, Point(3000, 1200), 2, 1, Scalar(138, 21, 17), 1, CV_AA);
		putText(mapWonText, twoString, Point(3000, 1400), 2, 1, Scalar(138, 21, 17), 1, CV_AA);
		putText(mapWonText, threeString, Point(3000, 1600), 2, 1, Scalar(138, 21, 17), 1, CV_AA);

		putText(mapWonText, scores0, Point(3300, 1200), 2, 1, Scalar(138, 21, 17), 1, CV_AA);
		putText(mapWonText, scores1, Point(3300, 1400), 2, 1, Scalar(138, 21, 17), 1, CV_AA);
		putText(mapWonText, scores2, Point(3300, 1600), 2, 1, Scalar(138, 21, 17), 1, CV_AA);
		imshow(backgroundWindowName, mapWonText);
		mapWonText = mapWon.clone();*/
		
		Mat mapWon = imread("mapWon.png", CV_LOAD_IMAGE_COLOR);
		if (scores[0] == difference_time3) {
			putText(mapWon, scoreTextString, Point(500, 300), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, oneString, Point(500, 360), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);
			putText(mapWon, twoString, Point(500, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, threeString, Point(500, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, scores0, Point(575, 360), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);
			putText(mapWon, scores1, Point(575, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores2, Point(575, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			imshow(backgroundWindowName, mapWon);
		}
		else if (scores[1] == difference_time3){
			putText(mapWon, scoreTextString, Point(500, 300), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, oneString, Point(500, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, twoString, Point(500, 420), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);
			putText(mapWon, threeString, Point(500, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, scores0, Point(575, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores1, Point(575, 420), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);
			putText(mapWon, scores2, Point(575, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			imshow(backgroundWindowName, mapWon);
		}
		else if (scores[2] == difference_time3){
			putText(mapWon, scoreTextString, Point(500, 300), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, oneString, Point(500, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, twoString, Point(500, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, threeString, Point(500, 480), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);

			putText(mapWon, scores0, Point(575, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores1, Point(575, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores2, Point(575, 480), 2, 1.25, Scalar(255, 180, 17), 1, CV_AA);
			imshow(backgroundWindowName, mapWon);
		}
		else if (scores[3] == difference_time3){
			putText(mapWon, scoreTextString, Point(500, 300), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, oneString, Point(500, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, twoString, Point(500, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, threeString, Point(500, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);

			putText(mapWon, scores0, Point(575, 360), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores1, Point(575, 420), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			putText(mapWon, scores2, Point(575, 480), 2, 1.25, Scalar(138, 21, 17), 1, CV_AA);
			imshow(backgroundWindowName, mapWon);
		}

		//load text for won map and display scoretimer
		//std::string scoreString = std::to_string(score_output);
		//putText(mapWon, scoreString, Point(100, 100), 1, 2, Scalar(0, 0, 255), 2);

		//(highscore array?)
		//check if tracking lost with delaycheck, if yes set state 1, reset pointscounter, timer, if no do nothing except delaycheck
		if (objectFound == false) {
			cout << "state = 2: objectFound is false" << endl;
			checkDelayEnd();
			if (delay_result == true){
				cout << "state = 2: delay_result is true" << endl;
				state = 1;
				points_counter = 0;
				delay_result = false;
				difference_time = 0;
				difference_time2 = 0;
				difference_time3 = 0;
				level_time = 0;
				
				// score timer
			}
			else {
				cout << "state = 2: delay_result is false" << endl;
			}
		}
		else {
			difference_time2 = 0;
			cout << "state = 2: objectFound is true" << endl;
		}
		//stop delaycheck
	}
	else if (state == 3) { //game over
		cout << "went to state 3" << endl;
		//start delaycheck
		checkDelayStart();
		cout << "started delaycheck in state 3" << endl;
		cout << "map read in state 3" << endl;
		imshow(backgroundWindowName, mapLost);
		cout << "imshow in state 3" << endl;
		//waitkey(30);
		cout << "map loaded in state 3" << endl;
		//stop scoretimer
		//load text for game over and display scoretimer
		///std::string scoreString = std::to_string(score_output);
		//putText(mapWon, scoreString, Point(100, 100), 1, 2, Scalar(0, 0, 255), 2);

		//(highscore array?)
		//check if tracking lost with delaycheck, if yes set state 1, reset pointscounter, timer, if no do nothing except delaycheck
		if (objectFound == false) {
			cout << "state = 3: objectFound is false" << endl;
			checkDelayEnd();
			if (delay_result == true){
				cout << "state = 3: delay_result is true" << endl;
				state = 1;
				points_counter = 0;
				delay_result = false;
				difference_time = 0;
				difference_time2 = 0;
				difference_time3 = 0;
				level_time = 0;
				// score timer
			}
			else{
				cout << "state = 3: delay_result is false" << endl;
			}
		}
		else {
			difference_time2 = 0;
			cout << "state = 3: objectFound is true" << endl;
		}
		//stop delaycheck
	
	}
	
}

//check if timer over, if yes set state 3





void createTrackbars(){
	//create window for trackbars


	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, 255, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, 255, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, 255, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, 255, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, 255, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, 255, on_trackbar);


}
void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param){
	//int64 time_start = cv::getTickCount();
	//only if calibration mode is true will we use the mouse to change HSV values
	if (calibrationMode == true){
		//get handle to video feed passed in as "param" and cast as Mat pointer
		Mat* videoFeed = (Mat*)param;

		if (event == CV_EVENT_LBUTTONDOWN && mouseIsDragging == false)
		{
			//keep track of initial point clicked
			initialClickPoint = cv::Point(x, y);
			//user has begun dragging the mouse
			mouseIsDragging = true;
		}
		/* user is dragging the mouse */
		if (event == CV_EVENT_MOUSEMOVE && mouseIsDragging == true)
		{
			//keep track of current mouse point
			std::cout << 120 << endl;
			currentMousePoint = cv::Point(x, y);
			//user has moved the mouse while clicking and dragging
			mouseMove = true;
		}
		/* user has released left button */
		if (event == CV_EVENT_LBUTTONUP && mouseIsDragging == true)
		{
			//set rectangle ROI to the rectangle that the user has selected
			rectangleROI = Rect(initialClickPoint, currentMousePoint);

			//reset boolean variables
			mouseIsDragging = false;
			mouseMove = false;
			rectangleSelected = true;
		}

		if (event == CV_EVENT_RBUTTONDOWN){
			//user has clicked right mouse button
			//Reset HSV Values
			H_MIN = 0;
			S_MIN = 0;
			V_MIN = 0;
			H_MAX = 255;
			S_MAX = 255;
			V_MAX = 255;

		}
		if (event == CV_EVENT_MBUTTONDOWN){

			//user has clicked middle mouse button
			//enter code here if needed.
		}
	}
	//createTimer();

}
void recordHSV_Values(cv::Mat frame, cv::Mat hsv_frame){
	//int64 time_start = cv::getTickCount();

	//save HSV values for ROI that user selected to a vector
	if (mouseMove == false && rectangleSelected == true){

		//clear previous vector values
		if (H_ROI.size()>0) H_ROI.clear();
		if (S_ROI.size()>0) S_ROI.clear();
		if (V_ROI.size()>0)V_ROI.clear();
		//if the rectangle has no width or height (user has only dragged a line) then we don't try to iterate over the width or height
		if (rectangleROI.width<1 || rectangleROI.height<1) cout << "Please drag a rectangle, not a line" << endl;
		else{
			for (int i = rectangleROI.x; i<rectangleROI.x + rectangleROI.width; i++){
				//iterate through both x and y direction and save HSV values at each and every point
				for (int j = rectangleROI.y; j<rectangleROI.y + rectangleROI.height; j++){
					//save HSV value at this point
					H_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[0]);
					S_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[1]);
					V_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[2]);
				}
			}
		}
		//reset rectangleSelected so user can select another region if necessary
		rectangleSelected = false;
		//set min and max HSV values from min and max elements of each array

		if (H_ROI.size()>0){
			//NOTE: min_element and max_element return iterators so we must dereference them with "*"
			H_MIN = *std::min_element(H_ROI.begin(), H_ROI.end());
			H_MAX = *std::max_element(H_ROI.begin(), H_ROI.end());
			cout << "MIN 'H' VALUE: " << H_MIN << endl;
			cout << "MAX 'H' VALUE: " << H_MAX << endl;
		}
		if (S_ROI.size()>0){
			S_MIN = *std::min_element(S_ROI.begin(), S_ROI.end());
			S_MAX = *std::max_element(S_ROI.begin(), S_ROI.end());
			cout << "MIN 'S' VALUE: " << S_MIN << endl;
			cout << "MAX 'S' VALUE: " << S_MAX << endl;
		}
		if (V_ROI.size()>0){
			V_MIN = *std::min_element(V_ROI.begin(), V_ROI.end());
			V_MAX = *std::max_element(V_ROI.begin(), V_ROI.end());
			cout << "MIN 'V' VALUE: " << V_MIN << endl;
			cout << "MAX 'V' VALUE: " << V_MAX << endl;
		}

	}

	if (mouseMove == true){
		//if the mouse is held down, we will draw the click and dragged rectangle to the screen
		rectangle(frame, initialClickPoint, cv::Point(currentMousePoint.x, currentMousePoint.y), cv::Scalar(0, 255, 0), 1, 8, 0);
	}

	//createTimer();
}
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void drawObject(int x, int y, Mat &frame){
	//int64 time_start = cv::getTickCount();

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!


	//'if' and 'else' statements to prevent
	//memory errors from writing off the screen (ie. (-25,-25) is not within the window)

	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
	//createTimer();

}
void morphOps(Mat &thresh){
	//int64 time_start = cv::getTickCount();

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);


	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);



	//createTimer();
}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){
	//int64 time_start = cv::getTickCount();

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	int largestIndex = 0;
	objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we save a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
					//save index of largest contour to use with drawContours
					largestIndex = index;
				}
				else objectFound = false;


			}
			//let user know you found an object
			if (objectFound == true){
				putText(cameraFeed, "Tracking Object", Point(500, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject(x, y, cameraFeed);
				//draw largest contour
				//drawContours(cameraFeed, contours, largestIndex, Scalar(0, 255, 255), 2);
			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
	//createTimer();
}
int main(int argc, char* argv[])
{

	
	//some boolean variables for different functionality within this
	//program
	bool trackObjects = true;
	bool useMorphOps = true;
	calibrationMode = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x = 0, y = 0;
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	//must create a window before setting mouse callback
	cv::namedWindow(windowName);
	//set mouse callback function to be active on "Webcam Feed" window
	//we pass the handle to our "frame" matrix so that we can draw a rectangle to it
	//as the user clicks and drags the mouse
	cv::setMouseCallback(windowName, clickAndDrag_Rectangle, &cameraFeed);
	//initiate mouse move and drag to false 
	mouseIsDragging = false;
	mouseMove = false;
	rectangleSelected = false;
	

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1){
		createBackground();
		gameState();

		
		
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		//set HSV values from user selected region
		recordHSV_Values(cameraFeed, HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if (useMorphOps)
			morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if (trackObjects)
			trackFilteredObject(x, y, threshold, cameraFeed);
		//cout << objectFound;
		//show frames 
		if (calibrationMode == true){

			//create slider bars for HSV filtering
			createTrackbars();
			
			imshow(windowName1, HSV);
			imshow(windowName2, threshold);
		}
		else{

			destroyWindow(windowName1);
			destroyWindow(windowName2);
			destroyWindow(trackbarWindowName);
		}
		imshow(windowName, cameraFeed);

			



		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		//also use waitKey command to capture keyboard input
		if (waitKey(30) == 99) calibrationMode = !calibrationMode;//if user presses 'c', toggle calibration mode
	}





	
	return 0;
}