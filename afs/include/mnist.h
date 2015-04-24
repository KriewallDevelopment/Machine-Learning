#ifndef MNIST_H
#define MNIST_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <byteswap.h>
#include <inttypes.h>
#include <stdlib.h>
#include <fstream>
#include "seg.h"

using namespace std;

using cv::Point;
using cv::Mat;
using cv::Scalar;

const static char* TRAIN_IMAGES = "train/images";
const static char* TRAIN_LABELS = "train/labels";

const static char* TEST_IMAGES = "test/images";
const static char* TEST_LABELS = "test/labels";

vector<Mat> getImages(const char* file, int size){

	FILE* fp = fopen(file, "rb");

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

    for(int i=0; i < size; i++){

		Mat img(rowCount, colCount, CV_8UC1);

		for(int j=0; j < rowCount; j++){

			for(int k=0; k < colCount; k++){

				uint8_t color;
    			fread(&color, sizeof(uint8_t), 1, fp);
				img.at<uchar>(Point(k,j)) = 255 - color;
			}
		}

		images.push_back(img);
	}

	fclose(fp);
	
	return images;
}

vector<int> getLabels(const char* file, int size){

	FILE* fp = fopen(file, "rb");

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

    for(int i=0; i < size; i++){

		uint8_t label;

    	fread(&label, sizeof(uint8_t), 1, fp);
		labels.push_back(((int)label));
	}

	fclose(fp);
	
	return labels;
}

#endif
