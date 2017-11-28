#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std; 

int main()
{
	FILE* outfil; 
	outfil = fopen("vgapalette.cpp", "w"); 

	if (!outfil) 
	{
		cout << "Failed to open output file" << endl; 
		return -1; 
	}

	cv::Mat image = cv::imread("src/vgapalette.png");
	if (!(image.cols > 0 && image.rows > 0))
	{
		cout << "Failed to open vgapallete input file (src/vgapalette.png)" << endl; 
		return -1; 
	}

	// Information about vgapallet.jpg
	// Cols: 480
	// Colors per Col: 16
	// Pixels between each color: 2
	// Pixels between each color per col: (16) * 2 = 32 
	// Cols of color per row: 480 - pixels between each color per col: 480 - 32: 448 
	// Width of color: cols of color per row / 16: 448 / 16: 28 
	
	fprintf(outfil, "cv::Vec3b vga_colors[] = {\n");
	int j = 0; 
	for (int r = 2; r <= image.rows -30; r += 30)
	{
		fprintf(outfil, "// row: %d\n", j); 
		for (int c = 2; c <= image.cols-30; c += 30)
		{
			cv::Vec3b pix = image.at<cv::Vec3b>(r, c);
			fprintf(outfil, "cv::Vec3b(0x%02x, 0x%02x, 0x%02x), \n", pix[0], pix[1], pix[2]);  
		}
		fprintf(outfil, "\n"); 
		j++; 
	}	
	fprintf(outfil, "};"); 		

	cout << "Wrote to vgapalette.cpp!" << endl;
}
