#ifndef SEG_H
#define SEG_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>


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

cv::Mat deskew(cv::Mat img){

	int SZ = 28;

	vector<cv::Vec4i> lines;
	cv::HoughLinesP(img, lines, 1, CV_PI / 180.0, 100, SZ / 4.0, 20);

	
}
#endif
