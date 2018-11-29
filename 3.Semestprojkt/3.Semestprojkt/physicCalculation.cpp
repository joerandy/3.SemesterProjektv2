#include "physicCalculation.h"



physicCalculation::physicCalculation()
{
	//default constructor
}

cv::Vec<double, 5> physicCalculation::calc(double targetX, double targetY)
{
	double rotate, // Rotate base to (rad)
	double throwDist, // Distance tcp to target (m)
	double tangVel, // Tangential velocity (m/s)
	double angleVel, // Angle velocity: (rad/s)
	double angleAcc, // Angle acceleration (fixed startpos): (rad/s^2)
	double startAng) // Startposition (fixed max acceleraion): (rad)
	
	return cv::Vec<double, 5>();
}


physicCalculation::~physicCalculation()
{
}
