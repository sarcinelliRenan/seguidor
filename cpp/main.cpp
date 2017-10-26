/**
*/
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include "libs/raspicam/raspicam.h"
using namespace std;

//timer functions
#include <sys/time.h>
#include <unistd.h>

#define threshold 50
#define view_pixel_height 60 //Middle of image
	
class Timer{
    private:
    struct timeval _start, _end;

public:
    Timer(){}
    void start(){
        gettimeofday(&_start, NULL);
    }
    void end(){
        gettimeofday(&_end, NULL);
    }
    double getSecs(){
    return double(((_end.tv_sec  - _start.tv_sec) * 1000 + (_end.tv_usec - _start.tv_usec)/1000.0) + 0.5)/1000.;
    }

};

void saveImage ( string filepath,unsigned char *data,raspicam::RaspiCam &Camera ) {
    std::ofstream outFile ( filepath.c_str(),std::ios::binary );
    if ( Camera.getFormat()==raspicam::RASPICAM_FORMAT_BGR ||  Camera.getFormat()==raspicam::RASPICAM_FORMAT_RGB ) {
        outFile<<"P6\n";
    } else if ( Camera.getFormat()==raspicam::RASPICAM_FORMAT_GRAY ) {
        outFile<<"P5\n";
    } else if ( Camera.getFormat()==raspicam::RASPICAM_FORMAT_YUV420 ) { //made up format
        // Openable as a .yuv format
        outFile.write ( ( char* ) data,Camera.getImageBufferSize() );
        return;
    }
    outFile<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data,Camera.getImageBufferSize() );
}



int main ( int argc,char **argv ) {
	size_t nFramesCaptured=100;
	double error = 0;

    raspicam::RaspiCam Camera; //Cmaera object
    //Open camera 
	Camera.setWidth(320);
	Camera.setHeight(240);
	Camera.setFrameRate(90);
	Camera.setSensorMode(7);
	Camera.setFormat(raspicam::RASPICAM_FORMAT_GRAY);
	
    cout<<"Opening Camera..."<<endl;
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
    //wait a while until camera stabilizes
    sleep(5);
    
    double atans[320];
    for (int i=0; i<320; i++)
    	atans[i] = atan(0.0043261*(160.0-i));
    	
    cout<<"Connected to camera ="<<Camera.getId() <<" bufs="<<Camera.getImageBufferSize( )<<endl;
    unsigned char *data=new unsigned char[  Camera.getImageBufferSize( )];
    Timer timer;

    cout<<"Capturing...."<<endl;
    size_t i=0;
    timer.start();
    
    do{
        Camera.grab();
        Camera.retrieve ( data );
        
        //median filter //todo
        /*
       	for(int i=0; i<320; i++){
       		int med_vector[49];
       		for(int j=0; j<49; j++){
       			
       	}
       	*/
       	for(int i=0; i<320; i++){
       		data[view_pixel_height*320 + i] = ((data[view_pixel_height*320 + i]) < threshold) ? 0 : 1;
       	}
       	
       	error = 0;
       	for(int i=0; i<320; i++)
       		error +=  (double)(data[view_pixel_height*320 + i])*atans[i];
       	
       	printf("%lf\n",error);
       	
    }while(true);//(++i<nFramesCaptured || nFramesCaptured==0);//stops when nFrames captured or at infinity lpif nFramesCaptured<0
    
    timer.end();
 
 	for(int i=0; i<320; i++)
       		printf(" %lf ",atans[i]);
       	
       	
 
	saveImage("imagem2.WebP",data,Camera);
    cerr<< timer.getSecs()<< " seconds for "<< nFramesCaptured<< "  frames : FPS " << ( ( float ) ( nFramesCaptured ) / timer.getSecs() ) <<endl;

    Camera.release();

}
