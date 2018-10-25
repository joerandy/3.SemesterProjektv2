#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/videoio.hpp>

struct coordinates {
	int x;
	int y;
};

class image
{
public:
	image();
	~image();
	bool getImg();
	void calibrate();
	void convertHSV();
	void maskColour();
	void convertGray();
	void detectCircles();
	coordinates getCoordinates();
	void display();
private:
	cv::Mat _srcImg;
	cv::Mat _hsvImg;
	cv::Mat _mask;
	cv::Mat _grayImg;
	cv::Mat _dstImg;

	int _x;
	int _y;
	int _r;
	int _d;
};

