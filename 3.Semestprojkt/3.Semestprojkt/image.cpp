#include "image.h"

using namespace std;
using namespace cv;


image::image(){
}


image::~image(){
}

bool image::getImg() {
	VideoCapture cap(0); // open the default camera (0), (1) for USB
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1448);
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
	_dstImg = _srcImg;

	return true;
}


void image::calibrate() {
	vector<String> fileNames;
	glob("C:/images/calibration/Image*.png", fileNames, false);
	Size patternSize(25 - 1, 18 - 1); // corners inside squares
	vector<vector<Vec2f>> q(fileNames.size());

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
		//        imshow("Chessboard detection "+f, img);
		//        waitKey(0);

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


void image::convert() {
	cvtColor(_srcImg, _hsvImg, COLOR_BGR2HSV);
}

void image::detectColour() {
	inRange(_hsvImg, Scalar(15, 150, 60), Scalar(20, 250, 255), _dstImg); // Scalars are found using the HSV colormap
	//Orange: Scalar(10,200,60), Scalar(25,255,255) eller! Scalar(15,150,60), Scalar(20,250,255)
	//Blue: Scalar(110,200,60), Scalar(130,255,255)
}


void image::detectCircles() {
	vector<Vec3f> circles;
}

void image::display() {
	imshow("Display _dst", _dstImg);
	waitKey(0);
}
