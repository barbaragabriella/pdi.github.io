#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;


  int fundo = 20;
  int branco = 255;
  int furo = 0;
  int cFuro = 80;
  int sFuro = 200;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);


  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  // Mostrar imagem original
  imshow("Original", image);
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  //Cria uma borda branca
  for (int i = 0; i < width; i++) {
    image.at<uchar>(i, height -1) = branco;
    image.at<uchar>(i, 0) = branco;
  }

  for (int i = 0; i < height; i++) {
    image.at<uchar>(0, i) = branco;
    image.at<uchar>(width -1, i) = branco;
  }


  //remove todos os elementos da borda
  floodFill(image, p, furo);

  imshow("Sem borda", image);

  //pinta o fundo de toda imagem
  floodFill(image, p, fundo);

  imshow("fundo", image);

  // busca todos os objetos
  nobjects=0;
  for(int i=0; i<height; i++){
      for(int j=0; j<width; j++){
        if(image.at<uchar>(i,j) == 255 ){
  		// achou um objeto
  		p.x=j;
  		p.y=i;
  		floodFill(image,p,sFuro);
      nobjects++;
  	  }
  	}
  }

  //Procurando os que possuem furo
  int qtdFuros= 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      //Armazene a posição da bolha encontrada
      if (image.at<uchar>(i,j) == sFuro || image.at<uchar>(i,j) == cFuro) {
        p.x = j;
        p.y = i;
      } 
      else if (image.at<uchar>(i,j) == furo) {
        //Caso for encontrado um furo, mudar o nome para cFuro
         //se já não tiver mudado
        if (image.at<uchar>(p.y, p.x) == sFuro) {
          floodFill(image, p, cFuro);
          qtdFuros++;
        }
        //mude para cFuro
        p.x = j;
        p.y = i;
        floodFill(image, p, cFuro);
      }
    }
  }
  std::cout << "\nTotal de bolhas = " << nobjects;
  std::cout << "\ncom Furos = " << qtdFuros;
  nobjects = (nobjects - qtdFuros);
  std::cout << "\nsem Furos = " << nobjects;
  std::cout << "\n";
  imshow("Final", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}