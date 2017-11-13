//#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void analyze_char(Mat roi, char list)
{
	imshow("alalyzing", roi);
	resize(roi, roi, Size(5, 5));

	char h = '.';
	if (list)
	{
		h = '0';	
		cout << "0b";
	}
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			cout << (roi.at<unsigned char>(x, y) > 200? h : '1');
		}
		if (!list)
			cout << endl;
	}
	if (list)
	{
		cout << "0000000,\n"; 
	}
	waitKey(0);	
}

int main()
{
	Mat file = imread("5x5_font.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2 = file.clone(); 

	int startx; 	// index of first black pixel
	int endx;	// index of first white pixel 
	int spacing;	// space between chars

	for (startx = 0; file.at<unsigned char>(0, startx) > 200; startx++)
	{}
	for (endx = startx; file.at<unsigned char>(0, endx) < 200; endx++)
	{}
	for (spacing = endx; file.at<unsigned char>(0, spacing) > 200; spacing++)
	{}
	spacing -= endx; 

	line(img2, Point(startx, 10), Point(startx, 500), Scalar(0, 0, 0), 1);	
	line(img2, Point(endx, 10), Point(endx, 500), Scalar(0, 0, 0), 1);	
	line(img2, Point(endx, 10), Point(spacing, 10), Scalar(0, 0, 0), 1);	

	
	const int twidth = (endx - startx);
	const int cwidth = (int)(twidth / 5.0 + 0.5); 
	cout << "endx-startx is " << endx - startx << " Cell width is " << cwidth << " pixels"; 
	cout << "Spacing is " << spacing << endl;

	for (int x = startx; x < file.cols - twidth; x += twidth + spacing)
	{
		analyze_char(file(Rect(x, 0, twidth, twidth)).clone(), 1);
	}

	int r2;
	for (r2 = twidth; file.at<unsigned char>(r2, 6) > 200; r2++)
	{}
	
	for (startx = 0; file.at<unsigned char>(r2, startx) > 200; startx++)
	{}

	for (int x = startx; x < file.cols - twidth; x += twidth + spacing)
	{
		analyze_char(file(Rect(x, r2, twidth, twidth)).clone(), 1);
	}


	imshow("win", img2); 
	waitKey(0);


	return 0;
}
