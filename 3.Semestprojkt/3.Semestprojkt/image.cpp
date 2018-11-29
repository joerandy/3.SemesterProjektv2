#include "image.h"

using namespace std;
using namespace cv;


image::image() {
}

image::~image() {
}

//called from detectCircles(), no need to call externally
bool image::getCalibration(string fileName) {
	ifstream myfile;
	myfile.open(fileName);

	if (myfile.fail()) {							// checks for iostate failbit flag
		return false;
	}

	// the first 5 floats are the distortion coefficient
	for (int i = 0; i < 5; i++) {
		myfile >> _distortionCoefficient(i);
	}

	// the next 9 floats are the camera matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> _cameraMatrix(i, j);
		}
	}
	// the next  9 floats are the perspective matrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> _perspectiveMatrix(i, j);
		}
	}
	myfile.close();
	return true;
}

//called from detectCircles, no need to call externally
bool image::getImg() {
	VideoCapture cap(1); // open the default camera (0), (1) for USB
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1440);	
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	if (!cap.isOpened()) {  // check if we succeeded
		return false;
	}

	Mat undistImg;
	cap >> _inputImg; // get a new frame from camera

	//inputImg = imread("C:/Users/Rasmu/OneDrive/Desktop/Basler10.tiff");  //for testing without USB camera connected
	undistort(_inputImg, undistImg, _cameraMatrix, _distortionCoefficient);		// removes lens - distortion
	warpPerspective(undistImg, _srcImg, _perspectiveMatrix, undistImg.size());	// warps in respect to the angle between lens and object surface
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);

	return true;
}

//called from detectCircles() no need to call externally
void image::maskColour(string object) {
	// Scalars are found using the HSV colormap
	// must be called when looking for either "ball" or "cup"
	if (object == "ball") {
		inRange(_hsvImg, Scalar(5,150,0), Scalar(35, 255, 255), _mask); // Scalar(5, 150, 0), Scalar(35, 255, 255)

		_dstImg.release();

		bitwise_and(_srcImg, _srcImg, _dstImg, _mask);
	}
	else if (object == "cup") {
		inRange(_hsvImg, Scalar(0, 0, 0), Scalar(179, 255, 255), _mask); // Scalar(0, 0, 0), Scalar(100, 150, 255)

		_dstImg.release();

		bitwise_and(_srcImg, _srcImg, _dstImg, _mask);
	}
// kommenter 
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)); //muligvis sættes size op 
// kommenter 
	morphologyEx(_dstImg, _dstImg, MORPH_OPEN, kernel);
// kommenter 
	morphologyEx(_dstImg, _dstImg, MORPH_CLOSE, kernel);
}

//deprecated, merged into getImg() -- can be removed
void image::convertHSV() {			// Hue range is [0,179], Saturation range is [0,255] and Value range is [0,255]
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);
}

//deprecated, merged into detectCircles() -- can be removed
void image::convertGray() {
	cvtColor(_dstImg, _grayImg, COLOR_BGR2GRAY);
}

std::vector<cv::Vec3f> image::detectCircles(string object) {
	getImg();
	maskColour(object);
// FORSKELLIGT!!!
	cvtColor(_dstImg, _grayImg, COLOR_BGR2GRAY);

	vector<Vec3f> circles;
		// depending of the object we are looking for (ball or cup), we look for different sizes.
		if (object == "ball") {
			cout << "balls called" << endl;

			GaussianBlur(_grayImg, _grayImg, cv::Size(5, 5), 2, 2);

			HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 25, 18, 22); // the last two ints determine size of the circles we accept
			
			if (!circles.empty()) {
				cout << "circles (balls) found!" << endl;
				_x = circles[0][0]/1000;
				_y = (circles[0][1]/1000) - 0.95;
				_r = circles[0][2]/1000;
				_d = (circles[0][2]/1000) * 2;
			}
			else {
				cout << "No circles found..." << endl;
			}
		}
		else if (object == "cup") {
			cout << "cups called" << endl;
			medianBlur(_grayImg, _grayImg, 5);
			HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 25, 40, 45);
			if (!circles.empty()) {
				cout << "circles (cups) found!" << endl;
				for (int i = 0; i < circles.size(); i++)
				{
					circles[i][0] = circles[i][0] / 1000;
					circles[i][1] = (circles[i][1] / 1000) - 0.95;
					circles[i][2] = circles[i][2] / 1000;
					cups = circles;
				}
			}
			else {
				cout << "No circles found..." << endl;
			}
		}
		cout << "test print: " << _x << " " << _y << " " << _d << endl;
	return circles;
}

//replaces getBallCoordinatesTest() and getCupCoordinatesTest()
coordinates image::getCoordinates(string object) {
	if (getCalibration("input.txt")) {
		cout << "Calibration data loaded!" << endl;
	}
	else {
		cout << "Error in loading calibration data..." << endl;
	}
	// everything is called from detectcircles but does it have to
	//getImg();
	//convertHSV();
	//maskColour("ball");
	detectCircles(object);
	

	return coordinates{ _x, _y, _d};
}

//method for displaying image at a given time, for testing purposes
void image::display() {
	imshow("Display _dst", _dstImg);
	waitKey(0);
}

std::vector<cv::Vec3f> image::getCups()
{
	return cups;
}
