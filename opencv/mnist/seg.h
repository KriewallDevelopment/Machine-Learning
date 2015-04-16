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

static vector<Interval> getBlackIntervals(cv::Mat img, int col){

	vector<Interval> ret;
	bool curr = false;

	int HEIGHT = img.size().height;

	Interval itv;
	itv.y1 = itv.y2 = 0;

	for(int i=0; i < HEIGHT; i++){

		int color = ((int)img.at<uchar>(i,col));

		if((IS_WHITE(color) && curr) || (i == (HEIGHT-1) && curr)){
			curr = false;
			itv.y2 = SMALLER(i + 1, HEIGHT);
			ret.push_back(itv);
			itv.y1 = itv.y2 = 0;
		}
		else if(IS_BLACK(color) && curr){
			itv.y2 = i + 1;
		}
		else if(IS_BLACK(color)){

			itv.y1 = BIGGER(i-1,0);
			itv.y2 = i + 1;
			curr = true;
		}
	}

	return ret;
}

static vector<Interval> getWhiteIntervals(cv::Mat img, int col){

	vector<Interval> ret;
	bool curr = false;

	int HEIGHT = img.size().height;

	Interval itv;
	itv.y1 = itv.y2 = 0;

	for(int i=0; i < HEIGHT; i++){

		int color = ((int)img.at<uchar>(i,col));

		if((IS_BLACK(color) && curr) || (i == (HEIGHT-1) && curr)){
			curr = false;
			itv.y2 = SMALLER(i + 1, HEIGHT);
			ret.push_back(itv);
			itv.y1 = itv.y2 = 0;
		}
		else if(IS_WHITE(color) && curr){
			itv.y2 = i + 1;
		}
		else if(IS_WHITE(color)){

			itv.y1 = BIGGER(i-1,0);
			itv.y2 = i + 1;
			curr = true;
		}
	}

	return ret;
}

PointPair getSingleImageRectangle(cv::Mat img){

	PointPair p;

	/* Left */

	int idx = 0;

	while(true){

		if(getBlackIntervals(img,idx).size() == 0)
			idx++;
		else
			break;
	}

	p.p1.x = BIGGER(idx - 1, 0);

	/* Right */

	idx = 27;

	while(true){

		if(getBlackIntervals(img,idx).size() == 0)
			idx--;
		else
			break;
	}

	p.p2.x = SMALLER(idx + 1, 27);

	return p;
}


void combineIntervals(vector<Interval>* v1, vector<Interval>* v2){

	for(int i=0; i < v2->size(); i++)
		v1->push_back(v2->at(i));

	/* eh... n^2... */

	for(int i=0; i < v1->size(); i++){

		Interval itv1 = v1->at(i);

		for(int j = i+1; j < v1->size(); j++){

			Interval itv2 = v1->at(j);

			/* Lower overlap? */

			if(itv1.y2 >= itv2.y1 && itv1.y1 <= itv2.y1){
				itv1.y2 = BIGGER(itv1.y2, itv2.y2);
				itv1.y1 = SMALLER(itv1.y1, itv2.y1);
				itv1.x1 = SMALLER(itv1.x1, itv2.x1);
				v1->at(i) = itv1;
				v1->erase(v1->begin() + j);
				i=0;
				break;
			}

			/* Upper overlap? */

			if(itv2.y2 >= itv1.y1 && itv2.y1 <= itv1.y1){
				itv2.y2 = BIGGER(itv1.y2, itv2.y2);
				itv2.y1 = SMALLER(itv1.y1, itv2.y1);
				itv2.x1 = SMALLER(itv1.x1, itv2.x1);
				v1->at(j) = itv2;
				v1->erase(v1->begin() + i);
				i=0;
				break;
			}
		}
	}
}

static void purgeDuplicates(vector<PointPair>* rects){

	for(int i=0; i < rects->size(); i++){

		for(int j=i+1; j < rects->size(); j++){
		
			PointPair one = rects->at(i);
			PointPair two = rects->at(j);

			/* Does two contain one? */

			if( one.p1.x > two.p1.x &&
				one.p1.x < two.p2.x &&
				one.p2.y < two.p2.y &&
				one.p2.y > two.p1.y)
			{
				rects->erase(rects->begin() + i);
				i = -1;
				break;
			}
			
			/* Does one contain two? */

			if( two.p1.x > one.p1.x &&
				two.p1.x < one.p2.x &&
				two.p2.y < one.p2.y &&
				two.p2.y > one.p1.y)
			{
				rects->erase(rects->begin() + j);
				i = -1;
				break;
			}
		}
	}
}

vector<PointPair> getRectangles(cv::Mat img){

	vector<PointPair>   rects;
    vector<Interval>    ints;

	int WIDTH = img.size().width;
    int state = 0;

    for(int i=0; i < WIDTH; i++){

        vector<Interval> bints = getBlackIntervals(img,i);
        vector<Interval> wints = getWhiteIntervals(img,i);

        /* Mark current X value */

        for(int j=0; j < bints.size(); j++)
            bints[j].x1 = BIGGER(i-1,0);

        combineIntervals(&ints, &bints);

        /* Have any intervals terminated? */

        for(int j=0; j < wints.size(); j++){

            Interval witv = wints[j];

            for(int k=0; k < ints.size(); k++){

                Interval bitv = ints[k];
				
				if(witv.y1 <= bitv.y1 && witv.y2 >= bitv.y2){
                    bitv.x2 = SMALLER(i+1, WIDTH);
                    PointPair pp;
                    pp.p1 = cv::Point(bitv.x1,bitv.y1);
                    pp.p2 = cv::Point(bitv.x2,bitv.y2);
                    rects.push_back(pp);
                    ints.erase(ints.begin() + k);
                    k = -1;
                }
            }
        }
    }

    purgeDuplicates(&rects);

	return rects;
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
	cv::namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
	cv::imshow( "corners_window", dst_norm_scaled );
	cv::waitKey(0);

	return count;
}


cv::Mat deskew(cv::Mat img){

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

cv::Mat hog(cv::Mat image){

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

#endif
