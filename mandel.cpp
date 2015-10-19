#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// #include <boost/multiprecision/gmp.hpp>
#include "bitmap_image.hpp"

using namespace std;
//using boost::multiprecision::double;
const int RSPEED = 499;
const int GSPEED = 101;
const int BSPEED = 23;

void usage(char*prog){
		cout << "Usage : "<<prog<<" <output name> <nb frames> <first frame> <nb iterations> <XRES> <YRES>"
			<<" <width> <zoomfactor> <zoomx> <zoomy> <zoomtype> <power>where a=zoom, b=iteration, c=xtrans, d=ytrans, e=tiles"<<endl;
}

double norme(double x, double y){
	return (x*x+y*y);
}

int iterate(int iterations, double x, double y,int puissance){
	int bound = 0;
	double x0 = x;
	double y0 = y;
	while(norme(x,y)<=4&&bound<iterations){
		bound++;
		double tempx=x;
		double tempy=y;
		double x1;
		for (int i = 1; i < puissance; ++i)
		{
			x1 = x;
			x = x*tempx-y*tempy;
			y = y*tempx+x1*tempy;
		}
		x=x+x0;
		y=y+y0;
	}
	if (bound==iterations-1){
		return 0;
	}
	return bound;
}

bitmap_image generate(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance){
	if (YMIN+YMAX==0)
	{
	for(int i=0; i<XRES; i++){
		for(int j=0; j<YRES/2; j++){
			int color = iter-iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),puissance);
			//image.set_pixel(XRES-i-1,j,color/100*10+20,color/10%10*10+20,color%10*20+30);
			image.set_pixel(XRES-i-1,j,2*(int)((double)(color%RSPEED*(RSPEED-color%RSPEED))/(RSPEED*RSPEED)*255),2*(int)((double)(color%GSPEED*(GSPEED-color%GSPEED))/(GSPEED*GSPEED)*255),2*(int)((double)(color%BSPEED*(BSPEED-color%BSPEED))/(BSPEED*BSPEED)*255));
			image.set_pixel(XRES-i-1,YRES-j-1,2*(int)((double)(color%RSPEED*(RSPEED-color%RSPEED))/(RSPEED*RSPEED)*255),2*(int)((double)(color%GSPEED*(GSPEED-color%GSPEED))/(GSPEED*GSPEED)*255),2*(int)((double)(color%BSPEED*(BSPEED-color%BSPEED))/(BSPEED*BSPEED)*255));
		}
	}
	}else{
	for(int i=0; i<XRES; i++){
		for(int j=0; j<YRES; j++){
			int color = iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),puissance);
			//image.set_pixel(XRES-i-1,j,color/100*10+20,color/10%10*10+20,color%10*20+30);
			image.set_pixel(XRES-i-1,j,2*(int)((double)(color%RSPEED*(RSPEED-color%RSPEED))/(RSPEED*RSPEED)*255),2*(int)((double)(color%GSPEED*(GSPEED-color%GSPEED))/(GSPEED*GSPEED)*255),2*(int)((double)(color%BSPEED*(BSPEED-color%BSPEED))/(BSPEED*BSPEED)*255));

		}
	}
	}
	return image;
}

int iteratej(int iterations, double x, double y,int puissance,double x0,double y0){
	int bound = 0;
	while(norme(x,y)<=4&&bound<iterations){
		bound++;
		double tempx=x;
		double tempy=y;
		double x1;
		for (int i = 1; i < puissance; ++i)
		{
			x1 = x;
			x = x*tempx-y*tempy;
			y = y*tempx+x1*tempy;
		}
		x=x+x0;
		y=y+y0;
	}
	if (bound==iterations-1){
		return 0;
	}
	return bound;
}

bitmap_image generatej(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance,double x,double y){
	for(int i=0; i<XRES; i++){
		for(int j=0; j<YRES; j++){
			int color = iteratej(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),puissance,x,y);
			//image.set_pixel(XRES-i-1,j,color/100*10+20,color/10%10*10+20,color%10*20+30);
			image.set_pixel(XRES-i-1,j,2*(int)((double)(color%RSPEED*(RSPEED-color%RSPEED))/(RSPEED*RSPEED)*255),2*(int)((double)(color%GSPEED*(GSPEED-color%GSPEED))/(GSPEED*GSPEED)*255),2*(int)((double)(color%BSPEED*(BSPEED-color%BSPEED))/(BSPEED*BSPEED)*255));

		}
	}
	return image;
}

int parse_digit(char digit) {
    return digit - '0';
}

int mandel(int argc,char**argv){
	if(argc!=13){
		usage(argv[0]);
		return EXIT_FAILURE;
	}
		string name = string(argv[1]);
		int numFrames = stoi(argv[2]);
		int beginframe = stoi(argv[3]);
		int iter = stoi(argv[4]);
	  int XRES = stoi(argv[5]);
	  int YRES = stoi(argv[6]);
		double widthx = stold(argv[7]);
		double widthy = widthx/XRES*YRES;
		double zoomfactor = stold(argv[8]);
		double zoomx = stold(argv[9]);
		double zoomy = stold(argv[10]);
		char zoomtype = argv[11][0];
		int puissance = stoi(argv[12]);

	// else{
	// 	cout<<"Entrez le nom à sauvegarder."<<endl;
	// 	getline (std::cin,name);
	// 	cout<<"Entrez le nombre d'images à générer."<<endl;
	// 	getline (std::cin,parse_digit(numFrames));
	// 	cout<<"Entrez la première image à sauvegarder."<<endl;
	// 	cout<<"Entrez le nombre d'iterations maximal."<<endl;
	// 	cout<<"Entrez la résolution horizontale."<<endl;
	// 	cout<<"Entrez la résolution verticale."<<endl;
	// 	cout<<"Entrez la demi largeur."<<endl;
	// 	double widthy = widthx/XRES*YRES;
	// 	cout<<"Entrez la coordonnée en X du point de zoom"<<endl;
	// 	cout<<"Entrez la coordonnée en Y du point de zoom"<<endl;
	// 	cout<<"Entrez le type de zoom."<<endl;

	// }
	double XMIN ;
	double XMAX ;
	double YMIN ;
	double YMAX ;
	ostringstream number;
	bitmap_image image(XRES,YRES);

	if(beginframe!=0){
		widthx = widthx/pow(zoomfactor,beginframe);
		widthy = widthy/pow(zoomfactor,beginframe);
	}
	switch (zoomtype) {
		case 'a':
			for (int i = beginframe; i < numFrames+beginframe; i++)
			{
				XMIN = zoomx-widthx;
				XMAX = zoomx+widthx;
				YMIN = zoomy-widthy;
				YMAX = zoomy+widthy;
				number.str("");
				number << i ;
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance).save_image(name+number.str() +".bmp");
				widthx /= zoomfactor;
				widthy /= zoomfactor;
			}
			break;
		case 'b':
			XMIN = zoomx-widthx;
			XMAX = zoomx+widthx;
			YMIN = zoomy-widthy;
			YMAX = zoomy+widthy;
			for(int i = beginframe; i<=numFrames+beginframe; i++)
			{
				number.str("");
				number << i ;
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance).save_image(name+number.str() +".bmp");
				iter++;
			}
			break;
		case 'e':
			widthx = stold(argv[7]);
			widthy = widthx;
			YRES = XRES*(2*numFrames-1);
			YMIN = zoomy-widthy;
			YMAX = zoomy+widthy;
			for (int i = -numFrames+1+beginframe; i < numFrames; ++i)
			{
				XMIN = zoomx+2*i*widthx/(2*numFrames-1)-widthx/(2*numFrames-1);
				XMAX = zoomx+2*i*widthx/(2*numFrames-1)+widthx/(2*numFrames-1);
				number.str("");
				number << (i+numFrames-1);
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance).save_image(name+number.str()+".bmp");
			}
			break;
		case 'j':
		default:
			for (int i = beginframe; i < numFrames+beginframe; i++)
			{
				XMIN = 0-widthx;
				XMAX = 0+widthx;
				YMIN = 0-widthy;
				YMAX = 0+widthy;
				number.str("");
				number << i ;
				generatej(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,zoomx,zoomy).save_image(name+number.str() +".bmp");
				widthx /= zoomfactor;
				widthy /= zoomfactor;
			}
			break;
	}
	return 0;

}
