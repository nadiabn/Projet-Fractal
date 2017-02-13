// g++ -Wall fractal-noir-blanc.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11


#include <opencv2/opencv.hpp>
#include <iostream>
#include <complex>
#include <thread>
#include <mutex>
#include <vector>


//Déclaration des dimensions de la fenêtre d'affichage
#define IMG_W 720
#define IMG_H 720

//Déclaration du nombre complexe C
std::complex<double> c(-0.181,-0.667);

//un nouvel objet image 
cv::Mat newImg(IMG_W, IMG_H, CV_8UC3);
  
//Fonction de transposition des coordonnées et d'incrémentation de la suite de Julia 
cv::Vec3b couleur(int i, int j ,  std::complex<double> c) {
	int val;
	double x1=( (float)i / IMG_W) * 2. - 1.; //Transposition des coordonnées x
        double y1=( (float)j / IMG_H) * 2. - 1.; //Transposition des coordonnées y
	std::complex<double> z(x1,y1);
	for ( val= 0; val < 255 && std::norm(z) < 4; val++) 
        {
		z = z*z + c; //Suite de julia
	}
	return cv::Vec3b(val,val,val); //Nuances de gris pour la coloration des pixels
}

//Main
int main(int argc, char * argv[]) {
for (int y=0; y < IMG_H; y++) 
{
        for (int x=0 ; x < IMG_W; x++)
	{
		newImg.at<cv::Vec3b>(cv::Point(y, x)) = couleur(x,y,c); //Dessin d'un pixel
	}
}
  
while(int key = cvWaitKey(30)) {
    	
    	if (key == 27)
    		break;
    	imshow("image", newImg); //affiche l'image
}
imwrite("image.jpg", newImg);    // sauve une copie de l'image
cvDestroyWindow("image");        //ferme la fenêtre

	return 0;
}
