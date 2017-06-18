#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <iterator>
#include <time.h>
#include <fstream>

using namespace cv;
using namespace std;

string timeFromMilis(double x) {

	int y = 60 * 60 * 1000;
	int h = x / y;
	int m = (x - (h*y)) / (y / 60);
	int s = (x - (h*y) - (m*(y / 60))) / 1000;
	int mi = x - (h*y) - (m*(y / 60)) - (s * 1000);

	string value;
	if (h < 10) {
		value += to_string(0) + to_string(h);
	}
	else {
		value += to_string(h);
	}
	if (m < 10) {
		value += ":" + to_string(0) + to_string(m);
	}
	else {
		value += ":" + to_string(m);
	}
	if (s < 10) {
		value += ":" + to_string(0) + to_string(s);
	}
	else {
		value += ":" + to_string(s);
	}

	return value;
}

long getFileSize(string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

string convertSize(size_t size) {
	static const char *SIZES[] = { "B", "KB", "MB", "GB" };
	int div = 0;
	size_t rem = 0;

	while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
		rem = (size % 1024);
		div++;
		size /= 1024;
	}

	double size_d = (float)size + (float)rem / 1024.0;
	string result = to_string(size_d) + " " + SIZES[div];
	return result;
}

int main(int, char** argv)
{
	String url = argv[1];

	VideoCapture cap(url);
	double number_of_frames = 50; //argv[2]
	int number_of_col = 5;
	double number_of_rows = number_of_frames / number_of_col;
	double frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	double frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double interval = cap.get(CV_CAP_PROP_FRAME_COUNT) / number_of_frames;

	string file = "File: " + url;
	string size = "Size: " + convertSize(getFileSize(url));
	string duration = " Duration: " +  timeFromMilis((cap.get(CV_CAP_PROP_FRAME_COUNT) / cap.get(CV_CAP_PROP_FPS))*1000);
	string video= "Video: " + to_string((int)frame_width) + "x" + to_string((int)frame_height);
	string fps = " Fps: " + to_string(cap.get(CV_CAP_PROP_FPS));


	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat firstrow;
	for (int i = 1; i <= 10; i++) {
		Mat row;
		for (int j = 1; j <= number_of_col; j++) {
			if (j == 1) {
				cap.set(CV_CAP_PROP_POS_FRAMES, ((((i - 1)*(number_of_col)+j)-1)*interval));
				cap.read(row);
				putText(row, timeFromMilis(cap.get(CV_CAP_PROP_POS_MSEC)), cvPoint(10, frame_height - 20), FONT_HERSHEY_PLAIN, 1.5, cvScalar(0, 0, 0), 2, CV_AA, false);
			}
			else {
				Mat mat1;
				cap.set(CV_CAP_PROP_POS_FRAMES, ((((i - 1)*(number_of_col)+j) - 1)*interval));
				cap.read(mat1);
				putText(mat1, timeFromMilis(cap.get(CV_CAP_PROP_POS_MSEC)), cvPoint(10, frame_height - 20), FONT_HERSHEY_PLAIN, 1.5, cvScalar(0, 0, 0), 2, CV_AA, false);
				hconcat(row, mat1, row);
			}
		}

		if (i == 1) { //FIRST ROW
			firstrow = row;
		}
		else {
			vconcat(firstrow, row, firstrow);
		}
	}

	double labelwidth = frame_width*number_of_col;

	Mat label = Mat::ones(200, labelwidth, CV_8UC3);
	label.setTo(Scalar(255, 255, 255));
	putText(label, file, cvPoint(0, 50), FONT_HERSHEY_PLAIN, 3, cvScalar(0, 0, 0), 2, CV_AA, false);
	putText(label, size + duration, cvPoint(0, 100), FONT_HERSHEY_PLAIN, 3, cvScalar(0, 0, 0), 2, CV_AA, false);
	putText(label, video + fps, cvPoint(0, 150), FONT_HERSHEY_PLAIN, 3, cvScalar(0, 0, 0), 2, CV_AA, false);

	vconcat(label, firstrow, firstrow);

	String imageurl = url + ".jpg";
	imwrite(imageurl, firstrow);

	return 0;
}