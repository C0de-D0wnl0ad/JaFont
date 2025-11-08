#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include "q3jafont.h"
#include "font_create.h"

// adnan - TODO - rewrite this to determine the optimal image size based on the actual size of each glyph...
//#define STEP_1x 256
//#define STEP_1y 256
#define STEP_1x 16384
#define STEP_1y 768
#define STEP_2x 512
#define STEP_2y 256
#define STEP_3x 512
#define STEP_3y 512
#define STEP_4x 1024
#define STEP_4y 512
#define STEP_5x 1024
#define STEP_5y 1024
// adnan - added new sizes to support huge fonts!
#define STEP_6x 2048
#define STEP_6y 1024
#define STEP_7x 2048
#define STEP_7y 2048
#define STEP_8x 4096
#define STEP_8y 2048
#define STEP_9x 4096
#define STEP_9y 4096

int ftSize;
// adnan - add a second size to declare the width, while ftSize will hold the height
int ftSizeWidth;
int blitsizex;
int blitsizey;

//using namespace std;
//int wuWriteFontDat( wJAf_t *wfont, char *filename );
int wuFileSuffixSet( char *ftName, char *suffix, char *endName );
// adnan added ability to define height and width for each glyph separately
int wuCreateFont(wJAf_t *wfont, char *filename, int ptSizeHeight, int ptSizeWidth, char *fontname);
//int wuCreateFont( wJAf_t *wfont, char *filename, int ptSize, char *fontname );
// adnan - try the optimal font size creation
int azCreateOptimalFont(wJAf_t *wfont, char *filename, int ptSizeHeight, int ptSizeWidth, char *fontname);
wJAf_t wuFont;
extern int can_not_add;

int main(int argc, char *argv[])
{
	int step;
	char ftName[64];
	char endName[64];
	printf( " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf( " JAfont by Wudan, OJP et al, built with help from Bejitt and Fracmann\n" );
	printf("   Modifed by Adnan 'skew' Zafar to make fonts with varying aspect ratios\n");
	printf( " A Tool for creating fonts for Jedi Academy from TrueType font files.\n" );
	printf( " Version 0.0.1b\n" );
	printf( " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	
	int comp_check;
	if( argc == 1 )
	{
		printf( " usage: JAfont -create <fontfile> <size> <JAfontname>\n" );
		printf( " example: jafont -create verdana.ttf 16 anewhope\n\n" );
		printf(" OR for widescreen fonts...\n");
		printf(" usage: JAfont -create_sized <fontfile> <font_height> <font_width> <JAfontname>\n");
		printf( " example: jafont -create_sized anewhope.ttf 180 135 anewhope\n\n");
		
		//system("PAUSE");
		return 0;
	}

	// adnan - there is no strcasecmp outside of POSIX!
	//if( !strcasecmp( argv[1], "-create" ) ) 
	if( !strcmp( argv[1], "-create" ) )
	{
		//printf( "Create Option:\n\n" );
		strcpy( ftName, argv[2] );
		ftSize = atoi( argv[3] );
		strcpy( endName, argv[4] );
		
		step = 1;
		while( step < 6 )
		{
			can_not_add = 0;
			switch(step)
			{
				case 1:
				blitsizex = STEP_1x;
				blitsizey = STEP_1y;
				break;
				case 2:
				blitsizex = STEP_2x;
				blitsizey = STEP_2y;
				break;
				case 3:
				blitsizex = STEP_3x;
				blitsizey = STEP_3y;
				break;
				case 4:
				blitsizex = STEP_4x;
				blitsizey = STEP_4y;
				break;
				case 5:
				blitsizex = STEP_5x;
				blitsizey = STEP_5y;
				break;
			}
			// adnan - pass in the one size as both width and height if not trying to create widescreen fonts
			wuCreateFont(&wuFont, ftName, ftSize, ftSize, endName);
			//wuCreateFont( &wuFont, ftName, ftSize, endName );
			if( can_not_add )
			{
				printf( " --- step %d: unsuccessful (%d), couldn't make %d by %d image.\n", step, can_not_add, blitsizex, blitsizey );
				step++;
			}
			else
			{
				printf( " --- (%d) All glyphs blitted to image successfully to a %d by %d image.\n", ftSize, blitsizex, blitsizey );
				step = 6;
			}
		}
		return 0;
	}

	// adnan - try the optimal sized approach
	else if (!strcmp(argv[1], "-create_sized"))
	{
		//printf( "Create Option:\n\n" );
		strcpy(ftName, argv[2]);
		ftSize = atoi(argv[3]);
		// adnan - now the font width is the 4th parameter
		ftSizeWidth = atoi(argv[4]);
		// adnan - now the font output name is the 5th parameter
		strcpy(endName, argv[5]);
		//strcpy(endName, argv[4]);

		if (!azCreateOptimalFont(&wuFont, ftName, ftSize, ftSizeWidth, endName))
		{
			printf("ERROR: Couldn't create optimal font!\n");
			return 1;
		}
		
		return 0;
	}
	
	return 1;
}
