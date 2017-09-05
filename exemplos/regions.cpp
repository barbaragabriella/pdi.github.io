

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int xp1, xp2, yp1, yp2;

  bool flag = true;

  image= imread("imagem.jpg",CV_LOAD_IMAGE_COLOR);
  if(!image.data)
    cout << "imagem não abriu" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  //Solicitar coordenadas do usuário
  while(flag){
    cout << "Informe as coordenadas\n";
    cout << "x de P1: ";
    cin >> xp1;
    cout << "y de P1: ";
    cin >> yp1; 
    cout << "x de P2: ";
    cin >> xp2; 
    cout << "y de P2: ";
    cin >> yp2;

    if((xp1 > image.size().height || yp1 > image.size().width)&&(xp2<xp1||yp2<yp1)&&(xp2 > image.size().height || yp2 > image.size().width)){
      cout << "informe as coordenadas corretamente\n";
    } 
    else flag = false;
  }

  for(int i=xp1;i<xp2;i++){
    for(int j=yp1;j<yp2;j++){
      val = image.at<Vec3b>(i,j);
      val[0] = 255 - val[0];
      val[1] = 255 - val[1];
      val[2] = 255 - val[2];
      image.at<Vec3b>(i,j) = val;
    }
  }
  
  imshow("janela", image); 
  imwrite("regions.png",image); 
  waitKey();
  return 0;
}