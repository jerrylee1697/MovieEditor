/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* Movie.c: source file for basic movie manipulations                */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
MOVIE *CreateMovie(void)
{
	/* to be implemented */
    MOVIE *mov = NULL;
    mov = malloc(sizeof(MOVIE));
    return mov;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
	/* to be implemented */
    assert(movie);
    ILIST *list = movie->Frames;
    DeleteImageList(list);
    free(movie);
    movie = NULL;
}

int clip(int x) {
    if (0 <= x && x <= 255) {
        return x;
    }
    else if (x < 0) {
        return 0;
    }
    else if (x > 255) {
        return 255;
    }
    return 0;
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(MOVIE *movie)
{
    ILIST *list = movie->Frames;
    IENTRY *frame = list->First;
    int width = frame->YUVImage->W;
    int height = frame->YUVImage->H;
    YUVIMAGE *yuvimage = NULL;
    while (frame != NULL) {
        IMAGE *image = CreateImage(width, height);
        yuvimage = frame->YUVImage;
        assert(yuvimage);
        for (unsigned i = 0; i < width; ++i) {
            for (unsigned j = 0; j < height; ++j) {
                int C = GetPixelY(yuvimage, i, j) - 16;
                int D = GetPixelU(yuvimage, i, j) - 128;
                int E = GetPixelV(yuvimage, i, j) - 128;
                SetPixelR(image, i, j, clip(( 298 * C           + 409 * E + 128) >> 8));
                SetPixelG(image, i, j, clip(( 298 * C - 100 * D - 208 * E + 128) >> 8));
                SetPixelB(image, i, j, clip(( 298 * C + 516 * D           + 128) >> 8));
            }
        }
        frame->RGBImage = image;
        DeleteYUVImage(frame->YUVImage);
        frame->YUVImage = NULL;
        frame = frame->Next;   
    }
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(MOVIE *movie)
{
	ILIST *list = movie->Frames;
    IENTRY *frame = list->First;
    int width = frame->RGBImage->W;
    int height = frame->RGBImage->H;
    IMAGE *image = NULL;    
    while(frame != NULL) {
        YUVIMAGE *yuvimage = CreateYUVImage(width, height);
        image = frame->RGBImage;
        assert(image);
        for (unsigned i = 0; i < width; ++i) {
            for (unsigned j = 0; j < height; ++j) {
                int R = GetPixelR(image, i, j);
                int G = GetPixelG(image, i, j);
                int B = GetPixelB(image, i, j);
                SetPixelY(yuvimage, i, j, clip( ( ( 66  * R + 129 * G + 25 * B + 128) >> 8) + 16 ));
                SetPixelU(yuvimage, i, j, clip( ( ( -38 * R - 74 * G + 112 * B + 128) >> 8) + 128 ));
                SetPixelV(yuvimage, i, j, clip( ( ( 112 * R - 94 * G - 18  * B + 128) >> 8) + 128 ));
            }
        }
        frame->YUVImage = yuvimage;
        DeleteImage(frame->RGBImage);
        frame->RGBImage = NULL;
        frame = frame->Next;
    }
}

/* EOF */
