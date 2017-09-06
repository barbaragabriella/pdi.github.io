#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argvc, char** argv){
  VideoCapture cap;
  VideoWriter videoWriter;

  Mat mask(5,5,CV_32F), final, image1, image2;
  int height, width;

  double l1 = 0, l2 = 0, altura = 0.2, d = 60, alfa;
  int faixa = 20;

  int stepReductionRate = 2, cont = 1;

  cap.open(argv[1]);
  
  if(!cap.isOpened()){
    cout << "\n\nnao deu pra abrir o arquivo de entrada, foi mal\n";
    return -1;
  } else cout << "arquivo de entrada lido\n";
  
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  Size s = Size(width, height);
  videoWriter.open("video.avi", CV_FOURCC('D', 'I', 'V', '3'), cap.get(CV_CAP_PROP_FPS) / stepReductionRate, s, true);

  if(!videoWriter.isOpened()) {
    cout << "\n\nnao deu pra abrir o arquivo de saida, foi mal\n";
    return -1;
  } else cout << "arquivo de saida lido\n";

  namedWindow("image", WINDOW_AUTOSIZE);
  cout << "dando inicio a edicao e gravacao do video\n";

  while(cap.isOpened()) {
    cap >> image1;
    
    if (image1.empty()) break;

    if(cont < stepReductionRate){
      cont++;
      continue;
    }
    cont = 1;

    height = image1.size().height;
    image2 = image1.clone();

    Mat aux, mask, mask1;

    float media[] = {1, 1, 1, 1, 1,
                     1, 1, 1, 1, 1,
                     1, 1, 1, 1, 1,
                     1, 1, 1, 1, 1,
                     1, 1, 1, 1, 1};

    mask = Mat(5, 5, CV_32F, media);
    scaleAdd(mask, 1 / 25.0, Mat::zeros(5, 5, CV_32F), mask1);
    swap(mask, mask1);

    image2.convertTo(aux, CV_32F);
    // Aplicar o filtro algumas vezes para criar um bom desfoque
    for (int i = 0; i < 20; i++) {
      filter2D(aux, aux, aux.depth(), mask, Point(2, 2), 0);
    }

    aux.convertTo(final, CV_8UC3);

    faixa = (double) 0.5 * height; //calcula onde vai ser o centro do foco de acordo com o valor do slider
    l1 = faixa - altura * height; //calcula os parametros l1 e l2 a partir do centro
    l2 = faixa + altura * height;

    if(l1 < 0){ //caso l1 seja menor que 0, ele assume o valor zero
       l1 = 0;
    }

    if(l2 > height){ //caso l2 seja maior que a altura da imagem ele assume o valor da altura imagem
     l2 = height;
    }

    for(int i = 0; i < final.size().height; i++){ //a formula eh calculada linha a linha
        alfa = 0.5 * (tanh((i - l1) / (double) (d + 1.0)) - tanh((i-l2) / (double) (d + 1.0)));
        addWeighted(image1.row(i), alfa, final.row(i), 1.0 - alfa, 0.0, image2.row(i));
    }
    videoWriter << image2;
  }
  cout << "video gravado com sucesso\n";
  return 0;
}