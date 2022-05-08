/***********OBJECT MEASURMENT***********/
/* This program is written to calculate*/
/* the dimensions of various objects.  */
/***************************************/




//The header files required for the    
//functions used are given below      

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <math.h>

using namespace std;
using namespace cv;

/*********************This is the main part of the program**************************/
/*The following function is responsible for getting image input from vision system,*/
/*perform image filter operations, shape detection and finally measurment operation*/
/***********************************************************************************/

int main(int argc, char** argv)
{
	// Create an object for video capturing
	VideoCapture cap;

	// Give access of webcam to the video object and give it scaling constraints
	cap.open(1);
	cap.set(3, 1920);
	cap.set(4, 1080);

	// Initialise all the necessary variable for the program to work
	Mat img, img1;
	float area, peri;
	double valuex, valuey, fillet;
	int frames = 1000;
	int key;

	// Introduction about the program for the user
	cout << "*********************WELCOME*************************\n";
	cout << "This system measures the dimensions of the obejcts\n\n\n ";
	cout << "Please enter the dimensions of the platform used for measurement\n";

	cout << "NOTE: Make sure these measurements are accurate since they will be used as reference\n";
	cout << "Enter the width of your region of interest: ";
	cin >> valuex;
	// This value is used as reference in scaling of pixels in x-direction
	cout << "Enter the height of your region of interest: ";
	cin >> valuey;
	// This value is used as reference in scaling of pixels in y-direction
	cout << "Do you want to find fillet radius (1 if yes/ 0 if no)";
	cin >> fillet;
	//check with user if conditions are met for fillet area
		
	// checking of the videocapture object created is working and connected to webcam
	if (!cap.isOpened())
	{
		std::cout << "error: could not grab a frame" << std::endl;
		exit(0);
		// Error meassage is sent and program is closed if connection is absent
	}
	

	for (int i = 0; i < frames; i++)
	{
		// Image is captured and stored in a Matrix
		cap >> img;
		img1 = img;
		
		// The Following steps performs image filtering operations for feature extraction

		cvtColor(img, img, COLOR_BGR2GRAY); //color conversion from bgr to greyscale
		GaussianBlur(img, img, Size(5, 5), 1);  //blurring to remove noise from the image
		Canny(img, img, 100, 100);  //edge detection to identify the features
		dilate(img, img, Mat(), Point(-1, 1), 2); //used to create closed figures from features
		erode(img, img, Mat(), Point(-1, 1), 2);  //erode the extra thickness
		
		imshow("Image", img);

		key = waitKey(50);
		if (key == 'c') //An hotkey is provided to quit the program anytime the user wants to
		{
			exit(0);
		}

		//Initialising variable to identify the platform region
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;		
		int max_index{}, maxarea = 0;
		float scale_x,scale_y;
		
		//finds all the contours in the image and returns them along with heirarchy
		findContours(img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		
		//Condition to go to next frame when no contours are detected
		if (contours.size() == 0)
			continue;
		
		//This part loops through all the contours in the image and gives the index of maxmimum area contour
		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]); //finds the area of contour
			
			if (area > maxarea) //compares the area with previous contours
			{
				max_index = i; //stores the index of contour with max area
				maxarea = area;
			}
		}
		
		//The properties of platform is obtained in this part
		RotatedRect minRect_max = minAreaRect(contours[max_index]);	//finds the minimum area rectangle that covers the contour

		//the ppi value of the current frame is updated to the system
		scale_x = valuex / minRect_max.size.width; //x-scaling of the image
		scale_y = valuey / minRect_max.size.height;//y-scaling of the image
		
		//Initialising variables to find the shape of the contours detected
		vector<vector<Point> > conpoly(contours.size());
		vector<Rect> boundRect(contours.size());
		int pts;
		
		//This loop is dedicated to find the shape and dimension of the objects
		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]); //calculates the contour area

			// contours may be detected from noises which needs to be eliminated before proceeding
			if (area > 1000) //Only significant contours are considered after this
			{
				float peri = arcLength(contours[i], true); //calculates the perimeter of the contour
				approxPolyDP(contours[i], conpoly[i], 0.02 * peri, true); //approximates the contours to the shape of standard polygon
				pts = conpoly[i].size(); //determine the no of points required for approximation
				

				if (pts == 3) //Traingle objects can be detected with only 3 points
				{
					// displays the lines that make up the triangle along with their dimensions
					for (int j = 0; j < pts-1; j++)
					{
						line(img1, conpoly[i][j], conpoly[i][(j + 1) % 4], Scalar(0, 255, 0), 2); //function to draw a line 
						double res = scale_x*cv::norm(conpoly[i][j]-conpoly[i][(j + 1) % 4]); // calculates the distance between 2 points
						putText(img1,to_string(int(res)), Point(conpoly[i][j].x, conpoly[i][j].y+20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1); //enters text in image matrix
					}
				}

					
				else if (pts == 4) //Rectangle or Square can be detected with 4 points
				{
					RotatedRect minRect = minAreaRect(contours[i]); //detectes the minimum area rectangle for the contour
					Point2f rect_points[4];
					minRect.points(rect_points); //extracts the points that make up the rectangle

					// the following lines calculates the dimensions of object in pixel value and converts it into actual dimensions
					float width = minRect.size.width; //calculates the width of bounding rectangle
					float height = minRect.size.height; //calculates the height of bounding rectangle
					float real_width = scale_x * width; //actual width in millimeters
					float real_height = scale_y * height; //actual height in millimeters
					

					string r_width = to_string(int(real_width));
					string r_height = to_string(int(real_height));

					string part1 = "(";
					string part2 = "mm,";
					string part3 = "mm)";
					

					string final = part1 + r_width + part2 + r_height + part3; //combining all the information to display
					putText(img1, final, Point(rect_points[0].x, rect_points[0].y +20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
					putText(img1, "Quadilateral", Point(rect_points[0].x, rect_points[0].y + 40), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
					
					// this loops prints the points as lines that make up the rectangle
					for (int j = 0; j < pts; j++)
					{
						line(img1, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 255, 0), 2);
						
					}

					if (fillet == 1)
					{
						//Radii caluclation of the fillet
						Point center = minRect.center; //center point of the rectangle
						float fillet_radius, area_diff;
						area_diff = (real_width * real_height) - (area * scale_x * scale_y); //area difference between shape and rectangle that defines it
						fillet_radius = sqrt(area_diff / 0.8584); //fillet radius
						putText(img1, to_string(fillet_radius), center, FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1); //prints radii in center point
					}
					

				}

				else if (pts > 6) //For circle and higher order polygons
				{
					//get the rectangle that bounds the object and print them
					boundRect[i] = boundingRect(conpoly[i]);
					rectangle(img1, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);

					//Calculate the radius of the circle
					double width = scale_x*cv::norm(boundRect[i].tl()-boundRect[i].br());
					int radius = width / sqrt(8); //calculating the radius in actual values
					string name = "radius:";
					string final = name + to_string(radius);
					putText(img1, final, Point(boundRect[i].tl().x, boundRect[i].tl().y-20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);

				}
								
			}
		}
		imshow("New image", img1); //This displays the image with the dimensions
			
			
	}
}
