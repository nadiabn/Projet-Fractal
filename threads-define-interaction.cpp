// g++ -Wall threads-define-interaction.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lpthread -std=c++11


#include <opencv2/opencv.hpp>
#include <iostream>
#include <complex>
#include <thread>
#include <mutex>
#include <vector>


//Déclaration des dimensions de la fenêtre d'affichage
#define IMG_W 720
#define IMG_H 720

//Déclaration de nombre de Threads
#define num_threads 6

//Déclaration du nombre complexe C
std::complex<double> c(-0.181,-0.667);
double re= c.real();
double im= c.imag();

//un nouvel objet image 
cv::Mat newImg(IMG_W, IMG_H, CV_8UC3);

//Conversion Hsv to RGB
static void HSVToRGB(float h, float s, float v, float *r, float *g, float *b)
{
	if (h < 0) h = 0;
	if (h > 359) h = 359;
	if (s < 0) s = 0;
	if (s > 1) s = 100;
	if (v < 0) v = 0;
	if (v > 1) v = 100;

	float tmp = h/85.0;
	int hi = floor(tmp);
	float f = tmp - hi;
	float p =  (1 - s);
	float q = (1 - f * s);
	float t =  (1 - (1 - f) * s);
	switch (hi) {
		case 0:
			*r = 173;
			*g = 201;
			*b = 190;
			break;
		case 1:
			*r = q;
			*g = t;
			*b = v;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = 26;
			*g = q;
			*b = 80;
			break;
		case 4:
			*r = v;
			*g = t;
			*b = q;
			break;
		case 5:
			*r = p;
			*g = q;
			*b = v;
			break;
	}
}
 
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
	float r , g , b ;
	HSVToRGB(val,100,100,&r,&g,&b);
        return cv::Vec3b(r,g,b); //Pixels en couleurs
}

//Fonction de dessin de la dractale avec les threads
void threado(int i) {
double morceau=(float) IMG_H/num_threads;
int n=(i*morceau)+morceau;
for (int y=i*morceau; y <n; y++) 
{
	for (int x=0 ; x<IMG_W ; x++)
       {  
          newImg.at<cv::Vec3b>(cv::Point(y, x)) = couleur(x,y,c);
       }
}
}


//Main
int main(int argc, char * argv[]) {
std::thread tid[num_threads];
for(int i=0;i<num_threads;i++)
{
	tid[i]=std::thread (threado,i); // Creation du thread
}

for(int i=0; i<num_threads; i++)
{
	tid[i].join();
}
while(int key = cvWaitKey(30)) {
 	
    	if (key != -1)
	{
		switch (key)
		{
			case 279165: //HAUT
			   re-=0.05;

			case 279167: //Bas
			   im-=0.07;	

			case 279166: //DROITE
			   re+=0.05;

			case 279168: //GAUCHE
			   im+=0.07;	
			  
		}
	c = std::complex<long double>(re, im); 

}
    		
    	imshow("image", newImg); //affiche l'image
}
imwrite("image.jpg", newImg);    // sauve une copie de l'image
cvDestroyWindow("image");        //ferme la fenêtre

	return 0;
}
