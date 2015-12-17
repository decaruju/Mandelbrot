#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <boost/multiprecision/gmp.hpp>
#include "bitmap_image.hpp"


const int RSPEED = 29;
const int GSPEED = 17;
const int BSPEED = 11;

double norme(double x, double y){
	return (x*x+y*y);
}

void complexpower(double *x, double *y, int p, double cr, double cs, double ct, double sr, double ss, double st){
	double x1;
	double tempx=*x;
	double tempy=*y;
	for (int i = 1; i < p; ++i)
	{
		x1=*x;
		*x = x1*tempx*cr+tempy**y*cs+(x1*tempy+*y*tempx)*ct;
		*y = x1*tempx*sr+tempy**y*ss+(x1*tempy+*y*tempx)*st;
	}
}
void complexpower(double *x, double *y, int p){
	double x1;
	double tempx=*x;
	double tempy=*y;
	for (int i = 1; i < p; ++i)
	{
		x1=*x;
		*x = x1*tempx-tempy**y;
		*y = (x1*tempy+*y*tempx);
	}
}

int iterate(int iterations, double x, double y,double x0, double y0,int puissance,double cr,double cs,double ct,double sr,double ss,double st,double *norm){
	while(*norm<=4&&iterations>0){
		iterations--;
		complexpower(&x,&y,puissance,cr,cs,ct,sr,ss,st);
		x=x+x0;
		y=y+y0;
		*norm = norme(x,y);
	}
	return iterations;
}

void colorsetter(int color,int *red,int *green,int *blue){
	*red = 2*(int)((double)(color%RSPEED*(RSPEED-color%RSPEED))/(RSPEED*RSPEED)*255);
	*green = 2*(int)((double)(color%GSPEED*(GSPEED-color%GSPEED))/(GSPEED*GSPEED)*255);
	*blue = 2*(int)((double)(color%BSPEED*(BSPEED-color%BSPEED))/(BSPEED*BSPEED)*255);
}

bitmap_image generate(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance, double cr, double cs, double ct, double sr, double ss, double st){
	#pragma omp parallel for schedule(dynamic)
		for(int i=0; i<XRES; i++){
			for(int j=0; j<YRES; j++){
				int red;
				int green;
				int blue;
				int color;
				double norm;
				norm=0;
				color = iter-iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),puissance,cr,cs,ct,sr,ss,st,&norm);
				if( color == iter ){
					//red=127-(norm/2)*127;
					red=255;
					//green=127-(norm/2)*127;
					green=255;
					//blue=127-(norm/2)*127;
					blue=255;
				}else{
					colorsetter(color,&red,&green,&blue);
				}
				image.set_pixel(XRES-i-1,j,green,red,blue);
		}
	}
	return image;
}

int set_distance(double x,double y,int iterations){
	int count=0;
	double zx,zy, tempzx,dzx,dzy, tempdzx;
	zx=0;zy=0;
	dzx=0;dzy=0;
	while (iterations>count) {
		tempdzx=dzx;
		dzx=2*(zx*dzx-zy*dzy)+1;
		dzy=2*(zy*tempdzx+zx*dzy);
		tempzx=zx;
		zx=(zx*zx-zy*zy)+x;
		zy=(tempzx*zy+zy*tempzx)+y;
		count++;
		if (norme(zx,zy)>=4) {
			return count;
		}
	}
	return count;
}

bitmap_image generatedistance(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance, double cr, double cs, double ct, double sr, double ss, double st){
	#pragma omp parallel for schedule(dynamic)
		for(int i=0; i<XRES; i++){
			for(int j=0; j<YRES; j++){
				int red;
				int green;
				int blue;
				int color;
				color=set_distance((XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),iter);
				if( color == iter ){
					red=0;
					green=0;
					blue=0;
				}else{
					colorsetter(color,&red,&green,&blue);
				}
				image.set_pixel(XRES-i-1,j,color,color,color);
		}
	}
	return image;
}

void averagecolor(bitmap_image img, unsigned char *red, unsigned char *green, unsigned char *blue){
	double sommered = 0;
	double sommegreen = 0;
	double sommeblue = 0;
	for (unsigned int i = 0; i < img.width(); i++) {
		for (unsigned int j = 0; j <img.height() ; j++) {
			sommeblue = sommeblue+(double)img.blue_channel(i,j);
			sommegreen = sommegreen+(double)img.green_channel(i,j);
			sommered = sommered+(double)img.red_channel(i,j);
		}
	}
	sommeblue/=(img.width()*img.height());
	sommegreen/=(img.width()*img.height());
	sommered/=(img.width()*img.height());
	*blue = (unsigned char) (sommeblue);
	*green = (unsigned char) (sommegreen);
	*red = (unsigned char) (sommered);
}

bitmap_image generatej(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance,double x,double y,double cr, double cs, double ct, double sr, double ss, double st){
	#pragma omp parallel for schedule(dynamic)
		for(int i=0; i<XRES; i++){
			for(int j=0; j<YRES; j++){
				int red;
				int green;
				int blue;
				int color;
				double norm;
				norm=0;
				color = iter-iterate(iter,(XMIN + i * (XMAX - XMIN) / XRES),(YMIN + j * (YMAX - YMIN) / YRES),x,y,puissance,cr,cs,ct,sr,ss,st,&norm);
				if( color == iter ){
					//red=(1023-(norm/2)*1023);
					red=255;
					//green=(1023-(norm/2)*1023);
					green=255;
					//blue=(1023-(norm/2)*1023);
					blue=255;
				}else{
					colorsetter(color,&red,&green,&blue);
				}
				image.set_pixel(XRES-i-1,j,green,red,blue);
		}
	}
	return image;
}

bitmap_image generatejulia(int iter,int XRES, int YRES, double XMAX, double XMIN, double YMIN, double YMAX,bitmap_image image,int puissance, double cr, double cs, double ct, double sr, double ss, double st){
		for(int i=0; i<XRES; i++){
			for(int j=0; j<YRES; j++){
				unsigned char red;
				unsigned char green;
				unsigned char blue;
				averagecolor(generatej(iter,1,1,0,0,0,0,image,puissance,(((XMAX-XMIN)*i)/XRES)+XMIN,(((YMAX-YMIN)*j)/YRES)+YMIN,cr,cs,ct,sr,ss,st),&red,&green,&blue);
				image.set_pixel(i,j,red,green,blue);
		}
	}
	return image;
}
