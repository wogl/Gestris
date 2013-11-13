#include "opencv2/opencv.hpp" 

using namespace std;
using namespace cv; 


void Tetris(){
	int width = 440;
	int height = 820;
	int gravity = 0;
	int move = 0;
	int rotate = 0;
	int random = rand() % 2 + 1;
	int random_color_r;
	int random_color_g;
	int random_color_b;
	bool cube= false;
	bool I= false;
	bool L= false;
	bool S= false;
	bool collision=false;
	bool r_collision=false;
	bool l_collision=false;
	bool test;
	Mat image(height, width, CV_8UC3);
	Mat image_d(height, width, CV_8UC3);
	Mat i_mask2(height, width, CV_8UC1);
	//Mat test_image(height, width, CV_8UC3);


		
	//Startkoordinaten Cube
	int start1_1 = 180;
	int start1_2 = 20;
	int start2_1 = 259;
	int start2_2 = 99;

	image = 0;
	image_d=0;

	namedWindow("Tetris");
	//namedWindow("Test");
	//namedWindow("Test2");
	
	while(true){

		//image.copyTo(test_image);
		

		//Randomizer

		switch(random){
		case 1: cube = true;
			break;

		case 2: I = true;
			break;

		case 3: L = true;
			break;

		case 4: S = true;
			break;
		}

		

		//Position Cube
		if(cube){
		Point position1(start1_1+move, start1_2 + gravity);
		Point position2(start2_1+move, start2_2 + gravity);
		rectangle(image,position2,position1,Scalar(random_color_b,random_color_g,random_color_r),-1);
		}


		//Position I
		if(I && (rotate == 0 || rotate == 2)){
		Point position1_l(start1_1+move, start1_2 + gravity);
		Point position2_l(start2_1-40+move, start2_2+80 + gravity);
		rectangle(image, position2_l,position1_l,Scalar(random_color_b,random_color_g,random_color_r),-1);
		}

		if(I && (rotate == 1 || rotate == 3)){
		Point position1_l(start1_1+move, start1_2 + gravity);
		Point position2_l(start2_1+80+move, start2_2-40 + gravity);
		rectangle(image, position2_l,position1_l,Scalar(random_color_b,random_color_g,random_color_r),-1);
		}
					

	imshow("Tetris", image);
	imshow("Test", image_d);
	
	
	int key = waitKey(10);		


	//Kollisionsabfrage 
		cvtColor(image, i_mask2, CV_BGR2GRAY);
		threshold(i_mask2, i_mask2, 1,255,0);
		
		//cube
		if(cube){
		for(int x = start1_1+move+1; x < start2_1+move; x++){
			for(int y = start1_2+gravity; y < start2_2+gravity; y++){
				
				if(i_mask2.at<uchar>(start2_2+gravity+1,x) > 0)
				{
					collision = true;
				}
				if(i_mask2.at<uchar>(y,start2_1+move+1) > 0)
				{
					r_collision = true;
				}
				else
				{
					r_collision = false;
				}
				if(i_mask2.at<uchar>(y,start1_1+move-1) > 0)
				{
					l_collision = true;
				}
				else
				{
					l_collision = false;
				}
			
			
				
			}
		}
		}
		// I
		if(I && (rotate == 0 || rotate == 2)){
		for(int x = start1_1+move+1; x < start2_1-40+move; x++){
			for(int y = start1_2+gravity; y < start2_2+80+gravity; y++){
				
				if(i_mask2.at<uchar>(start2_2+80+gravity+1,x) > 0)
				{
					collision = true;
				}
				if(i_mask2.at<uchar>(y,start2_1-40+move+1) > 0)
				{
					r_collision = true;
				}
				else
				{
					r_collision = false;
				}
				if(i_mask2.at<uchar>(y,start1_1+move-1) > 0)
				{
					l_collision = true;
				}
				else
				{
					l_collision = false;
				}
			
			
				
			}
		}
		}
		
		if(I && (rotate == 1 || rotate == 3)){
		for(int x = start1_1+move+1; x < start2_1+80+move; x++){
			for(int y = start1_2+gravity; y < start2_2-40+gravity; y++){
				
				if(i_mask2.at<uchar>(start2_2-40+gravity+1,x) > 0)
				{
					collision = true;
				}
				if(i_mask2.at<uchar>(y,start2_1+80+move+1) > 0)
				{
					r_collision = true;
				}
				else
				{
					r_collision = false;
				}
				if(i_mask2.at<uchar>(y,start1_1+move-1) > 0)
				{
					l_collision = true;
				}
				else
				{
					l_collision = false;
				}
			
			
				
			}
		}
		}
		
	
		
	

		
		
		
	if((start2_2+gravity > height-20 && cube) || (start2_2+80+gravity > height-20 && I && (rotate == 0 || rotate == 2))|| (start2_2-40+gravity > height-20 && I && (rotate == 1 || rotate == 3)) || collision ){
		image.copyTo(image_d);
		random_color_r = rand() % 255 + 1;
		random_color_g = rand() % 255 + 1;
		random_color_b = rand() % 256 + 1;
		random = rand() % 2 +1;
		rotate = rand() % 4;
		move = 0;

		//Destroy Lines
		
		int lines [20] = {40,80,120,160,200,240,280,320,360,400,440,480,520,560,600,640,680,720,760,800};
		int cols [10] = {40,80,120,160,200,240,280,320,360,400};
		int count = 0;
		for(int y=0;y<20;y++){
			
			for(int x=0;x<10;x++){
				if(i_mask2.at<uchar>(lines[y],cols[x]) > 0)
				{
					count++;
					
				}
				else
					count = 0;
				

				
				

				if(count == 10)					
				{					
					switch(y)
					{
					
					case 19:rectangle(image,Point(20,height-55),Point(width-20,height-16),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-16-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
										
										image_d.at<Vec3b>(height-16-y_1,x_1) = image.at<Vec3b>(height-40-16-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
						
							
					
					case 18:rectangle(image,Point(20,height-95),Point(width-20,height-56),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-56-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
									
										image_d.at<Vec3b>(height-56-y_2,x_2) = image.at<Vec3b>(height-40-56-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;
						
					case 17:rectangle(image,Point(20,height-135),Point(width-20,height-96),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-96-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
										
										image_d.at<Vec3b>(height-96-y_1,x_1) = image.at<Vec3b>(height-40-96-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
							
					
					case 16:rectangle(image,Point(20,height-175),Point(width-20,height-136),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-136-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
							
										image_d.at<Vec3b>(height-136-y_2,x_2) = image.at<Vec3b>(height-40-136-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 15:rectangle(image,Point(20,height-215),Point(width-20,height-176),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-176-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
									
										image_d.at<Vec3b>(height-176-y_1,x_1) = image.at<Vec3b>(height-40-176-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 14:rectangle(image,Point(20,height-255),Point(width-20,height-216),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-216-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
										
										image_d.at<Vec3b>(height-216-y_2,x_2) = image.at<Vec3b>(height-40-216-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 13:rectangle(image,Point(20,height-295),Point(width-20,height-256),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-256-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
									
										image_d.at<Vec3b>(height-256-y_1,x_1) = image.at<Vec3b>(height-40-256-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 12:rectangle(image,Point(20,height-335),Point(width-20,height-296),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-296-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
							
										image_d.at<Vec3b>(height-296-y_2,x_2) = image.at<Vec3b>(height-40-296-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 11:rectangle(image,Point(20,height-375),Point(width-20,height-336),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-336-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
									
										image_d.at<Vec3b>(height-336-y_1,x_1) = image.at<Vec3b>(height-40-336-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 10:rectangle(image,Point(20,height-415),Point(width-20,height-376),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-376-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
										
										image_d.at<Vec3b>(height-376-y_2,x_2) = image.at<Vec3b>(height-40-376-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 9:rectangle(image,Point(20,height-455),Point(width-20,height-416),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-416-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
								
										image_d.at<Vec3b>(height-416-y_1,x_1) = image.at<Vec3b>(height-40-416-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 8:rectangle(image,Point(20,height-495),Point(width-20,height-456),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-456-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
								
										image_d.at<Vec3b>(height-456-y_2,x_2) = image.at<Vec3b>(height-40-456-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 7:rectangle(image,Point(20,height-535),Point(width-20,height-496),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-496-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
								
										image_d.at<Vec3b>(height-496-y_1,x_1) = image.at<Vec3b>(height-40-496-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 6:rectangle(image,Point(20,height-575),Point(width-20,height-536),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-536-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
							
										image_d.at<Vec3b>(height-536-y_2,x_2) = image.at<Vec3b>(height-40-536-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 5:rectangle(image,Point(20,height-615),Point(width-20,height-576),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-576-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
										
										image_d.at<Vec3b>(height-576-y_1,x_1) = image.at<Vec3b>(height-40-576-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 4:rectangle(image,Point(20,height-655),Point(width-20,height-616),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-616-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
									
										image_d.at<Vec3b>(height-616-y_2,x_2) = image.at<Vec3b>(height-40-616-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 3:rectangle(image,Point(20,height-695),Point(width-20,height-656),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-656-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
										
										image_d.at<Vec3b>(height-656-y_1,x_1) = image.at<Vec3b>(height-40-656-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 2:rectangle(image,Point(20,height-735),Point(width-20,height-696),Scalar(0,0,0),-1);
							for(int y_2=0;y_2<=height-696-51;y_2++){
								for(int x_2=20;x_2<=width-20;x_2++){
									
										
										image_d.at<Vec3b>(height-696-y_2,x_2) = image.at<Vec3b>(height-40-696-y_2,x_2);									
									
									
								}
							}
							count = 0;
							break;

					case 1:rectangle(image,Point(20,height-775),Point(width-20,height-736),Scalar(0,0,0),-1);
							for(int y_1=0;y_1<=height-736-51;y_1++){
								for(int x_1=20;x_1<=width-20;x_1++){
									
									
										image_d.at<Vec3b>(height-736-y_1,x_1) = image.at<Vec3b>(height-40-736-y_1,x_1);									
									
									
								}
							}
							count = 0;
							break;
					
					case 0:	rectangle(image,Point(20,height-815),Point(width-20,height-776),Scalar(0,0,0),-1);
							count = 0;
							break;
							

							
						
					
							
							

					}
					image_d.copyTo(image);
				
				}
				
				
			}
			
		}

		gravity = 0;
		collision = false;
	
			cube= false;
			I= false;
			L= false;
			S= false;
		
	}
	gravity = gravity + 5;


	//Control

	if(key == 27){ //Escape
			break;
		}
		
	if(cube){
		if(key == 2555904 && move+start2_1+40 < width-20 && !r_collision ){ //ArrowRight
			move = move +40;
			
		}
		if(key == 2424832 && move + start1_1 > 20 && !l_collision){ //ArrowLeft
			move = move -40;
			
		}
		if(key == 2490368){ //Arrow-Up (Rotate)
			rotate++;
			if(rotate > 3)
				rotate = 0;
		}
	}

	if(I){
		if(key == 2555904 && move+start2_1 < width-20 && !r_collision && (rotate == 0 || rotate == 2)){ //ArrowRight
			move = move +40;
			
		}

		if(key == 2555904 && move+start2_1+100 < width-20 && !r_collision && (rotate == 1 || rotate == 3)){ //ArrowRight rotated
			move = move +40;
		}

		if(key == 2424832 && move + start1_1 > 20 && !l_collision) { //ArrowLeft
			move = move -40;
		}
					
		
		if(key == 2490368){ //Arrow-Up (Rotate)
			rotate++;
			if(rotate > 3)
				rotate = 0;
		}
	}

	
			
	
		
	
	image = 0 + image_d;
	
	

	
	rectangle(image,Point(0,0),Point(width,height),Scalar(128,0,0),20);

	}
	

		
}
	
	




int main(){ 	
	Tetris();
}


