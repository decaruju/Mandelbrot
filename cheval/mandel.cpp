#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <boost/multiprecision/gmp.hpp>
#include "bitmap_image.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include "generate.cpp"

using boost::multiprecision::mpf_float_100;
using namespace std;

#define PI 3.14159265359

void usage(char*prog){
		cout << "Usage : "<<prog<<" <output name> <nb frames> <first frame> <nb iterations> <XRES> <YRES>"
			<<" <width> <zoomfactor> <zoomx> <zoomy> <zoomtype> <power>where a=zoom, b=iteration, c=xtrans, d=ytrans, e=tiles"<<endl;
}

string zeropad(string name,int n){
	for (int i = 0; i < name.length()-n; i++) {
		name = "0"+name;
	}
	return name;
}
string name;
int numFrames;
int beginframe;
int iter;
int XRES;
int YRES;
double widthx;
double widthy;
double zoomfactor;
double zoomx;
double zoomy;
char zoomtype;
int puissance;
double XMIN;
double XMAX;
double YMIN;
double YMAX;
double r;
double s;
double t;
double cr;
double cs;
double ct;
double sr;
double ss;
double st;
int main(int argc,char**argv){
	if(argc==1){
	  Fl_Window win(720,486);
	  win.begin();
		  Fl_Button *button = new Fl_Button(460, 20, 200, 20, "Generate");
		  Fl_Input *xresfield = new Fl_Input(20,60,200,20);
		  Fl_Input *yresfield = new Fl_Input(20,100,200,20);
		  Fl_Input *iterfield = new Fl_Input(20,140,200,20);
		  Fl_Input *numFramesfield = new Fl_Input(20,180,200,20);
		  Fl_Input *beginframefield = new Fl_Input(240,20,200,20);
		  Fl_Input *widthxfield = new Fl_Input(240,60,200,20);
		  Fl_Input *widthyfield = new Fl_Input(240,100,200,20);
		  Fl_Input *zoomfactorfield = new Fl_Input(20,20,200,20);
		  Fl_Input *zoomxfield = new Fl_Input(240,140,200,20);
		  Fl_Input *zoomyfield = new Fl_Input(240,180,200,20);


		  //string name = string(argv[1]);







		  //char zoomtype = argv[11][0];
		  //int puissance = stoi(argv[12]);
		  //double XMIN ;
		  //double XMAX ;
		  //double YMIN ;
		  //double YMAX ;
		  //double r=0;
		  //double s=PI;
		  //double t=PI/2;
		  //double cr = cos(r);
		  //double cs = cos(s);
		  //double ct = cos(t);
		  //double sr = sin(r);
		  //double ss = sin(s);
		  //double st = sin(t);
	  win.end();
      win.show();
      return(Fl::run());
	}
	if(argc!=13){
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	if (argc=13) {
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
		double XMIN ;
		double XMAX ;
		double YMIN ;
		double YMAX ;
		double r=0;
		double s=PI;
		double t=PI/2;
		double cr = cos(r);
		double cs = cos(s);
		double ct = cos(t);
		double sr = sin(r);
		double ss = sin(s);
		double st = sin(t);
	}
	ostringstream number;
	bitmap_image image(XRES,YRES);
	int pipa=0;

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
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name+zeropad(number.str(),5) +".bmp");
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
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name+number.str() +".bmp");
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
				generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name+number.str()+".bmp");
			}
			break;
			case 'j':
				for (int i = beginframe; i < numFrames+beginframe; i++)
				{
					XMIN = 0-widthx;
					XMAX = 0+widthx;
					YMIN = 0-widthy;
					YMAX = 0+widthy;
					number.str("");
					number << i ;
					generatej(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,zoomx,zoomy,cr,cs,ct,sr,ss,st).save_image(name+number.str() +".bmp");
					widthx /= zoomfactor;
					widthy /= zoomfactor;
				}
			break;
			case 'k':
			for (int k = 0; k <1; k++) {
				for (int j = 0; j < numFrames; j++) {
					for (int i = 0; i < numFrames; i++)
					{
						XMIN = zoomx-widthx;
						XMAX = zoomx+widthx;
						YMIN = zoomy-widthy;
						YMAX = zoomy+widthy;
						number.str("");
						number << pipa ;
						cr = cos(r);
						cs = cos(s);
						ct = cos(t);
						sr = sin(r);
						ss = sin(s);
						st = sin(t);
						generate(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name+zeropad(number.str(),5) +".bmp");
						pipa++;
						r=r+2*PI/numFrames;
						t=t+2*PI/(numFrames);
						s=s+2*PI/numFrames;
					}
						t=t+2*PI/(numFrames);
				}
						s=s+2*PI/numFrames;
			}
			break;
			case 'l':
				XMIN = 0-widthx;
				XMAX = 0+widthx;
				YMIN = 0-widthy;
				YMAX = 0+widthy;
				generatejulia(iter,XRES,YRES,XMAX,XMIN,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name +".bmp");
			break;
			case 'm':
				for (int i = beginframe; i < numFrames+beginframe; i++)
				{
					XMIN = zoomx-widthx;
					XMAX = zoomx+widthx;
					YMIN = zoomy-widthy;
					YMAX = zoomy+widthy;
					number.str("");
					number << i ;
					generatedistance(iter,XRES,YRES,XMIN,XMAX,YMIN,YMAX,image,puissance,cr,cs,ct,sr,ss,st).save_image(name+zeropad(number.str(),5) +".bmp");
					widthx /= zoomfactor;
					widthy /= zoomfactor;
				}
				break;
			default:;
	}
	return 0;
}
//g++ `fltk-config --cxxflags` mandel.cpp `fltk-config --ldflags` -o mandel -lgmp -std=c++11
