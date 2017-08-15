#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  VideoCapture cap;
  Vec3b val;
  Mat image, imagegray, imagecopia;
  char tecla;

  cap.open("MyVideo.avi");
  int width = cap.get(CV_CAP_PROP_FRAM_WIDTH);
  int height = cap.get(CV_CAP_PROP_FRAM_HEIGHT);
  Size frameSize(width, height);
  VideoWriter out (
  		"MyVideoOut.avi",
  		CV_FOURCC('P', 'I', 'M', '1'),
  		20,
  		frameSize,
  		true);

  cap.open(0);
  if(!cap.isOpened()) {
  	printf("Nao abriu a camera\nsaindo...");
  	exit(0);
  }

  while(1) {
  	cap >> image;

	val[0] = 0;
	val[1] = 0;
	val[2] = 255;

  	for(int i = 200; i < 200; i++) {
  		for(int j = 10; j < 200; j++) {
  			image.at<Vec3b>(i,j) = val;
		}  			
	}

	val[0] = 0;
	val[1] = 0;
	val[2] = 255;

	cvtColor(image, imagegray, CV_BGR2GRAY);
	// imagegray.copyTo(imagecopia);
	flip(imagegray, imagegray, 1);
	imagecopia = imagegray.clone();
  	imshow("image", imagegray);
  	tecla = waitKey(30);
  	if(tecla == ' ') {
  		break;
  	}
  }
  return 0;
}