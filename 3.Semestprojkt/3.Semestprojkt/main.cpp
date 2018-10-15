#include <stdio.h>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/videoio.hpp>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>

using namespace std;
using namespace cv;

int main() {


	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}

	getchar();
	return 0;
}