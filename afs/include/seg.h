#ifndef SEG_H
#define SEG_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <inttypes.h>
#include <climits>

using std::cout;
using std::endl;
using std::vector;

#define IS_WHITE(x)		((x) == 255)
#define IS_BLACK(x)		((x) < 125)

#define SMALLER(x,y)	((x) < (y) ? (x) : (y))
#define BIGGER(x,y)		((x) < (y) ? (y) : (x))


typedef struct _pointpair {

    cv::Point p1, p2;

} PointPair;

typedef struct _interval {

	int y1, y2;

	int x1, x2;

} Interval;


static inline void draw_line(cv::Mat img, PointPair p){

	int thick = 1;
	int type = 8;

	cv::line(img, p.p1, p.p2, cv::Scalar(0, 255, 0), thick, type);
}


static inline void draw_rect(cv::Mat img, PointPair p){

	int thick = 1;
	int type = 8;

	cv::rectangle(img, p.p1, p.p2, cv::Scalar(0, 255, 0), thick, type);
}

/***********************************/

static void findX(cv::Mat img, int* min, int* max){

	int minFound=0;

	cv::Mat data;
	cv::Scalar maxVal(28 * 255);
	cv::Scalar val(0);

	for(int i=0; i < 28; i++){

		data = img.col(i);
		val = cv::sum(data);

		if(val.val[0] < maxVal.val[0]){

			*max= i;

			if(!minFound){
				*min= i;
				minFound= 1;
			}
		}
	}
}

static void findY(cv::Mat img, int* min, int* max){

	int i;
	int minFound=0;
	cv::Mat data;
	cv::Scalar maxVal(28 * 255);
	cv::Scalar val(0);

	for (i=0; i < 28; i++){

		data = img.row(i);
		val = cv::sum(data);

		if(val.val[0] < maxVal.val[0]){

			*max=i;

			if(!minFound){
				*min= i;
				minFound= 1;
			}
		}
	}
}

static cv::Rect findBB(cv::Mat img){

	cv::Rect aux;
	int xmin, xmax, ymin, ymax;
	xmin=xmax=ymin=ymax=0;

	findX(img, &xmin, &xmax);
	findY(img, &ymin, &ymax);

	aux = cv::Rect(xmin, ymin, xmax-xmin, ymax-ymin);

	return aux;
}

static cv::Mat preprocessing(cv::Mat img,int new_width, int new_height){

	cv::Mat result;
	cv::Mat scaledResult;

	cv::Mat data;
	cv::Mat dataA;
	cv::Rect bb;
	cv::Rect bba;

	bb = findBB(img);
	data = img(bb);

	int size = (bb.width>bb.height)?bb.width:bb.height;

	result = cv::Mat(cv::Size(size, size), CV_8UC3, cv::Scalar(255,255,255));

	int x=(int)floor((float)(size-bb.width)/2.0f);
	int y=(int)floor((float)(size-bb.height)/2.0f);

	dataA = result(cv::Rect(x,y,bb.width, bb.height));
	data.copyTo(dataA);

	cv::Size dsize(new_width, new_height);
	
	scaledResult = cv::Mat(dsize, CV_8UC3);
	cv::resize(result, scaledResult, dsize, 0,0, CV_INTER_NN);

	return scaledResult;
}

/*********************/

static cv::Mat cropImage(cv::Mat img, PointPair p){

	cv::Rect rect(p.p1.x, p.p1.y, p.p2.x - p.p1.x, p.p2.y - p.p1.y);
    return img(rect);
}

static cv::Mat centerImage(cv::Mat img){

	int SZ = 28;

	cv::Mat output(SZ, SZ, CV_8UC1, cv::Scalar(255));

	int sx = (SZ / 2) - (img.size().width / 2);
	int sy = (SZ / 2) - (img.size().height / 2);

	cv::Rect roi(	cv::Point(sx,sy), 
					cv::Size(img.size().width, img.size().height));

	img.copyTo(output(roi));

	return output;
}

static int cornerHarris_demo(cv::Mat img){

	cv::Mat dst, dst_norm, dst_norm_scaled;
	dst = cv::Mat::zeros( img.size(), CV_32FC1 );

	// Detector parameters
	int blockSize = 4;
	int apertureSize = 3;
	double k = 0.04;
	int thresh = 200;
	int count = 0;

	// Detecting corners
	cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

	// Normalizing
	cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
	cv::convertScaleAbs( dst_norm, dst_norm_scaled );

	// Drawing a circle around corners
	for(int j = 0; j < dst_norm.rows ; j++ ){
		for(int i = 0; i < dst_norm.cols; i++){
            if((int)dst_norm.at<float>(j,i) > thresh ){
				//cv::circle( dst_norm_scaled, cv::Point( i, j ), 2,  cv::Scalar(0), 1, 8, 0 );
				count++;
			}
		}
	}

	//Showing the result
	//cv::namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
	//cv::imshow( "corners_window", dst_norm_scaled );
	//cv::waitKey(0);

	return count;
}


static cv::Mat deskew(cv::Mat img){

	int SZ = 28;
	cv::Mat output(SZ, SZ, CV_8UC1, cv::Scalar(255));
	cv::Moments m = cv::moments(img);

	if(abs(m.mu02) < 1e-2)
		return img;

	float skew = m.mu11 / m.mu02;
	skew *= -10.0;

	cv::Mat M(2, 3, cv::DataType<float>::type);

	M.at<float>(0,0) = 1.0;
	M.at<float>(0,1) = skew;
	M.at<float>(0,2) = -0.5 * SZ * skew;
	M.at<float>(1,0) = 0.0;
	M.at<float>(1,1) = 1.0;
	M.at<float>(1,2) = 0.0;

	cv::warpAffine(img, output, M, img.size(), 
						cv::WARP_INVERSE_MAP | cv::INTER_LINEAR,
						cv::BORDER_CONSTANT,
						cv::Scalar(255,255,255));

	return output;
}

static cv::Mat hog(cv::Mat image){

	int level =1;

	vector <cv::Mat> GaussianPyramid;
	vector <cv::Mat> LaplacianPyramid;

	cv::Mat temp1, temp2, temp3;
	cv::Mat Lap;

	image.copyTo(temp1);

	for(int i=0; i<level; i++){

		cv::pyrDown(temp1, temp2);
		cv::pyrUp(temp2, temp3, temp1.size()); 

		Lap = temp1-temp3;
		GaussianPyramid.push_back(temp2);
		LaplacianPyramid.push_back(Lap);

		temp1=temp2;
	}

	//show whatever you want.

	//cv::namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
    //cv::imshow( "corners_window", /*LaplacianPyramid[0]*/ image );

	//cv::waitKey(0);

	return LaplacianPyramid[0];
}

static cv::Mat edges(cv::Mat img){

    cv::Mat output;
    cv::Canny(img, output, 100, 300, 3);

	vector<cv::Vec4i> lines;

	cv::HoughLinesP(output, lines, 1, CV_PI/180, 80, 30, 10);

	cout << "Line Count: " << lines.size() << endl;

	cv::namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
    cv::imshow( "corners_window", output);

	cv::waitKey(0);

    return output;
}

#endif
