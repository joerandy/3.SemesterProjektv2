#include "image.h"

using namespace std;
using namespace cv;


image::image() {
}

image::~image() {
}

bool image::getCalibration(string fileName) {
	float buffer;
	ifstream myfile;
	myfile.open(fileName);

	if (myfile.fail()) {							// checks for iostate failbit flag
		cout << "Failed to load file!" << endl;
		return false;
	}

	// the first 5 floats are the distortion coefficient
	for (int i = 0; i < 5; i++) {
		myfile >> buffer;
		_distortionCoefficient(i) = buffer;
	}
	cout << _distortionCoefficient << endl;

	// the next 9 floats are the camera matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> buffer;
			_cameraMatrix(i, j) = buffer;
		}
	}
	cout << _cameraMatrix << endl;

	// the next  9 floats are the perspective matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> buffer;
			_perspectiveMatrix(i, j) = buffer;
		}
	}
	cout << _perspectiveMatrix << endl;

	myfile.close();

	return true;
}

bool image::getImg() {
	VideoCapture cap(0); // open the default camera (0), (1) for USB
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1420);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	if (!cap.isOpened()) {  // check if we succeeded
		return false;
	}

	Mat inputImg;
	Mat undistImg;
	cap >> inputImg; // get a new frame from camera
	inputImg = imread("C:/Users/rasmu/Dropbox/RobTek/3. Semester/Semesterprojekt/semesterprojekt/Basler10.tiff", 1);
	
	undistort(inputImg, undistImg, _cameraMatrix, _distortionCoefficient);		// removes lens - distortion
	
	warpPerspective(undistImg, _srcImg, _perspectiveMatrix, undistImg.size());	// warps in respect to the angle between lens and object surface

	return true;
}

void image::convertHSV() {
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);
}

void image::maskColour() {
	inRange(_hsvImg, Scalar(10, 150, 0), Scalar(25, 255, 200), _mask); // Scalars are found using the HSV colormap
																	   // Blue: Scalar(100, 150, 0), Scalar(140, 255, 255)
																	   // Orange: Scalar(10, 150, 60), Scalar(25,255,200)
																	   // Hvid: Scalar(0, 0, 60), Scalar(180, 30, 255)

	bitwise_and(_srcImg, _srcImg, _dstImg, _mask);
}

<<<<<<< HEAD
void image::convertGray() {
	cvtColor(_dstImg, _grayImg, COLOR_BGR2GRAY);
}

void image::detectCircles() {
=======
std::vector<cv::Vec3f> image::detectCircles() {
>>>>>>> 133bf26f2dfcea26da78977997e1530bca57258d
	vector<Vec3f> circles;

	GaussianBlur(_grayImg, _grayImg, cv::Size(9, 9), 2, 2);

	HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 18, 22); // see opencv documentation: 15 = radius min. 30 = radius max. 
	
	return circles;
	/*
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		int diameter = radius * 2;
		cout << "Cirkel nr. " << i << "'s diameter er: " << diameter << endl;
		cout << "Center = " << center << endl;
		// circle center
		circle(_dstImg, center, 3, Scalar(255, 0, 0), -1, 8, 0);
		// circle outline
		circle(_dstImg, center, radius, Scalar(0, 255, 0), 3, 8, 0);	// black circle around circular opbject
		_x = center.x;
		_y = center.y;
		_r = radius;
		_d = diameter;
	}*/
}

coordinates image::getCoordinates() {
	coordinates pos;
	pos.x = _x;
	pos.y = _y;
	return pos;
}

void image::display() {
	imshow("Display _dst", _dstImg);
	waitKey(0);
}