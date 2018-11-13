#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
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
	bool getCalibration(std::string fileName);	// reads from input file
	bool getImg();								// takes picture
	void convertHSV();							// converts to HSV
	void maskColour(std::string object);		// creates mask
	void convertGray();							// converts to grayscale
	std::vector<cv::Vec3f> detectCircles(std::string object);		// deetect circles
	coordinates getCoordinates();
	coordinates getBallCoordinates();
	void display();								// display picture

private:
	cv::Matx33f _cameraMatrix;						// camera-matrix 
	cv::Vec<float, 5> _distortionCoefficient;		// Distortion-coefficient
	cv::Matx33f _perspectiveMatrix;					// Perspective matrix
	cv::Mat _srcImg;								// stores input picture
	cv::Mat _hsvImg;								// stores picture converted to hsv
	cv::Mat _mask;									// stores picture of color-mask
	cv::Mat _grayImg;								// stores picture converted to grayscale
	cv::Mat _dstImg;								// stores output pictue

	int _x;
	int _y;
	int _r;
	int _d;
};

