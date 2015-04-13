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

//const char* IMAGE_FILE = "train/images";
//const char* LABEL_FILE = "train/labels";

const char* IMAGE_FILE = "test/images";
const char* LABEL_FILE = "test/labels";


vector<Mat> getImages(){

	FILE* fp = fopen(IMAGE_FILE, "rb");

    if(!fp){
        printf("Failed to open image file\n");
        exit(EXIT_FAILURE);
    }

	vector<Mat> images;

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

		for(int j=0; j < rowCount; j++){

			for(int k=0; k < colCount; k++){

				uint8_t color;
    			fread(&color, sizeof(uint8_t), 1, fp);
				img.at<uchar>(Point(k,j)) = 255 - color;
			}
		}

		vector<PointPair> bounds = getRectangles(img);
	
		if(bounds.size() < 1)
			images.push_back(img);
		else
			images.push_back(centerImage(cropImage(img,bounds[0])));
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

int main(int argc, char* argv[]){

	vector<Mat> images = getImages();
	vector<int> labels = getLabels();


	for(int itr = 2; itr < images.size(); itr++){
	
		Mat temp = images[itr];

		imwrite("imgout.jpg", temp);
		break;
	
		int bins[7][7];
	
		for(int i=0; i<7; i++)
			for(int j=0; j<7; j++)
				bins[i][j] = 0;

		for(int i=0; i < 28; i++){
			for(int j=0; j < 28; j++){

				int val = (int)temp.at<uchar>(Point(i,j));
				int row = (i - (i % 4)) / 4;
				int col = (j - (j % 4)) / 4;
				bins[row][col] += val;
			}
		}

		cout << labels[itr] << " ";

		for(int i=0; i<7; i++){
			for(int j=0; j<7; j++){
				bins[i][j] /= 16;
				cout << (i*7) + j + 1 << ":" << bins[i][j] << " ";
			}
		}

		cout << endl;
	}

	//cvNamedWindow("TEST", CV_WINDOW_AUTOSIZE);
	//imshow("TEST", images[0]);

	//while(true){

    //    char c = cvWaitKey(33);
    //    if(c == 27)
    //        break;
    //}

	//cvDestroyWindow("TEST");

	return 0;
}
