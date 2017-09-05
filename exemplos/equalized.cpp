#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  int key;
  Mat image, equalizar;

  int width, height;

  VideoCapture cap;

  vector<Mat> planes;

  Mat histR, histE;

  int nbins = 64;

  float range[] = {0, 256};

  const float *histrange = { range };

  bool uniform = true;

  bool acummulate = false;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;

  Mat histImgR(histh, histw, CV_BGR2GRAY, Scalar(0,0,0));
  Mat histImgE(histh, histw, CV_BGR2GRAY, Scalar(0,0,0));

  while(1){
    cap >> image;
    //split (image, planes);

    /// converte para tons de cinza
    cvtColor( image, image, CV_BGR2GRAY );

    calcHist(&image, 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
    }

    equalizeHist( image, equalizar);

    calcHist(&equalizar, 1, 0, Mat(), histE, 1,
               &nbins, &histrange,
               uniform, acummulate);

    normalize(histE, histE, 0, histImgE.rows, NORM_MINMAX, -1, Mat());
    histImgE.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
       line(histImgE, Point(i, histh),
         Point(i, cvRound(histE.at<float>(i))),
         Scalar(255), 1, 8, 0);
    }

    histImgR.copyTo(image(Rect(0, 0 ,nbins, histh)));
    histImgE.copyTo(equalizar(Rect(0, 0 ,nbins, histh)));

    imshow("image", image);
    imshow("Equalizado", equalizar);

    imwrite("nequalizado.png", image);
    imwrite("equalizado.png", equalizar);
    key = (char) waitKey(10);
    if(key == 27) {
      break;
    }
  }

  return 0;
}