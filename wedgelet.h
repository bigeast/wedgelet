#ifndef __AVS_DMM_WEDGELET__
#define __AVS_DMM_WEDGELET__
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DMM_WEDGELET_SIZE_MAX 5
#define DMM_WEDGELET_SIZE_MIN 2
#define DMM_WEDGELET_SIZE_NUM (DMM_WEDGELET_SIZE_MAX - DMM_WEDGELET_SIZE_MIN + 1)

#define PRINT_PATTERN 0
#define NUM_WEDGELET 1400

#define NUM_SYMMERTY 2
#define NUM_ORIENTATION 2
// 2:1, 4:1
#define NUM_SLOPE 5
#define NUM_OFFSET ((1<<DMM_WEDGELET_SIZE_MAX) - 1)
typedef int Int;
enum Int {
	FALSE,
	TRUE
};

struct wedgelet {
	Int m_width;
	Int m_height;
	Int m_isSym; // 0{vertical or horizontal partition}, 1{others}
	Int m_slope; // 0{1:1}, 100{2:1}, 101{1:2}, 110{4:1}, 111{1:4}
	Int m_ori; // 0{slope>0}, 1{slope<0}
	Int m_offset;
	Int *m_pbPattern;
};
typedef struct wedgelet Wedgelet;
Wedgelet *g_wedgeletList; // all of the valid wedgelet
Wedgelet *g_wedgeletListIdx[DMM_WEDGELET_SIZE_NUM]; // g_wedgeletListSize[n] = g_wedgeletList[num of wedgelet of size n]
Int g_wedgeletListSizeN[DMM_WEDGELET_SIZE_NUM]; // how many different wedgelets of size n
Int g_wlnum = 0;

Int isSymList[] = {0,1};
Int slopeList[] = {0,4,5,6,7};

/*
struct offsetIdx {
	Int numOffset; // number of all valid offsets
	Int *offset; // offsets
}g_wedgeletOffsetIdx[DMM_WEDGELET_SIZE_NUM][NUM_SYMMERTY][NUM_ORIENTATION][NUM_SLOPE];

Int g_wedgeletIdx[DMM_WEDGELET_SIZE_MAX - DMM_WEDGELET_SIZE_MIN + 1][NUM_SYMMERTY][NUM_ORIENTATION][NUM_SLOPE][NUM_OFFSET];
*/
int initWedgelet(Wedgelet *w, Int width, Int height);
int setWedgelet(Wedgelet *w, Int width, Int height, Int isSym, Int ori, Int slope, Int offset, Int addToList);
int creatWedgelet(Wedgelet **wlist);
int printWedgelet(Wedgelet w);
int deleteWedgelet(Wedgelet *w);
//int creatWedgeletList();
#endif