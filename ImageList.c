/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* ImageList.c: source file for image list manipulations             */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ImageList.h"

/* Create a new Image Entry */
static IENTRY *NewImageEntry(IMAGE *image, YUVIMAGE *YUV) {
    IENTRY *e;
    e = malloc(sizeof(IENTRY));
    if (!e) {
        perror("Out of memory! Aborting...");
        exit(10);
    }
    e->List = NULL;
    e->Next = NULL;
    e->Prev = NULL;
    e->RGBImage = image;
    e->YUVImage = YUV;
    return e;
}

static void *DeleteImageEntry(IENTRY *entry) {
    assert(entry);
    entry->List->Length -= 1;
    IENTRY *next = entry->Next;
    // Connect Previous and Next
    if (next != NULL) {
        next->Prev = entry->Prev; 
    }
    if (entry->Prev != NULL) {
        entry->Prev->Next = entry->Next;    
    }
    // Either YUV or RGB image must be NULL at all times so only one to delete
    if (entry->RGBImage != NULL) {
        DeleteImage(entry->RGBImage);
        entry->RGBImage = NULL;
    }
    if (entry->YUVImage != NULL) {
        DeleteYUVImage(entry->YUVImage);
        entry->YUVImage = NULL;
    }
    free(entry);
    entry = NULL;
    return 0;
}

/* Create a new image list */
ILIST *CreateImageList(void)
{
	ILIST *list = NULL;
    list = malloc(sizeof(ILIST));
    if (!list) {
        perror("Out of memory. Aborting ...");
        exit(10);
    }
    list->Length = 0;
    list->First = NULL;
    list->Last = NULL;
    return list;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
	assert(list);
    IENTRY *e, *n;

    e = list->First;
    while(e != NULL) {
        n = e->Next;
        DeleteImageEntry(e);
        e = n;
    }
    free(list);
    list = NULL;
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, IMAGE *RGBimage)
{
    assert(list);
    assert(RGBimage);

    list->Length += 1;
    
    IENTRY *m;
    IENTRY *k;
    
    k = NewImageEntry(RGBimage, NULL);
    k->List = list;
    // Check if Empty List
    if (list->First == NULL) {
        list->First = k;
    }
    if (list->Last == NULL) {
        list->Last = k;
    }
    else {
        m = list->Last;
        m->Next = k;
        list->Last = k;
        k->Prev = m;
    }
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVIMAGE *YUVimage)
{
	list->Length += 1;
    
    IENTRY *m;
    IENTRY *k;
    k = NewImageEntry(NULL, YUVimage);
    k->List = list;
    if (list->First == NULL) {
        list->First = k;
    }
    if (list->Last == NULL) {
        list->Last = k;
    }
    else {
        m = list->Last;
        m->Next = k;
        list->Last = k;
        k->Prev = m;
    }
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
	IENTRY *currentframe = list->First;
    IENTRY *next = NULL;
    int counter = 0;
    while (currentframe != NULL) {
        next = currentframe->Next;
        if (counter < start) {
            DeleteImageEntry(currentframe);
            currentframe = NULL;
        }
        if (counter == start) {
            list->First = currentframe;
        }
        if (counter > end) {
            DeleteImageEntry(currentframe);
            currentframe = NULL;
        }
        if (next == NULL) {
            list->Last = currentframe;
        }
        currentframe = next;
        counter += 1;
    }

    printf("Operation Crop is done! New number of frames is %i.\n", list->Length);
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
    IENTRY *current = list->First;
    IENTRY *next = NULL;
    int count = factor;
    int totalFrames = 0;
    while (current != NULL) {
        if (count == factor) {
            count = 0;
            current = current->Next;
            totalFrames += 1;
        }
        else {
            next = current->Next;
            DeleteImageEntry(current);
            current = next;
        }
        count += 1;
    }
    printf("Operation Fast Forward is done! New number of frames is %i.\n", totalFrames);
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	/* to be implemented */
    IENTRY *current = list->First;
    IENTRY *next = NULL;
    IENTRY *tmp = NULL;
    while (current) {
        next = current->Next;
        tmp = current->Next;
        current->Next = current->Prev;
        current->Prev = tmp;
        current = next;
    }
    tmp = list->First;
    list->First = list->Last;
    list->Last = tmp;
    
}

/* EOF */
