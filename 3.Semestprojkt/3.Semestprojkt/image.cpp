#include "image.h"

using namespace std;
using namespace cv;


image::image() {
}

image::~image() {
}

bool image::getCalibration(string fileName) {
	ifstream myfile;
	myfile.open(fileName);

	if (myfile.fail()) {							// checks for iostate failbit flag
		cout << "Failed to load file!" << endl;
		return false;
	}

	// the first 5 floats are the distortion coefficient
	for (int i = 0; i < 5; i++) {
		myfile >> _distortionCoefficient(i);
	}
	//cout << _distortionCoefficient << endl;

	// the next 9 floats are the camera matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> _cameraMatrix(i, j);
		}
	}
	//cout << _cameraMatrix << endl;

	// the next  9 floats are the perspective matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> _perspectiveMatrix(i, j);
		}
	}
	//cout << _perspectiveMatrix << endl;

	myfile.close();

	return true;
}

bool image::getImg() {
	//VideoCapture cap(1); // open the default camera (0), (1) for USB
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1440);	
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	//if (!cap.isOpened()) {  // check if we succeeded
	//	return false;
	//}

	//cap >> inputImg; // get a new frame from camera
	Mat inputImg;
	Mat undistImg;
	inputImg = imread("C:/Users/rasmu/Desktop/img1.tiff");

	//imshow("The captured image", inputImg);
	//waitKey(0);
	
	undistort(inputImg, undistImg, _cameraMatrix, _distortionCoefficient);		// removes lens - distortion
	
	//imshow("Undistorted image", undistImg);
	//waitKey(0);

	warpPerspective(undistImg, _srcImg, _perspectiveMatrix, undistImg.size());	// warps in respect to the angle between lens and object surface
	
	//imshow("warped image", _srcImg);
	//waitKey(0);

	return true;
}

void image::convertHSV() {			// Hue range is [0,179], Saturation range is [0,255] and Value range is [0,255]
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);

	//imshow("HSV image", _hsvImg);
	//waitKey(0);
}

void image::maskColour(string object) {
	// Scalars are found using the HSV colormap
	// must be called when looking for either "ball" or "cup"
	if (object == "ball") {
		inRange(_hsvImg, Scalar(10, 150, 0), Scalar(25, 255, 200), _mask); // Scalar(10, 150, 0), Scalar(25, 255, 200)

		bitwise_and(_srcImg, _srcImg, _dstImg, _mask);

		//imshow("masked image", _dstImg);
		//waitKey(0);
	}
	else if (object == "cup") {
		inRange(_hsvImg, Scalar(0, 0, 0), Scalar(179, 255, 255), _mask); // Scalar(0, 0, 0), Scalar(100, 150, 255)

		bitwise_and(_srcImg, _srcImg, _dstImg, _mask);

		//imshow("masked image", _dstImg);
		//waitKey(0);
	}

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2));

	morphologyEx(_dstImg, _dstImg, MORPH_OPEN, kernel);

	morphologyEx(_dstImg, _dstImg, MORPH_CLOSE, kernel);
}

void image::convertGray() {
	cvtColor(_dstImg, _grayImg, COLOR_BGR2GRAY);
}

std::vector<cv::Vec3f> image::detectCircles(string object) {
	vector<Vec3f> circles;

	// depending of the object we are looking for (ball or cup), we look for different sizes.
	if (object == "ball") {
		GaussianBlur(_grayImg, _grayImg, cv::Size(5, 5), 2, 2);

		HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 25, 18, 22); // the last two ints determine size of the circles we accept
	}
	else if (object == "cup") {
		medianBlur(_grayImg, _grayImg, 5);

		HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 25, 40, 45);  
	}

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
	 }
	return circles;
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