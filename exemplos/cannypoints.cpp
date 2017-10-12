#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;

int STEP=5;
int JITTER=3;
int RAIO=4;

#define STEP_MAX 20
#define JITTER_MAX 10
#define RAIO_MAX 10


Mat image, border, points;

void on_trackbar_canny(int, void*){

  vector<int> yrange;
  vector<int> xrange;

  Vec3b colorida;

  int width, height, gray;
  int x, y;

  width=image.size().width;
  height=image.size().height;

  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);

  if(STEP < 1) STEP = 1;
  if(JITTER < 1) JITTER = 1;
  if(RAIO < 1) RAIO = 1;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);

      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(colorida[0],colorida[1],colorida[2]),
             -1,
             CV_AA);
    }
  }

  for(int i=0; i<height; i++ ){
        for(int j=0; j<width; j++){
           if(border.at<uchar>(i,j)>0){
              colorida[0] = (int)(image.at<Vec3b>(i,j)[2]);
              colorida[1] = (int)(image.at<Vec3b>(i,j)[1]);
              colorida[2] = (int)(image.at<Vec3b>(i,j)[0]);

              circle(points,
                     cv::Point(j,i),
                     RAIO,
                     CV_RGB(colorida[0],colorida[1],colorida[2]),
                     -1,
                     CV_AA);
         }
      }
   }

   imshow("cannypoints", points);
}


int main(int argc, char** argv){
  
  image= imread(argv[1],CV_LOAD_IMAGE_COLOR);

  namedWindow("cannypoints",1);
  createTrackbar( "Threshold", "cannypoints",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  createTrackbar( "RAIO", "cannypoints",
                  &RAIO,
                  RAIO_MAX,
                  on_trackbar_canny );

  createTrackbar( "JITTER", "cannypoints",
                  &JITTER,
                  JITTER_MAX,
                  on_trackbar_canny );

  createTrackbar( "STEP", "cannypoints",
                  &STEP,
                  STEP_MAX,
                  on_trackbar_canny );

  on_trackbar_canny(top_slider, 0 );

  imwrite("cannyPoints.jpg", points);

  waitKey();
  return 0;
}