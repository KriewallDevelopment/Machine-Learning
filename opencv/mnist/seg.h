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


static void debug(cv::Mat m){

	cv::namedWindow("debug", CV_WINDOW_AUTOSIZE );
	cv::imshow("debug", m );
	cv::waitKey(0);
}


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

void findX(cv::Mat img, int* min, int* max){

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


void findY(cv::Mat img, int* min, int* max){

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

cv::Rect findBB(cv::Mat img){

    cv::Rect aux;
    int xmin, xmax, ymin, ymax;
    xmin=xmax=ymin=ymax=0;

    findX(img, &xmin, &xmax);
    findY(img, &ymin, &ymax);

    aux = cv::Rect(xmin, ymin, xmax-xmin, ymax-ymin);

    return aux;
}

cv::Mat cropImage(cv::Mat img, PointPair p){

	cv::Rect rect(p.p1.x, p.p1.y, p.p2.x - p.p1.x, p.p2.y - p.p1.y);
    return img(rect);
}

cv::Mat centerImage(cv::Mat img){

	int SZ = 28;

	cv::Mat output(SZ, SZ, CV_8UC1, cv::Scalar(255));

	int sx = (SZ / 2) - (img.size().width / 2);
	int sy = (SZ / 2) - (img.size().height / 2);

	cv::Rect roi(	cv::Point(sx,sy), 
					cv::Size(img.size().width, img.size().height));

	img.copyTo(output(roi));

	//debug(output)

	return output;
}

int cornerHarris_demo(cv::Mat img){

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
				cv::circle( dst_norm_scaled, cv::Point( i, j ), 2,  cv::Scalar(0), 1, 8, 0 );
				count++;
			}
		}
	}

	//Showing the result
	debug(dst_norm_scaled);

	return count;
}


cv::Mat deskew(cv::Mat img){

/*
	cv::Mat M, rotated;

	std::vector<cv::Point> points;

	for(int i=0; i < img.size().width; i++)
		for(int j=0; j < img.size().height; j++)
			if(!IS_WHITE(img.at<uchar>(i,j)))
				points.push_back(cv::Point(i,j));

	cv::RotatedRect box = cv::minAreaRect(points);

	float angle = box.angle;
	cv::Size rs = box.size;

	if(box.angle < -45.0){

		angle += 90.0;
		double tmp = rs.width;
		rs.width = rs.height;
		rs.height = tmp;	
	}

	M = cv::getRotationMatrix2D(box.center, angle, 1.0);
	cv::warpAffine(img, rotated, M, img.size(), cv::INTER_CUBIC);

	debug(rotated);

	//cv::Scalar color(0,0,0);
	//cv::Point2f rect_points[4];
	//box.points( rect_points );
    //for( int j = 0; j < 4; j++ )
	//	line( img, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

	//debug(img);

	*/

	/***********************/

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

	//debug(output);

	return output;
}

std::vector<float> hog(cv::Mat image){

	cv::Mat img(cv::Size(28,28), CV_8UC3);

	cv::cvtColor(image, img, cv::COLOR_GRAY2RGB);

//	for(int i=0; i < 28; i++)
//		for(int j=0; j < 28; j++)
//			img

	cv::Mat hsv;
	cv::Mat hsvHalf;

	//cout << img.channels() << endl;

	cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);

	hsvHalf = hsv(cv::Range(hsv.rows / 2, hsv.rows - 1),
					cv::Range( 0, hsv.cols - 1));

	int hbins = 12;
	int sbins = 12;
	int histSize[] = { hbins, sbins };

	float hranges[] = { 0, 180 };
	float sranges[] = { 0, 256 };

	const float* ranges[] = { hranges, sranges };
	
	int channels[] = { 0, 1 };

	cv::MatND histbase;

	cv::calcHist(&hsv, 1, channels, cv::Mat(),
					histbase, 2, histSize, ranges,
					true, false);

	cv::normalize(histbase, histbase, 0, 1, 
					cv::NORM_MINMAX, -1, cv::Mat());

	std::vector<float> out;

	for(int i=0; i < 256; i++){
		float f = histbase.at<float>(i);
		if(f != f)
			out.push_back(0.0);
		else
			out.push_back(histbase.at<float>(i));
	}

	return out;

	/*
	vector<float>		ders;
	vector<cv::Point>	locs;

	cv::Size sz( 28, 28);
	cv::Size pd( 0, 0);

	cv::HOGDescriptor hog;
	hog.winSize = cv::Size(64,128);

	hog.compute(image, ders, sz, pd, locs);

	cout << ders.size() << endl;

	return ders;
	*/
}

cv::Mat laplace(cv::Mat image){

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

	return LaplacianPyramid[0];
}

static int edges(cv::Mat img){

	//cv::Canny(img, output, 100, 300, 3);

	int LEN = 6; // line length in pixels
	

	int count = 0;

	return count;
}

static std::vector<double> on_pixels(cv::Mat img){

    std::vector<double> out;

    int set = 0;
    cv::Rect bb = findBB(img);

    double bmx = bb.x + (bb.width / 2);
    double bmy = bb.y + (bb.height / 2);

	out.push_back(bmx);
    out.push_back(bmy);
    out.push_back(bb.width);
    out.push_back(bb.height);

    double mux = 0.0;
    double muy = 0.0;

    for(int i=0; i < img.size().width; i++){
        for(int j=0; j < img.size().height; j++){

            if(!IS_WHITE(img.at<uchar>(i,j))){
                mux += i;
                muy += j;
                set++;
            }
        }
    }

	mux /= (1.0 * set);
    muy /= (1.0 * set);

    mux -= bmx;
    muy -= bmy;

    mux /= (1.0 * bb.width);
    muy /= (1.0 * bb.height);

    out.push_back((double)set);
    out.push_back(mux);
    out.push_back(muy);
    out.push_back(pow(mux,2.0));
    out.push_back(pow(muy,2.0));
    out.push_back(mux * muy);
    out.push_back(muy * pow(mux,2.0));
    out.push_back(mux * pow(muy,2.0));

    return out;
}

#endif
