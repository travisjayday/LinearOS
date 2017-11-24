#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
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
	uint8_t* raw_img = NULL;  
	size_t image_size;

	// loop through each argument
	for (int i = 1; i < argc; i++)
	{
		if (memcmp(argv[i] + strlen(argv[i]) - 4, ".png", 4) == 0)
		{
			fname = argv[i]; 
			transparent = true; 
			image = cv::imread(fname, cv::IMREAD_UNCHANGED); 
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


	// check if image loaded
	if (image.cols > 320 || image.rows > 200)
		panic("Image dimensions/size too large!");
	else if (image.cols == 0 || image.rows == 0)
		panic("Failed to read image!"); 


	// handle transparent alpha mat
	if (transparent)
	{
		// extract alhpa channel, store it in 'alpha' and remerge the bgr channels
		vector<cv::Mat> channels; 
		cv::split(image, channels); 	
		alpha = channels[3]; 
		channels.pop_back(); 
		cv::merge(channels, image); 
	}

	

	// output config variables
	cout << "Processing Image: " << fname << endl; 
	cout << "Transparency: " << (transparent? "ON" : "OFF") <<  endl; 
	cout << "Blur: " << blur << endl; 

	if (outname) 
		cout << "Writing image to " << outname << endl; 
	else
		cout << "Warning: NOT writing image to file" << endl; 
	
	
	// start gui
	cv::namedWindow("input", cv::WINDOW_NORMAL); 
	cv::namedWindow("output", cv::WINDOW_NORMAL); 
	cv::resizeWindow("input", 640, 480); 
	cv::resizeWindow("output", 640, 480); 


	// blur image and show input
	cv::Mat tmp = image.clone(); 
	cv::imshow("input", image); 
	printf("Pre-Processing... (this may take a while)\n"); 
	if (blur > 0)
	//	cv::GaussianBlur(image, image, cv::Size(blur, blur), 0, 0); 
	//	cv::blur(image, image, cv::Size(blur, blur)); 
	//	cv::medianBlur(image, image, (int)blur); 
		cv::bilateralFilter(tmp, image, -1, blur, blur); 

	// start image processing; convert true colors to VGA 8 bit colors
	printf("Converting Image to 256 bit VGA compatible colors... \n"); 

	image_size = image.cols * image.rows; 
	raw_img = (uint8_t*) malloc(image_size); 

	int i = 0; 
	int t = image_size; 
	for (int r = 0; r < image.rows; r++)
	{
		for (int c = 0; c < image.cols; c++)
		{
			// get nearest color and apply it to new image
			cv::Vec3b pix = image.at<cv::Vec3b>(r, c);  
			unsigned char idx = nearest_color(pix); 

			if (transparent && alpha.at<uchar>(r, c) < 10)
			{
				pix = cv::Vec3b(255, 0, 255);	// transparent color (for gui) 
				idx = 0xfa; 					// color code for 'transparent pixel' 
			}
			image.at<cv::Vec3b>(r, c) = pix; 

			*(raw_img + i) = idx; 

			// update gui
			printf("%d%%\r", (int)((float)(i+1) / (float)t * 100.0f)); 
			i++; 
		}	
	}

	// at this point the array raw_img is populated with rows * cols vga bytes 
	printf("\nEncoding image in RLE (runtime-length-encoding)...\n"); 
	fflush(stdout); 

	// allocate more than enough space for encoded array. Worst case scenario image is noise and each pixel requires 3 bytes
	uint8_t* encoded_img = (uint8_t*) malloc(image_size * 3); 
	
	// 0xFF = next byte determines number to put bytes 
	// 0xFA = next byte determines number to skip bytes (for transparent image) 

	int e = 0; 
	i = 0; 
	while (i < image_size)
	{
		uint8_t inbyte = *(raw_img + i); 
		
		// j is number of repeated bytes 
		int j;
		for (j = 0; *(raw_img + i + j) == inbyte && j < 0xFF - 0x8; j++); 
		
		// if transparent, just put 0xfa (skip byte)
		if (transparent && inbyte == 0xfa)
		{
			*(encoded_img + e) = 0xfa; 			// command : skip byte
			*(encoded_img + e + 1) = j;			// number : repeated times	
			e += 2; 
		}
		// else, put 0xff (put byte), and inbyte (the color byte)
		else 	
		{
			*(encoded_img + e) = 0xff; 			// command : put byte
			*(encoded_img + e + 1) = j;			// number : repeated times	
			*(encoded_img + e + 2) = inbyte; 	// data	  : value of color byte	
			e += 3; 
		}
		i += j; 
	}
	printf("Successfully Encoded Image!\n"); 
	printf("Image Size (no encoding) : %ld bytes\n", image_size); 
	printf("RLE Encoded Image Size   : %d bytes\n", e);  
	
	if (e > image_size) 
		printf("Choosing NO encoding...\n");
	else
		printf("Choosing RLE Encoding...\n");
	
	fflush(stdout); 


	// handle output file
	if (outname) 
	{
		// open output file
		outfile = fopen(outname, "w"); 
		
		if (outfile == NULL) 
			panic("Error Writing: Cannot open output file"); 

		printf("Writing Encoded Image to file...\n"); 
		fflush(stdout); 

		// prepare image array (remove extension) 
		//*(fname + strlen(outname) -4) = '\0'; 
		fprintf(outfile, "static uint8_t %s[] = {\n", outname); 
		
		// print header bytes
		// print rows, columns (low byte), columns (high byte), transparency is most significant bit of high byte of columns
		fprintf(outfile, "0x%02x, // rows \n\
0x%02x, // cols (low byte) \n\
0x%02x, // cols (high byte); most significant bit dictates if sprite is encoded with RLE \n\
0x%02x, // size of this array (elements) (low byte) \n\
0x%02x, // size of this array (elements) (high byte) \n", 
				image.rows, 
				(uchar) image.cols,  
				(uchar) e > image_size? image.cols >> 8 | 1 << 7 : image.cols >> 8,
				(uchar) e,
				(uchar) (e >> 8)
			  ); 

		// encoded image is smaller than raw image
		if (e < image_size) 
		{
			int i = 0; 
			while (1)
			{
				uint8_t cmd_b = *(encoded_img + i); 
				// put byte
				if (cmd_b == 0xff) 
				{
					fprintf(outfile, "0xff, 0x%02x, 0x%02x, \n", *(encoded_img + i + 1), *(encoded_img+ i + 2)); 
					i += 3; 
				}
				// skip byte
				else if (cmd_b == 0xfa)
				{
					fprintf(outfile, "0xfa, 0x%02x, \n",  *(encoded_img + i + 1)); 
					i += 2; 
				}
				// terminal byte
				else
					break;
			}	 
		}
		else 
		{
			for (int i = 0; i < image_size; i++)
				fprintf(outfile, "%d,\n", *(raw_img + i)); 
		}
/*
	int i = 0; 
		while (i < e)
		{
			fprintf(outfile, "0x%02x,\n", *(encoded_img + i)); 
			i++; 
		}*/


		// magic finishing byte
		fprintf(outfile, "0xfc // magic finishing signature\n};"); 
		fflush(outfile); 
		
		fclose(outfile); 

		printf("Successfully wrote image to %s\n", outname); 
		fflush(stdout); 
	}

	// finished 

	while (1)
	{
		cv::imshow("output", image); 
		if ((char)cv::waitKey(30) == 'q')
			break;
	}
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

