/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* MovieLab.c: source file for the main function                     */
/*                                                                   */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height);

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
    

	unsigned int width = 480;
	unsigned int height = 270;
    
    /* Crop Image Variables */
    unsigned int start = 0;
    unsigned int end = 0;

    /* Fast Image Variables */
    int numFrames = 0;

    unsigned int factor = 0;
    
    IMAGE *watermark = NULL;

    MOVIE *movie = NULL;

	/* the while loop to check options in the command line */
	while (x < argc && x < 9) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
            } /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
            x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
            } /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

        if (strcmp(argv[x], "-s") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[x + 1], "%ux%u", &width, &height) == 2) {
                    /* input is correct */
                    /* the image width is stored in width */
                   /* the image height is stored in height */
                } else {
                    /* input format error */
                    printf("Input format error.\n");
                }
            }
            else {
                printf("Missing argument for the frame size!\n");
                return 5;
            }
            x += 2;
            continue;
        }

        if (strcmp(argv[x], "-f") == 0) {
            if (x < argc - 1) {
                char *num = argv[x + 1];
                numFrames = atoi( num);
            }
            else {
                printf("Missing argument for number of frames!\n");
                return 5;
            }
            x += 2;
            continue;
            
        }
        if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
        } 
		x++;
	} /*elihw*/


	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
        return 5;
	}
    else if (movie == NULL) {
            movie = LoadMovie(fin, numFrames, width, height);
            //continue;
        }
    while (x < argc) {
        if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			x = argc;
            continue;
            return 0;
        } 
        if (strcmp(argv[x], "-aging") == 0) {
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            while (current != NULL) {
                Aging(current->RGBImage);
                current = current->Next;
            }
            RGB2YUVMovie(movie);
            printf("Operation Aging is done!\n");

            x += 1;
            continue;
        }
        if (strcmp(argv[x], "-hflip") == 0) {
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            while (current != NULL) {
                HFlip(current->RGBImage);
                current = current->Next;
            }
            RGB2YUVMovie(movie);
            printf("Operation HFlip is done!\n");
            x += 1;
            continue;
        }
        if (strcmp(argv[x], "-edge") == 0)  {
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            while (current != NULL) {
                Edge(current->RGBImage);
                current = current->Next;
            }
            RGB2YUVMovie(movie);
            printf("Operation Edge is done!\n");
            x += 1;
            continue;
        }
        
        if (strcmp(argv[x], "-crop") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[x + 1], "%u-%u", &start, &end) == 2) {
                    /* input is correct */
                    /* the image width is stored in width */
                    /* the image height is stored in height */
                } else {
                    /* input format error */
                    printf("Input format error.\n");
                }
            }
            else {
                printf("Missing argument for the crop start and end!\n");
                return 5;
            }
            CropImageList(movie->Frames, start, end);
            /* Print inside function */
            x += 2;
            continue;
        }
        
        if (strcmp(argv[x], "-fast") == 0) {
            if (x < argc - 1) {
                factor = atoi(argv[x + 1]);
            }
            else {
                printf("Missing argument for fast factor!\n");
                return 5;
            }
            FastImageList(movie->Frames, factor);
            /* Print inside function */
            x += 2;
            continue;
        }
        
        if (strcmp(argv[x], "-reverse") == 0) {
            ReverseImageList(movie->Frames);
            printf("Operation Reverse is done!\n");
            x += 1;
            continue;
        }
        
        if (strcmp(argv[x], "-watermark") == 0) {
            if (x < argc - 1) {
                watermark = LoadImage(argv[x + 1]);
                assert(watermark);
            }
            else {
                printf("Missing argument for watermark!\n");
            }
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            int counter = 15;
            int rand_x = 0;
            int y = 0;
            srand(time(NULL));
            while (current != NULL) {
                if (counter == 15) { 
                    rand_x = rand() % width;
                    y = rand() % height;
                    counter = 0;
                }   
                Watermark(current->RGBImage, watermark, rand_x, y);
                current = current->Next;
                counter += 1;
            }
            DeleteImage(watermark);
            watermark = NULL;
            RGB2YUVMovie(movie);
            printf("Operation Watermark is done!\n");
            x += 2;
            continue;
        }
        
        if (strcmp(argv[x], "-spotlight") == 0) {
            if (movie->Frames->Length < 60) {
                printf("Error! Minimum 60 frames required for Spotlight Operation!\n");
                x += 1;
                continue;
            }
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            int firstframes = 0;
            int lastframes = movie->Frames->Length - 20;
            int counter = 0;
            double radius = 0;
            int center_x = width / 2;
            int center_y = height / 2;
            int maxRadius = sqrt(center_x * center_x + center_y * center_y);
            while (current != NULL) {
                Spotlight(current->RGBImage, center_x, center_y, (int)radius);
                if (firstframes < 40) {
                    radius += 7.06;
                    firstframes += 1;
                }
                if (counter == 40) {
                    radius = maxRadius;
                }
                if (counter >= lastframes) {
                    radius -= 14.49;
                }
                counter += 1;
                current = current->Next;
            }
            RGB2YUVMovie(movie);
            printf("Operation Spotlight is done!\n");
            x += 1;
            continue;
        }
        
        if (strcmp(argv[x], "-zoom") == 0) {
            YUV2RGBMovie(movie);
            IENTRY *current = movie->Frames->First;
            int percentage = 0;
            while (current != NULL) {
                if (percentage < 100) {
                    Zoom(current->RGBImage, percentage);
                }
                else if (percentage >= 100 && percentage <= 200) {
                    Zoom(current->RGBImage,200 -  percentage);
                }   
                if (percentage == 200) {
                    percentage = 0;
                }
                percentage += 2;
                current = current->Next;
            }
            RGB2YUVMovie(movie);
            printf("Operation Zoom is done!\n");
            x += 1;
            continue;
        }
        x++;
    }
    
    if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}
    else {
        SaveMovie(fout, movie);
        
    }
    
    if (watermark != NULL) {
        DeleteImage(watermark);
        watermark = NULL;
    }

    DeleteMovie(movie);
    movie = NULL; 
	fin = NULL;
	fout = NULL;

	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WIDTHxHEIGHT> [options]\n"
	       "Options:\n"
	       "-aging                Activate the aging filter on every movie frame\n"
	       "-hflip                Activate horizontal flip on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-zoom                 Zoom in and out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVIMAGE* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height)
{
	/* to be implemented */
    MOVIE *mov = CreateMovie();;
    ILIST *list = CreateImageList();
    YUVIMAGE *yuvframe = NULL;
    for (int i = 0; i < frameNum; ++i) {
        yuvframe = LoadOneFrame(fname, i, width, height);
        AppendYUVImage(list, yuvframe);
    }
    mov->Frames = list;
    printf("The movie file %s has been read successfully!\n", fname);
    return mov;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
    assert(movie);
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->YUVImage, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
