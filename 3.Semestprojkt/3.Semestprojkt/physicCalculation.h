#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <cmath>
class physicCalculation
{
public:
	physicCalculation();
	std::vector<double> calc(double targetX, double targetY, double cupZ);
	double getAngleVel();
	double getAngleAcc();
	~physicCalculation();
private:
	double _targetX;
	double _targetY;
	double _rotate;
	double _angleVel;
	double _angleAcc;

	std::vector<double> result;
};

