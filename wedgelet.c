#include "wedgelet.h"
Int cnt = 0;
int initWedgelet(Wedgelet *w, Int width, Int height)
{
	assert(width == height);
	if ((w->m_pbPattern = (Int *)malloc(width * height * sizeof(Int)) ) == NULL)
	{
		puts("mem allocate error");
		exit(1);
	}
	return 0;
}
int deleteWedgelet(Wedgelet *w)
{
	if(w->m_pbPattern != NULL)
	{
		free(w->m_pbPattern);
	}
	return 0;
}

// Generate pbPattern from isSym, ori, slope, offset
int setWedgelet(Wedgelet *w, Int width, Int height, Int isSym, Int ori, Int slope, Int offset, Int addToList)
{
	int i, j, shift;
	Int *pbData = w->m_pbPattern;
	assert(width == height);
	assert(w->m_pbPattern != NULL);
	w->m_width = width;
	w->m_height = height;
	w->m_isSym = isSym;
	w->m_slope = slope;
	w->m_ori = ori;
	w->m_offset = offset;
	if(isSym == 0)
	{
		//slope = inf;
		if(ori == 0)
		{
			for(i = 0; i < width; ++i)
			{
				for(j = 0; j < width; ++j)
				{
					pbData[i*width + j] = i < width/2 + offset? TRUE:FALSE;
				}
			}
		}
		else
		{
			for(i = 0; i < width; ++i)
			{
				for(j = 0; j < width; ++j)
				{
					pbData[i*width + j] = j < width/2 + offset? TRUE:FALSE;
				}
			}
		}
	}
	else
	{
		if (slope == 0) // 1:1
		{
			shift = 0;
			if(ori == 0)
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > j + offset ? TRUE:FALSE;
					}
				}
			}
			else
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > -j + width + offset ? TRUE:FALSE;
					}
				}
			}
		}
		else if(slope == 4 || slope == 6) // 2:1,4:1,+-
		{
			//shift = 1;
			if(slope == 4)shift = 1;
			else if(slope == 6)shift = 2;
			else shift = 0;
			if(ori == 0)
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > (j<<shift) + (width>>1) - (width>>(shift + 1)) + offset ? TRUE:FALSE;
					}
				}
			}
			else
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > -(j<<shift) + (width>>1) + (width>>(shift + 1)) + offset ? TRUE:FALSE;
					}
				}
			}
		}
		else if(slope == 5 || slope == 7) // 1:2,1:4,+-
		{
			if(slope == 5)shift = 1;
			else if(slope == 7)shift = 2;
			else shift = 0;
			if(ori == 0)
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > (j>>shift) + (width>>1) - (width>>(shift + 1)) + offset ? TRUE:FALSE;
					}
				}
			}
			else
			{
				for(i = 0; i < width; ++i)
				{
					for(j = 0; j < width; ++j)
					{
						pbData[i*width + j] = i > -(j>>shift) + (width>>1) + (width>>(shift + 1)) + offset ? TRUE:FALSE;
					}
				}
			}
		}
	}
	if(addToList)
	{
		g_wedgeletList[g_wlnum] = *w;
		g_wedgeletList[g_wlnum].m_pbPattern = (Int *)malloc(width * height * sizeof(Int));
		memcpy(g_wedgeletList[g_wlnum].m_pbPattern, w->m_pbPattern, width * height * sizeof(Int));
		g_wlnum++;
	}
#if PRINT_PATTERN
	++cnt;
	printf("%d\n",cnt);
	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < width; ++j)
		{
			putchar(pbData[i*width + j]==0?'X':'O');
		}
		putchar('\n'); 
	}
	puts("----------");
#endif
	return 0;
}

// Create all valid wedgelets.
int creatWedgeletList()
{
	int blkSize;
	int i, j, k, offset, n;
	int dummy = 0;
	int tmpsize = 0;
	//int idx_n, idx_ori, idx_sym, idx_offset;
	Wedgelet tmpWedgelet;
	g_wedgeletList = (Wedgelet *)malloc(NUM_WEDGELET*sizeof(Wedgelet));
	//g_wedgeletListSize = (Wedgelet **)malloc((DMM_WEDGELET_SIZE_MAX - DMM_WEDGELET_SIZE_MIN + 1)*sizeof(Wedgelet *));
	for(n = DMM_WEDGELET_SIZE_MIN; n < DMM_WEDGELET_SIZE_MAX + 1; ++n)
	{
		blkSize = 1<<n;
		//idx_n = n - DMM_WEDGELET_SIZE_MIN;
		initWedgelet(&tmpWedgelet, blkSize, blkSize);
		g_wedgeletListIdx[n] = g_wedgeletList + g_wlnum;
		tmpsize = g_wlnum;
		//for(j = 0; j < nOri; ++j)
		{
			for(i = 0; i < NUM_SYMMERTY; ++i)
			{
				if(isSymList[i] == 0)
				{
					for(offset = -(1<<(n-1))+1; offset <= (1<<(n-1)) - 1; ++offset) 
					{
						setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 0, dummy, offset, 1);
						setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 1, dummy, offset, 1);
					}
				}
				else
				{
					for(k = 0; k < NUM_SLOPE; ++k)
					{
						if(k==0) // 1:1
						{
							for(offset = -(1<<n) + 1; offset <= (1<<n) - 1; ++offset) 
							{
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 0, slopeList[k],offset, 1);
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 1, slopeList[k],offset, 1);
							}
						}
						else if(k==1||k==2) // 2:1,1:2
						{
							for(offset = -((1<<(n-1)) + (1<<(n-2)) - 1); offset <= ((1<<(n-1)) + (1<<(n-2))) - 1;) 
							{
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 0, slopeList[k],offset, 1);
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 1, slopeList[k],offset, 1);
								if(offset >= ((1<<(n-2)) - 1) || offset < -((1<<(n-2)) - 1) )offset+=2;
								else offset++;
							}
						}
						else if(n != 2)// 4:1,1:4
						{
							for(offset = -((1<<(n-1)) + (1<<(n-3)) - 1); offset <= (1<<(n-1)) + (1<<(n-3)) - 1;)
							{
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 0, slopeList[k], offset, 1);
								setWedgelet(&tmpWedgelet, blkSize, blkSize, isSymList[i], 1, slopeList[k], offset, 1);
								if(offset >= ((1<<(n-1)) - (1<<(n-3)) - 1) || offset < -((1<<(n-1))-(1<<(n-3)) - 1) )offset+=2;
								else offset++;
							}
						}
					}
				}
			}
		}
		deleteWedgelet(&tmpWedgelet);
		g_wedgeletListSizeN[n - DMM_WEDGELET_SIZE_MIN] = g_wlnum - tmpsize;
		//printf("%d\n",g_wlnum);
	}
	return 0;
}
int printWedgelet(Wedgelet w)
{
	printf("%d %d %d %d %d\n",w.m_width, w.m_isSym, w.m_ori, w.m_slope, w.m_offset);
	return 0;
}
int main()
{
	int i;
	freopen("wedgelet.txt","wb",stdout);
	creatWedgeletList();
	//printf("%d\n",g_wlnum);
	/*
	for(i = DMM_WEDGELET_SIZE_MIN; i < DMM_WEDGELET_SIZE_MAX + 1; ++i)
	{
		printWedgelet(g_wedgeletListIdx[i][0]);
	}
	*/
	
	for(i = 0; i < g_wlnum; ++i)
	{
		printWedgelet(g_wedgeletList[i]);
	}
	
	return 0;
}