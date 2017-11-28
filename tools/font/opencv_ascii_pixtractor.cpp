#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void analyze_char(Mat roi, char list)
{
	if (!list)
	imshow("alalyzing", roi);

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
			if (!list) 
				cout << " ";
			cout << (roi.at<unsigned char>(x, y) > 200? h : '1');
		}
		if (!list)
			cout << endl;
	}
	if (list)
	{
		cout << "0000000,\n"; 
	}
	else
	{
		cout << endl;
		waitKey(0);	
	}
}
int main()
{
	Mat file = imread("ascii_symbols.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	for (int x = 0; x < file.cols; x+=6)
	{
		// CHANGE THE LAST ARGUMENT TO 0 FOR VISUAL OUTPUT
		analyze_char(file(Rect(x, 0, 5, 5)), 1);
	}

}
