#include "physicCalculation.h"



physicCalculation::physicCalculation()
{
	//default constructor
}

std::vector<double> physicCalculation::calc(double targetX, double targetY, double cupZ)
{
	result.clear();
	_targetX = targetX;
	_targetY = targetY;
	//_rotate; // Rotate base to (rad)
	double throwDist; // Distance tcp to target (m)
	double tangVel; // Tangential velocity (m/s)
	//_angleVel; // Angle velocity: (rad/s)
	//_angleAcc; // Angle acceleration (fixed startpos): (rad/s^2)
	double startAng; // Startposition (fixed max acceleraion): (rad)
	
	const double base = 0.09; // UR5 base link
	const double upArm = 0.425; // UR link2
	const double lowArm = 0.390; // UR link3
	const double hand = 0.095; // UR link5
	const double tcp = 0.185; // WSG 10 gripper
	const double handAndTcp = sqrt(hand*hand + tcp * tcp); // combined hypotenuse of hand and gripper

	const double height = base + upArm + lowArm + handAndTcp; // maximum height
	const double radius = height - base; // radius
	const double PI = 3.141592653589793; // pi
	const double maxRadVel = PI; // maximum joint-velocity

	const double dropHeight = height - cupZ;
	const double dropTime = sqrt((2 * (dropHeight)) / 9.82);

	double t = 0; // cos(t) = 1 (maximum length of robotarm)
	const double firstJoint = upArm + lowArm + handAndTcp; // length of static arm rotated at first joint
	const double secondJoint = lowArm + handAndTcp; // length of static arm rotated at second joint
	const double thirdJoint = handAndTcp; // length of static arm rotated at third joint
	const double effectRadius = firstJoint + secondJoint + thirdJoint; // radius is replaced by the resulting leverage of 3 synchronized joints
	//double effektVel = maxRadVel * effectRadius; // v = w * r
	
	//double ratio = effektVel / maxRadVel;

	double offset = 0.112; // Fysik lektion 10 slide 11 states 0.10915

	double baseDist = sqrt(pow(_targetX, 2) + pow(_targetY, 2));
	double baseAngle = atan(_targetY / _targetX);

	// baseDist is the hypotenuse in a right triangle with sides 'offset' and 'distance'
	double tcpDist = sqrt(pow(baseDist, 2) - pow(offset, 2));

	// corrAngle is the angle between baseDist and tcpDist
	double corrAngle = asin(offset / tcpDist);

	// effectAngle = baseAngel - corrAngle
	_rotate = baseAngle + corrAngle;
	
	// velocity is the needed velocity [m/s], to obtain a throw length at 'distance' [m]
	double velocity = tcpDist / dropTime;

	// anglevelocity is the calculated rotational velocity [rad/s] to obtain required tangential velocity.
	_angleVel = velocity / effectRadius;

	// using 2-16, p22: v^2 = v_0^2 + 2a (x-x_0) => 1/2* ( v^2 / (pi/4) )
	_angleAcc = 0.5*((_angleVel*_angleVel) / (PI / 4));
	


	result.push_back(_rotate);
	result.push_back(_angleVel);
	result.push_back(_angleAcc);
	result.push_back(_angleVel / _angleAcc);
	
	return result;
}

double physicCalculation::getAngleVel()
{
	return _angleVel;
}

double physicCalculation::getAngleAcc()
{
	return _angleAcc;
}


physicCalculation::~physicCalculation()
{
}
