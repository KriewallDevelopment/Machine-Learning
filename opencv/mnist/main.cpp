#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <byteswap.h>
#include <inttypes.h>
#include <stdlib.h>
#include <fstream>
#include "seg.h"

using namespace cv;
using namespace std;

const char* IMAGE_FILE;
const char* LABEL_FILE;

const char* TRAIN_IMAGE_FILE = "train/images";
const char* TRAIN_LABEL_FILE = "train/labels";

const char* TEST_IMAGE_FILE = "test/images";
const char* TEST_LABEL_FILE = "test/labels";

typedef struct {

	Mat img;
	PointPair p;

} FEATURE;

vector<FEATURE> getImages(){

	FILE* fp = fopen(IMAGE_FILE, "rb");

    if(!fp){
        printf("Failed to open image file\n");
        exit(EXIT_FAILURE);
    }

	vector<FEATURE> images;

	uint32_t magic;
	uint32_t imageCount;
	uint32_t rowCount;
	uint32_t colCount;

    fread(&magic, sizeof(uint32_t), 1, fp);
    fread(&imageCount, sizeof(uint32_t), 1, fp);
    fread(&rowCount, sizeof(uint32_t), 1, fp);
    fread(&colCount, sizeof(uint32_t), 1, fp);
	
	magic = __bswap_32(magic);
	imageCount = __bswap_32(imageCount);
	rowCount = __bswap_32(rowCount);
	colCount = __bswap_32(colCount);

    for(int i=0; i < imageCount; i++){

		Mat img(rowCount, colCount, CV_8UC1);
		FEATURE f;

		for(int j=0; j < rowCount; j++){

			for(int k=0; k < colCount; k++){

				uint8_t color;
    			fread(&color, sizeof(uint8_t), 1, fp);
				img.at<uchar>(Point(k,j)) = 255 - color;
			}
		}

		//vector<PointPair> bounds = getRectangles(img);
	
		//if(bounds.size() < 1){
		//	f.p.p1 = Point(0,0);
		//	f.p.p2 = Point(0,0);
			f.img = img;
		//}
		//else{
		//	f.img = centerImage(cropImage(img,bounds[0]));
		//	f.p = bounds[0];
		//}
		
		images.push_back(f);
	}

	fclose(fp);
	
	return images;
}

vector<int> getLabels(){

	FILE* fp = fopen(LABEL_FILE, "rb");

    if(!fp){
        printf("Failed to open image file\n");
        exit(EXIT_FAILURE);
    }

	vector<int> labels;

	uint32_t magic;
	uint32_t labelCount;

    fread(&magic, sizeof(uint32_t), 1, fp);
    fread(&labelCount, sizeof(uint32_t), 1, fp);
	
	magic = __bswap_32(magic);
	labelCount = __bswap_32(labelCount);

    for(int i=0; i < labelCount; i++){

		uint8_t label;

    	fread(&label, sizeof(uint8_t), 1, fp);
		labels.push_back(((int)label));
	}

	fclose(fp);
	
	return labels;
}

int featurePrint(Mat img, vector<double> adtl, int l){

	int idx = 1;

	cout << l << " ";

	for(int i=0; i<28; i++){
		for(int j=0; j<28; j++){
			cout << idx++ << ":" << 
				(int) img.at<uchar>(i,j)  << " ";
		}
	}

	for(int i=0; i<adtl.size(); i++)
		cout << idx++ << ":" << adtl[i] << " ";

	cout << endl;

	return idx;
}

int main(int argc, char* argv[]){

	int many = -1;

	if(argc > 1){
		many = atoi(argv[1]);
		IMAGE_FILE = TRAIN_IMAGE_FILE;
		LABEL_FILE = TRAIN_LABEL_FILE;
	}
	else{
		IMAGE_FILE = TEST_IMAGE_FILE;
		LABEL_FILE = TEST_LABEL_FILE;
	}

	vector<FEATURE> images = getImages();
	vector<int> labels = getLabels();
	
	if(many == -1)
		many = images.size();


	for(int itr = 0; itr < many; itr++){
	
		Mat temp = images[itr].img;

		debug(temp);
		continue;
		//Mat blurred;

		temp = deskew(temp);
		//blur(temp, blurred, Size(1,1), Point(-1,-1));

		//for(int i=0; i < 28; i++)
		//	for(int j=0; j < 28; j++)
		//		if(blurred.at<uchar>(i,j) != 255)
		//			blurred.at<uchar>(i,j) = 0;

		//temp = blurred;

		vector<double> adtl = on_pixels(temp);
		vector<float> adtl2 = hog(temp);
		//vector<double> badtl = on_pixels(blurred);

		for(int i=0; i < adtl2.size(); i++)
			adtl.push_back((double)adtl2[i]);

		featurePrint(temp, adtl, labels[itr]);

		//if(IMAGE_FILE == TRAIN_IMAGE_FILE)
		//	featurePrint(blurred, badtl, labels[itr]);
	}

	return 0;
}
