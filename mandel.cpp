#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// #include <boost/multiprecision/gmp.hpp>
#include "bitmap_image.hpp"

using namespace std;
//using boost::multiprecision::double;

void noob(char*prog){
		cout << "Usage : "<<prog<<" <output name> <nb frames> <first frame> <nb iterations> <XRES> <YRES>"
			<<" <width> <zoomfactor> <zoomx> <zoomy> <zoomtype> where a=zoom, b=iteration, c=xtrans, d=ytrans, e=tiles"<<endl;
}

double norme(double x, double y){
	return (x*x+y*y);
}

int iterate(int iterations, double x, double y){
	int bound = 0;
	double x0 = x;
	double y0 = y;
	if (norme(x+1,y)<0.0625||norme(x+0.25,y)<0.25)
	{
		return iterations;
	}
	while(norme(x,y)<=4&&bound<iterations){
		bound++;
		double x1 = x;
		x = x*x-y*y+x0;
		y = 2*x1*y+y0;
	}
	return bound;
}
	int rspeed = 101;
	int gspeed = 71;
	int bspeed = 43;
bitmap_image generate(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image){
	if (YMIN+YMAX==0)
	{
	for(int i=0; i<XRES; i++){
		for(int j=0; j<YRES/2; j++){
			int color = iter-iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES));
			//image.set_pixel(XRES-i-1,j,color/100*10+20,color/10%10*10+20,color%10*20+30);
			image.set_pixel(XRES-i-1,j,2*(int)((double)(color%rspeed*(rspeed-color%rspeed))/(rspeed*rspeed)*255),2*(int)((double)(color%gspeed*(gspeed-color%gspeed))/(gspeed*gspeed)*255),2*(int)((double)(color%bspeed*(bspeed-color%bspeed))/(bspeed*bspeed)*255));
			image.set_pixel(XRES-i-1,YRES-j-1,2*(int)((double)(color%rspeed*(rspeed-color%rspeed))/(rspeed*rspeed)*255),2*(int)((double)(color%gspeed*(gspeed-color%gspeed))/(gspeed*gspeed)*255),2*(int)((double)(color%bspeed*(bspeed-color%bspeed))/(bspeed*bspeed)*255));		
		}
		cout << i<<"caca" << endl;
	}
	}else{
	for(int i=0; i<XRES; i++){
		for(int j=0; j<YRES; j++){
			int color = iter-iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES));
			//image.set_pixel(XRES-i-1,j,color/100*10+20,color/10%10*10+20,color%10*20+30);
			image.set_pixel(XRES-i-1,j,2*(int)((double)(color%rspeed*(rspeed-color%rspeed))/(rspeed*rspeed)*255),2*(int)((double)(color%gspeed*(gspeed-color%gspeed))/(gspeed*gspeed)*255),2*(int)((double)(color%bspeed*(bspeed-color%bspeed))/(bspeed*bspeed)*255));
			
		}
		cout << i << endl;
	}
	}
	return image;
}
int main(int argc,char**argv){
	if(argc!=12){
		noob(argv[0]);
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
	string zoomtype = string(argv[11]);
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
	if (zoomtype=="a")
	{
		for (int i = beginframe; i < numFrames+beginframe; i++)
		{
			XMIN = zoomx-widthx;
			XMAX = zoomx+widthx;
			YMIN = zoomy-widthy;
			YMAX = zoomy+widthy;
			number.str("");
			number << i ;
			generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image).save_image(name+number.str() +".bmp");
			widthx /= zoomfactor;
			widthy /= zoomfactor;
		}
	}
	if (zoomtype=="b")
	{
		XMIN = zoomx-widthx;
		XMAX = zoomx+widthx;
		YMIN = zoomy-widthy;
		YMAX = zoomy+widthy;
		for(int i = beginframe; i<=numFrames+beginframe; i++)
		{
			number.str("");
			number << i ;
			generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image).save_image(name+number.str() +".bmp");
			iter++;
		}
	}
	if (zoomtype=="e")
	{
		double widthx = stold(argv[7]);
		double widthy = widthx;
		YRES = XRES*(2*numFrames-1);
		YMIN = zoomy-widthy;
		YMAX = zoomy+widthy;
		for (int i = -numFrames+1+beginframe; i < numFrames; ++i)
		{
			XMIN = zoomx+2*i*widthx/(2*numFrames-1)-widthx/(2*numFrames-1);
			XMAX = zoomx+2*i*widthx/(2*numFrames-1)+widthx/(2*numFrames-1);
			number.str("");
			number << (i+numFrames-1);
			generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image).save_image(name+number.str()+".bmp");
		}
	}
	// widthy = widthy /pow(1.2,42);
	// widthx = widthx /pow(1.2,42);
	// XMIN = zoomx-widthx;
	// XMAX = zoomx+widthx;
	// YMIN = zoomy-widthy;
	// YMAX = zoomy+widthy;
	// for (int i = 17; i < numFrames+17; ++i)
	// {
	// 	number.str("");
	// 	number << i ;
	// 	generate(i,XRES,YRES,XMIN,XMAX,YMIN,YMAX).save_image(name+number.str() +".bmp");
	// }

	return 0;

}

