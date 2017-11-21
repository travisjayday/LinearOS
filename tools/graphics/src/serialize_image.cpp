#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include "vgapalette.cpp"	// cv::Vec3b vga_color[] = {...}; 

using namespace std; 

void panic(string msg); 
char nearest_color(cv::Vec3b& color);
float vec3b_difference(cv::Vec3b c1, cv::Vec3b c2); 

int main(int argc, char* argv[])
{
	char* fname = NULL; 
	char* outname = NULL; 
	FILE* outfile = NULL; 
	bool transparent = false; 
	float blur = 20; 
	cv::Mat image;
	cv::Mat alpha; 

	// loop through each argument
	for (int i = 1; i < argc; i++)
	{
		if (memcmp(argv[i] + strlen(argv[i]) - 4, ".png", 4) == 0)
		{
			fname = argv[i]; 
			transparent = true; 
			image = cv::imread(fname, cv::IMREAD_UNCHANGED); 

			// extract alhpa channel, store it in 'alpha' and remerge the bgr channels
			vector<cv::Mat> channels; 
			cv::split(image, channels); 	
			alpha = channels[3]; 
			channels.pop_back(); 
			cv::merge(channels, image); 
		}	
		else if (memcmp(argv[i] + strlen(argv[i]) - 4, ".jpg", 4) == 0)
		{
			fname = argv[i]; 
			image = cv::imread(fname); 
		}
		// given that arg starts with '-'
		else if (argv[i][0] == '-') 
		{
			switch (argv[i][1]) 
			{
				case 'b': 
					blur = atof(argv[i+1]); 
					break;
				case 'o':
					outname = argv[i+1]; 
					break;
				default:
					panic("Uknown Argument!"); 

			}
		}
	}
	
	// no filename given
	if (fname == NULL) 
		panic("Please provide an image filename as an argument!"); 	

	cout << "Processing Image: " << fname << endl; 
	cout << "Transparency: " << (transparent? "ON" : "OFF") <<  endl; 
	cout << "Blur: " << blur << endl; 

	if (outname) 
		cout << "Writing image to " << outname << endl; 
	else
		cout << "Warning: NOT writing image to file" << endl; 
	
	cv::namedWindow("input", cv::WINDOW_NORMAL); 
	cv::namedWindow("output", cv::WINDOW_NORMAL); 
	cv::resizeWindow("input", 640, 480); 
	cv::resizeWindow("output", 640, 480); 
	
	if (image.cols > 320 || image.rows > 200)
		panic("Image dimensions/size too large!");
	else if (image.cols == 0 || image.rows == 0)
		panic("Failed to read image!"); 

	if (outname) 
	{
		// open output file
		outfile = fopen(outname, "w"); 

		// prepare image array (remove extension) 
		*(fname + strlen(outname) -4) = '\0'; 
		fprintf(outfile, "uint8_t %s = {\n", outname); 
		
		// print header bytes
		// print rows, columns (low byte), columns (high byte), transparency is most significant bit of high byte of columns
		fprintf(outfile, "0x%02x, // rows \n\
0x%02x, // cols (low byte) \n\
0x%02x, // cols (high byte); most significant bit dictates of transparency is on/off \n", 
				image.rows, 
				(char) image.cols,  
				(char) transparent? image.cols >> 4 | 1 << 7 : image.cols >> 4
			  ); 
	}

	// blur image and show input
	cv::Mat tmp = image.clone(); 
	cv::imshow("input", image); 
	printf("Pre-Processing... (this may take a while)\n"); 
	if (blur > 0)
	//	cv::GaussianBlur(image, image, cv::Size(blur, blur), 0, 0); 
	//	cv::blur(image, image, cv::Size(blur, blur)); 
	//	cv::medianBlur(image, image, (int)blur); 
		cv::bilateralFilter(tmp, image, -1, blur, blur); 
	printf("Processing... \n"); 

	int i = 0; 
	int t = image.rows * image.cols; 
	for (int r = 0; r < image.rows; r++)
	{
		for (int c = 0; c < image.cols; c++)
		{
			// get nearest color and apply it to new image
			cv::Vec3b pix = image.at<cv::Vec3b>(r, c);  
			unsigned char idx = nearest_color(pix); 
			image.at<cv::Vec3b>(r, c) = pix; 

			if (outname)
				// output pixel data, BGR->RGB
				fprintf(outfile, "0x%02x,\n", idx); 
			// update gui
			printf("%d%%\r", (int)((float)(i+1) / (float)t * 100.0f)); 
			i++; 
		}	
	}
	if (outname) 
		// magic finishing byte
		fprintf(outfile, "0xfe // magic finishing signature\n};"); 

	cv::imshow("output", image); 
	cv::waitKey(0); 
//	printf("};\n"); 

	return 0; 
}

void panic(string msg)
{
	cout << msg << endl;
	exit(-1); 	
}

char nearest_color(cv::Vec3b& color)
{
	float smallest_diff = INT_MAX; 
	int smallest_diff_idx = 0; 
	for (int i = 0; i < 255; i++)
	{
		float d = vec3b_difference(vga_colors[i], color); 
		if (d < smallest_diff)
		{
			smallest_diff = d; 
			smallest_diff_idx = i; 
		}
	}	
	color = vga_colors[smallest_diff_idx]; 
	return smallest_diff_idx; 
}

float vec3b_difference(cv::Vec3b c1, cv::Vec3b c2)
{
	// cartesian distance in R3
	return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2)); 
}
