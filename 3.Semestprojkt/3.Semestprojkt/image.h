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
	float x = 0.0;
	float y = 0.0;
	float diameter = 0.0;
};

class image
{
public:
	image();
	~image();
	
	//called from detectCircles(), no need to call externally
	bool getCalibration(std::string fileName);	// reads from input file
	//called from detectCircles, no longer need to call externally
	bool getImg();								// takes picture
	//called from detectCircles() so no longer need to call it externally
	void maskColour(std::string object);		// creates mask

	//replaces getBallCoordinates() and getCupCoordinates(), untested with cups
	coordinates getCoordinates(std::string object);
	void display();								// display picture

	std::vector<cv::Vec3f> getCups();

	//deprecated, merged into getImg() -- can be removed
	void convertHSV();							// converts to HSV
	//deprecated, merged into detectCircles() -- can be removed
	void convertGray();							// converts to grayscale
	std::vector<cv::Vec3f> detectCircles(std::string object);		// deetect circles


private:
	cv::Mat _inputImg;								// input image
	cv::Matx33f _cameraMatrix;						// camera-matrix 
	cv::Vec<float, 5> _distortionCoefficient;		// Distortion-coefficient
	cv::Matx33f _perspectiveMatrix;					// Perspective matrix
	cv::Mat _srcImg;								// stores input picture
	cv::Mat _hsvImg;								// stores picture converted to hsv
	cv::Mat _mask;									// stores picture of color-mask
	cv::Mat _grayImg;								// stores picture converted to grayscale
	cv::Mat _dstImg;								// stores output pictue
	std::vector<cv::Vec3f> cups;
	float _x;
	float _y;
	float _r;
	float _d;
};

