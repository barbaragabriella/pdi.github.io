#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  Mat histLastR, histLastG, histLastB;
  double histDifR = 0, histDifRLast = 0,
      histDifG = 0, histDifGLast = 0,
      histDifB = 0, histDifBLast = 0,
      limite = 10;

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
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
  

  
  cap >> image;
  split (image, planes);
  calcHist(&planes[0], 1, 0, Mat(), histLastR, 1,
           &nbins, &histrange,
           uniform, acummulate);
  calcHist(&planes[1], 1, 0, Mat(), histLastG, 1,
           &nbins, &histrange,
           uniform, acummulate);
  calcHist(&planes[2], 1, 0, Mat(), histLastB, 1,
           &nbins, &histrange,
           uniform, acummulate);

  while(1){
    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i = 0; i < nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));

    // recebe a diferença entre  os histogramas
    histDifR = compareHist(histR, histLastR, CV_COMP_INTERSECT);
    histDifG = compareHist(histG, histLastG, CV_COMP_INTERSECT);
    histDifB = compareHist(histB, histLastB, CV_COMP_INTERSECT);

    // Compara a diferença atual com a antiga incrementada de um limiar
    if(histDifR > histDifRLast + limite || histDifR < histDifRLast - limite ||
        histDifG > histDifGLast + limite || histDifG < histDifGLast - limite ||
        histDifB > histDifBLast + limite || histDifB < histDifBLast - limite) {
      putText(image, "Ha movimento", cvPoint(100,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    } else {
      putText(image, "Sem movimento", cvPoint(100,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    }
    
    histLastR = histR.clone();
    histLastG = histG.clone();
    histLastB = histB.clone();

    histDifRLast = histDifR;
    histDifGLast = histDifG;
    histDifBLast = histDifB;

    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
