#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/videoio.hpp>


class image
{
public:
	image();
	~image();
	bool getImg();
	void calibrate();
	void convertHSV();
	void convertGray();
	void maskColour();
	void detectCircles();
	void display();
private:
	cv::Mat _srcImg;
	cv::Mat _mask;
	cv::Mat _dstImg;
	cv::Mat _hsvImg;
	cv::Mat _grayImg;
	int _x;
	int _y;
	int _r;
	int _d;
};

