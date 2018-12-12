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
	//Image is inteded to be used to get an image from a USB camera, apply undistortion/unwarping based on an input file, convert the image to HSV, apply a color mask,
	//convert to greyscale and finally detecting either ball or cup sized circles.
	//Use: Run getCoordinates() with either ("ball") or ("cup") to get coordinates for the objects relative to the UR5 robot.
	//For balls use getCoordinates() return value. Get a vector of cup coordinates by calling getCups().
public:
	image();
	~image();

	//loads distortion coeffecients and camera matrix from input file
	bool getCalibration(std::string fileName);	// reads from input file

	//take a picture from USB camera, undistort and unwarp
	bool getImg();								// takes picture

	//superimpose mask to filter objects by color
	void maskColour(std::string object);		// creates mask

	//gets coordinates for the given object, "ball" or "cup". Ball coordinates is returned, cup coordinates are accessed through img.getCups() after this method has run
	coordinates getCoordinates(std::string object);

	//displays an image. used for testing
	void display();								// display picture

	//returns a vector of cup coordinates. Cups must be found with getCoordinates before this method is run.
	std::vector<cv::Vec3f> getCups();

	//converts image to HSV
	void convertHSV();	

	//converts image to greyscale
	void convertGray();							

	//detects circles in the masked image looking for objects that match the size of either "ball" or "cup" objects
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
	std::vector<cv::Vec3f> _cups;
	float _x;
	float _y;
	float _r;
	float _d;
};

