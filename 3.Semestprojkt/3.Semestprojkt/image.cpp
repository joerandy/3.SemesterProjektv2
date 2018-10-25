#include "image.h"

using namespace std;
using namespace cv;


image::image() {
}


image::~image() {
}

bool image::getImg() {
	VideoCapture cap(0); // open the default camera (0), (1) for USB
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1420);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	if (!cap.isOpened()) {  // check if we succeeded
		return false;
	}

	//while (true) {
	//	cap >> _srcImg; // get a new frame from camera
	//	imshow("frame", _srcImg);
	//	if (waitKey(30) >= 0) {
	//		break;
	//	}
	//}

	cap >> _srcImg; // get a new frame from camera
	//_srcImg = imread("C:/Users/rasmu/Dropbox/RobTek/3. Semester/Semesterprojekt/semesterprojekt/Basler10.tiff", 1);

	return true;
}

void image::calibrate() {

	vector<String> fileNames;
	const string path = "c:/images/calibration/Image*.png";
	cv::setBreakOnError(true);
	cv::glob(path, fileNames, false);
	Size patternSize(25 - 1, 18 - 1); // corners inside squares
	vector<vector<Point2f>> q(fileNames.size());

	// Detect feature points
	size_t i = 0;

	for (auto const &f : fileNames) {
		//        cout << string(f) << endl;

		// 1. Read in the image and call findChessboardCorners()
		Mat img;
		img = imread(f, CV_LOAD_IMAGE_ANYDEPTH);
		
		bool success = findChessboardCorners(img, patternSize, q[i]);

		// 2. Use cornerSubPix() to refine the found corner detection
		Size zeroZone(15, 15);
		cornerSubPix(img, Mat(q[i]), patternSize, zeroZone, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)); // 30 = # of iterations, 0.1 = improvements pr. iteration

																														// Display
		drawChessboardCorners(img, patternSize, Mat(q[i]), success);
		        imshow("Chessboard detection "+f, img);
		        waitKey(0);

		i++;
	}

	vector<vector<Point3f>> Q;

	// 3. Generate checkerboard (world) coordinates Q. The board has 25x18
	// fields with a size of 15x15mm (Z-coordinate = 0)

	Matx33f K(Matx33f::eye()); // intrinsic camera matrix
	Vec<float, 5> k(0, 0, 0, 0, 0); // distortion coefficients

	vector<Mat> rvecs, tvecs;
	vector<double> stdIntrinsics, stdExtrinsics, perViewErrors;
	int flags = CALIB_FIX_ASPECT_RATIO + CALIB_FIX_K3 + CALIB_ZERO_TANGENT_DIST + CALIB_FIX_PRINCIPAL_POINT;
	Size frameSize(1440, 1080);

	cout << "Calibrating..." << endl;

	// 4. Call "float error = cv::calibrateCamera()" with the input coordinates
	// and output parameters as declared above...

	// 3D coordinates of chessboard points
	//board size
	int x = 25; //length
	int y = 18; //width
	int a = 15; //length of small squares
	std::vector<cv::Point3f> objectPoints;
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			objectPoints.push_back(cv::Point3f(j*a, i*a, 0));
		}
	}

	//    vector<Point2f> image_point;
	//    for (int i = 0; i < x; i++) {
	//        image_point.push_back(Point2f(q[i], q[i]));
	//    }
	//    imagePoints.push_back(image_point);

	Size imageSize = frameSize;
	Matx33f cameraMatrix = K;
	Vec<float, 5> distCoeffs = k;

	cout << "pre calibrateCamera" << endl;

	cout << "q size: " << q.size() << endl;
	cout << "objectPoints size: " << objectPoints.size() << endl;
	//    cout << "obj[i] size: " << objectPoints[1]. << endl;

	float error = calibrateCamera(objectPoints, q, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);
	cout << "Reprojection error = " << error << "\nK=\n"
		<< K << "\nK=\n"
		<< k << endl;

	cout << "post calibrateCamera" << endl;

	// Precompute lens correction interpolation
	Mat mapX, mapY;
	initUndistortRectifyMap(K, k, Matx33f::eye(), K, frameSize, CV_32FC1, mapX, mapY);

	cout << "end" << endl;

	// Show lens corrected images
	//    for (auto const &f : fileNames) {
	//        cout << string(f) << endl;

	//        Mat img = imread(f, IMREAD_COLOR);

	//        Mat imgUndistorted;
	//        // 5. Remap the image using the precomputed interpolation maps.

	//        // Display
	//        imshow("undistorted image", imgUndistorted);
	//        waitKey(0);
	//    }
}

void image::convertHSV() {
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);
}

void image::convertGray() {
	cvtColor(_dstImg, _grayImg, COLOR_BGR2GRAY);
}

void image::maskColour() {
	//inRange(_hsvImg, Scalar(10, 150, 0), Scalar(25, 255, 200), _mask); // Scalars are found using the HSV colormap
	inRange(_hsvImg, Scalar(100, 150, 0), Scalar(140, 255, 255), _mask);
	// Blue: Scalar(100, 150, 0), Scalar(140, 255, 255)
	// Orange: Scalar(10, 150, 60), Scalar(25,255,200)
	// Hvid: Scalar(0, 0, 60), Scalar(180, 30, 255)

	//GaussianBlur(_mask, _mask, cv::Size(9, 9), 2, 2);

	bitwise_and(_srcImg, _srcImg, _dstImg, _mask);
}

void image::detectCircles() {
	vector<Vec3f> circles;

	GaussianBlur(_grayImg, _grayImg, cv::Size(9, 9), 2, 2);

	HoughCircles(_grayImg, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0); // see opencv documentation: 15 = radius min. 30 = radius max. 

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
}

coordinates image::getCoordinates() {
	coordinates pos;
	pos.x = _x;
	pos.y = _y;
	return pos;
}

void image::display() {
	imshow("gray", _grayImg);
	imshow("Display _dst", _dstImg);
	waitKey(0);
}