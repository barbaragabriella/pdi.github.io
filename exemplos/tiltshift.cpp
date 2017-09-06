#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat mask(5,5,CV_32F), final;
int maximo = 100;
int height;

double l1 = 0, l2 = 0, altura = 0.2, alfa;

double decaimento = 1;
int decaimento_slider = 0;

int top_slider = 50;

int faixa = 20;
int altura_slider = 0;

Mat image1, image2, blended;

char TrackbarName[50];

void ajuste(int, void* ){
  faixa = (double) (top_slider * height) / maximo ; //calcula onde vai ser o centro do foco de acordo com o valor do slider
  l1 = faixa - altura * height; //calcula os parametros l1 e l2 a partir do centro
  l2 = faixa + altura * height;
  

  if(l1 < 0){ //caso l1 seja menor que 0, ele assume o valor zero
     l1 = 0;
  }
  if(l2 > height){ //caso l2 seja maior que a altura da imagem ele assume o valor da altura imagem
   l2 = height;
  }

  for(int i = 0; i < final.size().height; i++){ //a formula eh calculada linha a linha
      alfa = 0.5 * (tanh((i - l1) / (double) (decaimento + 1.0)) - tanh((i-l2) / (double) (decaimento + 1.0)));
      addWeighted(image1.row(i), alfa, final.row(i), 1.0 - alfa, 0.0, image2.row(i));
  }
  imshow("Tiltshift",image2); //mostra a imagem na tela
  imwrite("Tiltshift.png",image2); //cria nova imagem
}

void on_trackbar_decaimento(int, void* ){
  decaimento = (double) decaimento_slider;
  ajuste(top_slider, 0);
}

void on_trackbar_altura(int, void* ){
  altura = (double) altura_slider / maximo;
  ajuste(top_slider, 0);
}

int main(int argvc, char** argv){
  image1 = imread(argv[1]);
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

  namedWindow("tiltshift", 1);

   sprintf( TrackbarName, "Centro");
  createTrackbar( TrackbarName, "tiltshift",
          &top_slider,
          maximo,
          ajuste );
  ajuste(top_slider, 0);

  sprintf( TrackbarName, "Decaimento");
  createTrackbar( TrackbarName, "tiltshift",
          &decaimento_slider,
          maximo,
          on_trackbar_decaimento );
  on_trackbar_decaimento(decaimento_slider, 0);

  sprintf( TrackbarName, "Altura");
  createTrackbar( TrackbarName, "tiltshift",
          &altura_slider,
          maximo,
          on_trackbar_altura );
  on_trackbar_altura(altura_slider, 0);

  //ajuste();

  waitKey(0);
  imwrite("tiltshift.jpg", image2);
  return 0;
}