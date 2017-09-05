#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int x, y;

  int width, height;

  image= imread("imagem.jpg",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "imagem nao abriu" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  width = image.size().width;
  height = image.size().height;

//cria uma imagem do tamanho da imagem original
  Mat trocaregioes(height, width, CV_LOAD_IMAGE_GRAYSCALE);

  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      x=(i+height/2)%(height);
      y=(j+width/2)%(width); 
      trocaregioes.at<uchar>(i,j)=image.at<uchar>(x,y);
    }  
  }
  
  imshow("janela", trocaregioes);  
  imwrite("trocaregioes.png",trocaregioes);
  waitKey();

  return 0;
}