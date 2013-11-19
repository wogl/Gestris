#include "opencv2/opencv.hpp" 
#include <Windows.h>
using namespace cv; 
using namespace std;
void ScaleUP(int , Mat, Mat*);
void GravityAndUpdate(int*, bool*,int*,int*, float, Mat*, bool*);
void Collision(Mat, Mat, bool*, bool*, bool*, bool*);
void creatMask(Mat, Mat*);
void initializeBricks(Mat*, int, int, int,int);
void initializeBricksMeta(Mat*,Mat*, int, int, int,int*);



void Tetris(){
	//Initialisieren
	Mat image(21, 10, CV_8UC3);	
	Mat background(21, 10, CV_8UC3);
	Mat mask(21, 10, CV_8UC3);
	Mat meta_mask(21, 10, CV_8UC3);
	
	
	int scale = 30;
	Mat resizedImage(image.size()*scale,image.type());
	Mat test(mask.size()*scale,mask.type());
	
	int start=0,end=0;
	int gravity=0;
	int move=0;
	int rotate=1;
	int app=0;

	bool zeit=true;
	bool wait=false;
	float speed = 500;

	bool b_collision=false;
	bool r_collision=false;
	bool l_collision=false;
	bool rotate_collision = false;

	background = 0;
	image = 0;
	
	//Update
	while(true){
		
		//
		rectangle(image,Point(0,20),Point(10,20),Scalar(0,100,0));
		
		
		
		
		

		initializeBricks(&image,move,gravity,rotate,app);
		creatMask(image, &mask);	
		initializeBricksMeta(&meta_mask,&mask,move,gravity,rotate,&app);// Metadaten in mask
		
		ScaleUP(scale, mask, &resizedImage);
		ScaleUP(scale, meta_mask, &test);
		Collision(meta_mask,mask, &b_collision, &r_collision, &l_collision, &rotate_collision);
		GravityAndUpdate(&gravity, &zeit,&start,&end, speed, &image, &wait);
		
		cout << app << endl;
	
		
			

		if(b_collision && wait){
			image.copyTo(background);
			gravity = 0;
			move = 0;
			b_collision = false;
			app=0;
		}




		namedWindow( "Tetris"); 
		namedWindow( "Final_Tetris"); 
		namedWindow("Test");
		imshow( "Tetris", resizedImage); 
		imshow( "Final_Tetris", meta_mask ); 
		imshow("Test", test);
		int key = waitKey(1); 
		image = 0 + background;
		meta_mask =0;
		
		
		


		//Control
		if(key == 27){ //Escape
				break;
			}
		if(key == 2555904 && move+app < 4 && !r_collision){ //ArrowRight
				move++;
			
			}
		if(key == 2424832 && move+app > -6 && !l_collision){ //ArrowLeft
				move = move-1;
			
			}
		if(key == 2490368 && !rotate_collision){ //Arrow-Up (Rotate)
			rotate++;			
			if(rotate > 4){
				rotate = 1;
			}	

		}
		}
		
	
}

void ScaleUP(int scale, Mat image, Mat* resizedImage){

	for(int x = 0; x<10; x++){
		for(int y=0; y<21; y++){
			
			if(image.type() ==  CV_8UC3){
			Scalar pixel = image.at<Vec3b>(y,x);
			rectangle(*resizedImage,Point(x*scale,y*scale),Point((x+1)*scale,(y+1)*scale),pixel,-1);
			}

			if(image.type() ==  CV_8UC1){
			Scalar pixel = image.at<uchar>(y,x);
			rectangle(*resizedImage,Point(x*scale,y*scale),Point((x+1)*scale,(y+1)*scale),pixel,-1);
			}
		}
	}


}

void GravityAndUpdate(int* gravity, bool* zeit,int* start, int* end, float speed,Mat* image,bool* wait){
		
		
		SYSTEMTIME time_s, time_e;
		GetSystemTime(&time_s);
		GetSystemTime(&time_e);
		*wait=false;
	
	    if(*zeit){
		
			*start =time_s.wMinute*60000 + time_s.wSecond*1000 + time_s.wMilliseconds;
			*zeit = false;
		}

		*end = time_e.wMinute*60000 + time_e.wSecond*1000+ time_e.wMilliseconds;

		if((*end-*start) > speed-50){
		*wait=true;
			
		}
		
		if((*end-*start) > speed){
		*gravity = *gravity + 1;
		*zeit = true;
			
		}
}

void Collision(Mat meta_mask, Mat mask, bool* b_collision, bool* r_collision, bool* l_collision, bool* rotate_collision){
	
	bool meta_background = false;
	bool meta_brick = false;
	*r_collision = false;
	*l_collision = false;
	*rotate_collision = false;
	*b_collision = false;


		for(int y=0; y < mask.rows; y++){
			for(int x=0; x < mask.cols;x++){

				if(mask.at<Vec3b>(y,x)[0] == 255){
					meta_background = true;
				}
				if(mask.at<Vec3b>(y,x)[2] == 255){
					meta_brick = true;
				}
				if(mask.at<Vec3b>(y,x)[0] == 0 && mask.at<Vec3b>(y,x)[1] == 0 && mask.at<Vec3b>(y,x)[2] == 0){
					meta_brick = false;
					meta_background = false;
				}
				if((mask.at<Vec3b>(y,x)[0] == 255 && meta_brick)||(mask.at<Vec3b>(y,x)[2] == 255 && x==mask.cols-1)){
					*r_collision = true;
				}
				if((mask.at<Vec3b>(y,x)[2] == 255 && meta_background)||(mask.at<Vec3b>(y,x)[2] == 255 && x==0)){
					*l_collision = true;
				}

				if(mask.at<Vec3b>(y,x)[2] == 255 && mask.at<Vec3b>(y+1,x)[0] == 255){
					*b_collision = true;
				}
				

				if(mask.at<Vec3b>(y,x)[0] == 255 && meta_mask.at<Vec3b>(y,x)[1] == 255){
					*rotate_collision = true;
				}
			}
		}
		

}

void creatMask(Mat image, Mat* mask){

	for(int y=0; y < image.rows; y++){
		for(int x=0; x < image.cols; x++){
			
				if(image.at<Vec3b>(y,x)[0] > 0 || image.at<Vec3b>(y,x)[1] > 0 || image.at<Vec3b>(y,x)[2] > 0 ){
					rectangle(*mask, Point(x,y),Point(x+1,y+1),Scalar(255,0,0));
				}
				else{
					rectangle(*mask, Point(x,y),Point(x+1,y+1),Scalar(0,0,0));
				}
			
		}
	}
}

void initializeBricks(Mat* image, int move, int gravity, int rotate, int app){

	int random = rand() % 3 +1;
	//int random = 3;
	bool I= false;
	bool O= false;
	bool L= false;
	bool L_mirror = false;
	bool S= false;
	bool S_mirror = false;
	bool T= false;

	switch(random){
		case 1: I = true;
			break;

		case 2: O = true;
			break;

		case 3: L = true;
			break;

		case 4: L_mirror = true;
			break;

		case 5: S = true;
			break;

		case 6: S_mirror = true;
			break;

		case 7: T = true;
	}

	if(I){
		switch(rotate){
		case 1:	rectangle(*image,Point(5+move+app,0+gravity),Point(5+move+app,3+gravity),Scalar(0,0,255));
				break;

		case 2:	rectangle(*image,Point(4+move+app,1+gravity),Point(7+move+app,1+gravity),Scalar(0,0,255));
				break;

		case 3:	rectangle(*image,Point(6+move+app,0+gravity),Point(6+move+app,3+gravity),Scalar(0,0,255));
				break;

		case 4:	rectangle(*image,Point(4+move+app,2+gravity),Point(7+move+app,2+gravity),Scalar(0,0,255));
				break;
		}
	}

	if(O){
		rectangle(*image,Point(4+move,0+gravity),Point(5+move,1+gravity),Scalar(0,0,255));
	}

	if(L){
		
		switch(rotate){
			case 1:	rectangle(*image,Point(5+move+app,0+gravity),Point(5+move+app,2+gravity),Scalar(0,0,255));
					rectangle(*image,Point(5+move+app,2+gravity),Point(6+move+app,2+gravity),Scalar(0,0,255));				
					break;

			case 2:	rectangle(*image,Point(4+move+app,1+gravity),Point(6+move+app,1+gravity),Scalar(0,0,255));
					rectangle(*image,Point(4+move+app,1+gravity),Point(4+move+app,2+gravity),Scalar(0,0,255));
					break;

			case 3:	rectangle(*image,Point(6+move+app,0+gravity),Point(6+move+app,2+gravity),Scalar(0,0,255));
					rectangle(*image,Point(5+move+app,0+gravity),Point(6+move+app,0+gravity),Scalar(0,0,255));
					break;

			case 4:	rectangle(*image,Point(4+move+app,2+gravity),Point(6+move+app,2+gravity),Scalar(0,0,255));
					rectangle(*image,Point(6+move+app,1+gravity),Point(6+move+app,2+gravity),Scalar(0,0,255));
					break;
			}



	}


}

void initializeBricksMeta(Mat* meta_mask,Mat* mask, int move, int gravity, int rotate, int* app){

	int random = rand() % 3 +1;
	//int random = 3;
	
	bool I= false;
	bool O= false;
	bool L= false;
	bool L_mirror = false;
	bool S= false;
	bool S_mirror = false;
	bool T= false;

	switch(random){
		case 1: I = true;
			break;

		case 2: O = true;
			break;

		case 3: L = true;
			break;

		case 4: L_mirror = true;
			break;

		case 5: S = true;
			break;

		case 6: S_mirror = true;
			break;

		case 7: T = true;
	}

	if(I){
		switch(rotate){
		case 1:	rectangle(*mask,Point(5+move+*app,0+gravity),Point(5+move+*app,3+gravity),Scalar(0,0,255));
				rectangle(*meta_mask,Point(4+move+*app,1+gravity),Point(7+move+*app,1+gravity),Scalar(0,255,0));
				if(5+move+*app > 9){
					*app=*app-1;
				}
				if(5+move+*app < 0){
					*app=*app+1;
				}
				break;

		case 2:	rectangle(*mask,Point(4+move+*app,1+gravity),Point(7+move+*app,1+gravity),Scalar(0,0,255));
				rectangle(*meta_mask,Point(6+move+*app,0+gravity),Point(6+move+*app,3+gravity),Scalar(0,255,0));
				if(7+move+*app > 9){
					*app=*app-1;
				}
				if(4+move+*app < 0){
					*app=*app+1;
				}
				break;

		case 3:	rectangle(*mask,Point(6+move+*app,0+gravity),Point(6+move+*app,3+gravity),Scalar(0,0,255));
				rectangle(*meta_mask,Point(4+move+*app,2+gravity),Point(7+move+*app,2+gravity),Scalar(0,255,0));
				if(6+move+*app > 9){
					*app=*app-1;
				}
				if(6+move+*app < 0){
					*app=*app+1;
				}
				break;

		case 4:	rectangle(*mask,Point(4+move+*app,2+gravity),Point(7+move+*app,2+gravity),Scalar(0,0,255));
				rectangle(*meta_mask,Point(5+move+*app,0+gravity),Point(5+move+*app,3+gravity),Scalar(0,255,0));
				if(7+move+*app > 9){
					*app=*app-1;
				}
				if(4+move+*app < 0){
					*app=*app+1;
				}
				break;
		}
	}

	if(O){
		rectangle(*mask,Point(4+move,0+gravity),Point(5+move,1+gravity),Scalar(0,0,255));
	}

	if(L){
		
		switch(rotate){
			case 1:	rectangle(*mask,Point(5+move+*app,0+gravity),Point(5+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*mask,Point(5+move+*app,2+gravity),Point(6+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*meta_mask,Point(4+move+*app,1+gravity),Point(6+move+*app,1+gravity),Scalar(0,255,0));
					rectangle(*meta_mask,Point(4+move+*app,1+gravity),Point(4+move+*app,2+gravity),Scalar(0,255,0));
				if(6+move+*app > 9){
					*app=*app-1;
				}
				if(5+move+*app < 0){
					*app=*app+1;
				}
				break;

			case 2:	rectangle(*mask,Point(4+move+*app,1+gravity),Point(6+move+*app,1+gravity),Scalar(0,0,255));
					rectangle(*mask,Point(4+move+*app,1+gravity),Point(4+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*meta_mask,Point(6+move+*app,0+gravity),Point(6+move+*app,2+gravity),Scalar(0,255,0));
					rectangle(*meta_mask,Point(5+move+*app,0+gravity),Point(6+move+*app,0+gravity),Scalar(0,255,0));
					if(6+move+*app > 9){
						*app=*app-1;
					}
					if(4+move+*app < 0){
						*app=*app+1;
					}
					break;

			case 3:	rectangle(*mask,Point(6+move+*app,0+gravity),Point(6+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*mask,Point(5+move+*app,0+gravity),Point(6+move+*app,0+gravity),Scalar(0,0,255));
					rectangle(*meta_mask,Point(4+move+*app,2+gravity),Point(6+move+*app,2+gravity),Scalar(0,255,0));
					rectangle(*meta_mask,Point(6+move+*app,1+gravity),Point(6+move+*app,2+gravity),Scalar(0,255,0));
					if(6+move+*app > 9){
						*app=*app-1;
					}
					if(5+move+*app < 0){
						*app=*app+1;
					}
					break;

			case 4:	rectangle(*mask,Point(4+move+*app,2+gravity),Point(6+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*mask,Point(6+move+*app,1+gravity),Point(6+move+*app,2+gravity),Scalar(0,0,255));
					rectangle(*meta_mask,Point(5+move+*app,0+gravity),Point(5+move+*app,2+gravity),Scalar(0,255,0));
					rectangle(*meta_mask,Point(5+move+*app,2+gravity),Point(6+move+*app,2+gravity),Scalar(0,255,0));
					if(6+move+*app > 9){
						*app=*app-1;
					}
					if(4+move+*app < 0){
						*app=*app+1;
					}
					break;
			}



	}

}



int main(){ 	
	Tetris();
	
}