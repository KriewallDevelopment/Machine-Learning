#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "seg.h"


using namespace cv;
using namespace std;

const char* DISPLAY_WINDOW = "GreyScale";

static void suspend(){

	while(true){

		char c = cvWaitKey(33);
		if(c == 27)
			break;
	}
}

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Usage: " << argv[0] << " [FILENAME]" << endl;
		return 1;
	}

	cvNamedWindow(DISPLAY_WINDOW, CV_WINDOW_AUTOSIZE);

	Mat frame = imread(argv[1],1);
	Mat grey;
	Mat out;

	int WIDTH	= frame.size().width;
	int HEIGHT	= frame.size().height;

	cout << "WIDTH: " << WIDTH << endl;
	cout << "HEIGHT: " << HEIGHT << endl;

	cvtColor(frame, grey, CV_RGB2GRAY);
	cvtColor(grey, out, CV_GRAY2RGB);

	vector<PointPair> rects = getRectangles(grey);
	purgeDuplicates(&rects);

	cout << "RECTS SIZE: " << rects.size() << endl;

	for(int i=0; i < rects.size(); i++)
		draw_rect(out,rects[i]);

	Mat cropped = cropImage(grey,rects[0]);

	imwrite("imgout.jpg",cropped);
	imshow(DISPLAY_WINDOW, out);
	suspend();
	
	cvDestroyWindow(DISPLAY_WINDOW);

	return 0;
}
