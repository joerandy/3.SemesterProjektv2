#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
class physicCalculation
{
public:
	physicCalculation();
	cv::Vec<double, 5> calc(double targetX, double targetY);

	~physicCalculation();
private:
	double _targetX;
	double _targetY;
	double _rotate;
	double _angelVel;
	double _angleAcc;
};
