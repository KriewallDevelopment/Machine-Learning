#ifndef STATE_H
#define STATE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <cmath>
#include <string>
#include "svm.h"
#include "seg.h"

inline bool CLOSE_TO(double x, double y){
	return x > (y - 0.000001) && x < (y + 0.000001);
}

typedef std::vector<double> (*FEATURE_FUNCTION)(cv::Mat);


typedef struct _feature {

	std::string			name;
	FEATURE_FUNCTION	eval;

    bool operator == (const struct _feature &f) {
		return name == f.name;
    }

} FEATURE;


//extern std::vector<FEATURE> LARGE_SCALE_FEATURES;
//extern std::vector<FEATURE> SMALL_SCALE_FEATURES;


class State {

public:

	State();
	~State();

	State(const State &s){

		this->features = s.features;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
    }

    bool operator == (const State &s) {

		if(features.size() != s.features.size())
			return false;

		for(int i=0; i < s.features.size(); i++){
			
			if(features[i].name != s.features[i].name)
				return false;
		}

		return true;
    }

	State& operator = (const State &s){
		this->features = s.features;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
		return *this;
	}

	friend bool operator < (const State& one, const State& two){
		return one.fitnessScore > two.fitnessScore;
	}

	void print();

	/* Could also return double here */

	double fitness();
	double constFitness() const;
	
	/* Should be called a small percentage of the time */

	void mutate();


	/***** PSEUDO PRIVATE VARS *****/

	bool scoreIsCached;
	double fitnessScore;

	/* DEFINE ACTUAL STATE VARIABLES HERE */

	std::vector<FEATURE> features;
};


static std::vector<double> edge(cv::Mat img){

	img = edges(deskew(img));

	std::vector<double> out;

	for(int i=0; i<img.size().width; i++)
		for(int j=0; j<img.size().height; j++)
			out.push_back((double)img.at<uchar>(i,j));

	return out;
}

static std::vector<double> laplacian(cv::Mat img){

	img = hog(deskew(img));

	std::vector<double> out;

	for(int i=0; i<img.size().width; i++)
		for(int j=0; j<img.size().height; j++)
			out.push_back((double)img.at<uchar>(i,j));

	return out;
}

static std::vector<double> pixels(cv::Mat img){

	cv::Mat temp = deskew(img);

	std::vector<double> out;

	int bins[7][7];

    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            bins[i][j] = 0;

    for(int i=0; i < 28; i++){
        for(int j=0; j < 28; j++){

            int val = (int)temp.at<uchar>(cv::Point(i,j));
            int row = (i - (i % 4)) / 4;
            int col = (j - (j % 4)) / 4;
            bins[row][col] += val;
        }
    }

    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            bins[i][j] /= 16;
            out.push_back((double)bins[i][j]);
        }
    }

	return out;
}

static std::vector<double> skew(cv::Mat img){

	std::vector<double> skew;
	
	skew.push_back(0.0);

	cv::Moments m = cv::moments(img);

    if(abs(m.mu02) < 1e-2)
        return skew;

    skew[0] = -10.0 * (m.mu11 / m.mu02);

	return skew;
}

static std::vector<double> bounds(cv::Mat img){

	std::vector<double> out;

	cv::Rect r = findBB(img);

	out.push_back((double)((r.width / 2) + r.x));
	out.push_back((double)((r.height /2) + r.y));
	out.push_back(r.width);
	out.push_back(r.height);

	return out;
}

static std::vector<double> on_pixels(cv::Mat img){

	std::vector<double> out;

	int set = 0;
	cv::Rect bb = findBB(img);

	double bmx = bb.x + (bb.width / 2);
	double bmy = bb.y + (bb.height / 2);

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

static std::vector<double> corners(cv::Mat img){

	std::vector<double> out;
	out.push_back((double)cornerHarris_demo(img));
	return out;
}

#endif
