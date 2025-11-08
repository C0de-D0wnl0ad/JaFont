//font_create.cpp
//wu_FreeType.cpp
//tee hee hee!
#include <stdio.h>
#include "wu_io_func.h"
#include "font_create.h"
//#include "png.h"
#include <libpng/png.h>

// adnan
#include <math.h>

//wJAf_t wuFont;
int can_not_add;

int pngWrite( char *file_name, int *width, int *height, unsigned char *buffer )
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	//png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_color_8 sig_bit;
	int i;
	int j;
	int k;
	int line;
	// adnan - replaced this declaration
	//png_bytep *row_pointers;
	png_bytepp row_pointers;
	
	// adnan - use the secure fopen
	errno_t err;
	//fp = fopen(file_name, "wb");
	err = fopen_s(&fp, file_name, "wb");
	if (err != 0)
	{
		printf("ERROR opening file %s in pngWrite()!\n", file_name);
		return 0;
	}

	png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,NULL,NULL,NULL );
	if( png_ptr == NULL )
	{
		fclose(fp);
		return 0;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return 0;
	}

	//if (setjmp(png_ptr->jmpbuf))
	// adnan - fixed since libpng 1.5.0 onwards doesnt let you directly access members of the png struct.
	//  see http://patchwork.trinitydesktop.org/patch/101/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 0;
	}
	
	png_init_io(png_ptr, fp);
	//width = 64;
	//height = 64;
	bit_depth = 8;
	color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	
	png_set_IHDR(png_ptr, info_ptr, *width, *height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	sig_bit.red = bit_depth;
	sig_bit.green = bit_depth;
	sig_bit.blue = bit_depth;
	sig_bit.alpha = bit_depth;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);
	
	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);
	
	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
	for (unsigned row = 0; row < *height; row++)
	{
		if(color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			row_pointers[row] = (png_bytep)malloc(*width * 4);
		}
		else
		{
  			row_pointers[row] = (png_bytep)malloc(*width * 3);
		}
	}
	
	if ( row_pointers == NULL )
	{
		// adnan - close the file before proceeding
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 0;
	}
	
	k = 0;
	i = 0;
	while( i < *height )
	{
		j = 0;
		while( j < *width * 4 )
		{
			row_pointers[i][j+0] = buffer[k];
			k++;
			row_pointers[i][j+1] = buffer[k];
			k++;
			row_pointers[i][j+2] = buffer[k];
			k++;

			// adnan - only write this last byte if space for an alpha channel was allocated
			if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			{
				row_pointers[i][j + 3] = buffer[k];
			}
			k++;
			
			//row_pointers[i][j+3] = (int)((float)(row_pointers[i][j+0] + row_pointers[i][j+1] + row_pointers[i][j+2])/3.0);
			//row_pointers[i][j+3] = 255;
			// adnan - skip 3 bytes if no alpha channel, otherwise skip 4
			if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
				j += 4;
			else
				j += 3;
		}
		i++;
	}

	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	// adnan - only close the file if we can
	if(fp)
		fclose(fp);
	return 1;
}

//int PutCharOnBuffer

int FTnext_p2 ( int a )
{
	int rval=1;
	while(rval < a)
	{
		rval<<=1;
	}
	return rval;
}

void PutDotOnBuffer( unsigned char *buffer, int width, int height, int xpos, int ypos )
{
	int buf_pos = ((ypos * width) + xpos) * 4;
	buffer[buf_pos+0] = 255;
	buffer[buf_pos+1] = 255;
	buffer[buf_pos+2] = 255;
	buffer[buf_pos+3] = 255;
}

void PutDotOnBuffer2( unsigned char *buffer, int width, int height, int xpos, int ypos, int value )
{
	int buf_pos = ((ypos * width) + xpos) * 4;
	/*
	buffer[buf_pos+0] = value;
	buffer[buf_pos+1] = value;
	buffer[buf_pos+2] = value;
	buffer[buf_pos+3] = value;
	*/
	buffer[buf_pos+0] = 255;
	buffer[buf_pos+1] = 255;
	buffer[buf_pos+2] = 255;
	buffer[buf_pos+3] = value;
}

void GetGlyphSize( FT_Face font, int numglyph, int *wi, int *hi )
{
	FT_Glyph glyph;
	FT_BitmapGlyph bmglyph;
	
  //glyph fields:
  /*    width        :: The glyph's width.                                 */
  /*    height       :: The glyph's height.                                */
  /*    horiBearingX :: Horizontal left side bearing.                      */
  /*    horiBearingY :: Horizontal top side bearing.                       */
  /*    horiAdvance  :: Horizontal advance width.                          */
  /*    vertBearingX :: Vertical left side bearing.                        */
  /*    vertBearingY :: Vertical top side bearing.                         */
  /*    vertAdvance  :: Vertical advance height.                           */

	FT_Load_Glyph( font, FT_Get_Char_Index( font, numglyph ), FT_LOAD_DEFAULT );
	FT_Get_Glyph( font->glyph, &glyph );
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	bmglyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bmglyph->bitmap;
	//*wi = FTnext_p2( bitmap.width );
	//*hi = FTnext_p2( bitmap.rows );
	*wi = bitmap.width;
	*hi = bitmap.rows;
};

// adnan - return 0 if successful
//void PutGlyphOnBuffer( unsigned char *buffer, int width, int height, FT_Face font, wJAf_t *wfont, int numglyph, int xpos, int ypos )
int PutGlyphOnBuffer(unsigned char *buffer, int width, int height, FT_Face font, wJAf_t *wfont, int numglyph, int xpos, int ypos)
{
	FT_GlyphSlot glyphS;
	FT_Glyph glyph;
	FT_BitmapGlyph bmglyph;
	int wi;
	int hi;
	int i, j;
	
	FT_Load_Glyph( font, FT_Get_Char_Index( font, numglyph ), FT_LOAD_DEFAULT );
	FT_Get_Glyph( font->glyph, &glyph );
	glyphS = font->glyph;
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	bmglyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bmglyph->bitmap;
	
	wi = FTnext_p2( bitmap.width );
	hi = FTnext_p2( bitmap.rows );
	wfont->glyph[numglyph].phWidth = font->glyph->metrics.width >> 6;
	wfont->glyph[numglyph].phHeight = font->glyph->metrics.height >> 6;
	wfont->glyph[numglyph].hAdv = font->glyph->metrics.horiAdvance >> 6;
	wfont->glyph[numglyph].hOfs = font->glyph->metrics.horiBearingX >> 6;
	wfont->glyph[numglyph].baseline = (font->glyph->metrics.horiBearingY >> 6);
	wfont->glyph[numglyph].st1[0] = (float)(xpos) / (float)(width);
	wfont->glyph[numglyph].st1[1] = (float)(ypos) / (float)(height);
	wfont->glyph[numglyph].st2[0] = (float)(xpos + wfont->glyph[numglyph].phWidth) / (float)(width);
	wfont->glyph[numglyph].st2[1] = (float)(ypos + wfont->glyph[numglyph].phHeight) / (float)(height);
	//wfont->glyph[numglyph].st1[1] = 1.0 - wfont->glyph[numglyph].st1[1];
	//wfont->glyph[numglyph].st2[1] = 1.0 - wfont->glyph[numglyph].st2[1];
 	
	// adnan - check if the glyph fits in the image from this offset. if not, the image needs to be resized or need to try the next row
	if ((width <= xpos + wfont->glyph[numglyph].phWidth) || (height <= ypos + wfont->glyph[numglyph].phHeight))
		return 1;

	j = 0;
	while( j < bitmap.rows )
	{
		i = 0;
		while( i < bitmap.width )
		{
			PutDotOnBuffer2( buffer, width, height, i + xpos, j + ypos, bitmap.buffer[i + bitmap.width * j] );
			i++;
		}
		j++;
	}

	return 0;
}

int wuFontTexBufferCreate( FT_Face font, wJAf_t *wfont, int width, int height, char *pngfile )
{
	int i, j;
	int x1, y1;
	int xSz, ySz;
	int Space;
	int rowhigh;
	int newrow;
	unsigned char *buffer;
	int buf_size = width * height * 4;
	FT_Glyph glyph[256];
	//creating rgba buffer
	buffer = new unsigned char[buf_size];
	
	wfont->mAscender = font->ascender >> 6;
	wfont->mDescender = font->descender >> 6;
	if(wfont->mDescender < 0)
	{
		//wfont->mDescender = -wfont->mDescender;
	}
	wfont->mHeight = wfont->mAscender + wfont->mDescender;
	
	i = 0;
	j = 0;
	while( i < width * height )
	{
		buffer[j] = 255;
		j++;
		buffer[j] = 255;
		j++;
		buffer[j] = 255;
		j++;
		buffer[j] = 0;
		j++;
		i++;
	}
	
	x1 = 0;
	y1 = 0;
	Space = 7;
	rowhigh = 0;
	//now we load glyphs on to the buffer!  yay!
	i = 32;
	while( i < 256 )
	{
		if( i <= 0x7E || i >= 0xA0 )
		{
			GetGlyphSize( font, i, &xSz, &ySz );
			if( (x1 + xSz + Space) > width )
			{
				x1 = 0;
				y1 += rowhigh + Space;
				if( y1 + rowhigh > height )
				{
					can_not_add = (256 - i);
					i = 256;
				}
				rowhigh = 0;
			}
			else
			{
			
				if((x1 < width) && (y1 < height))
				{
					//PutDotOnBuffer( buffer, width, height, x1, y1 );
					// adnan - check if the glyph could not be added due to the bitmap dimensions exceeding the overall image size
					//PutGlyphOnBuffer(buffer, width, height, font, wfont, i, x1, y1);
					if (PutGlyphOnBuffer(buffer, width, height, font, wfont, i, x1, y1))
						can_not_add++;
				}
				else
				{
					can_not_add++;
				}
			
				x1 += xSz + Space;
				if( ySz > rowhigh )
				{
					rowhigh = ySz;
				}
				i++;
			}
		}
		else
		{
			i++;
		}
	}
	
	pngWrite( pngfile, &width, &height, buffer );
	//delete buffer;
	// adnan - need to deallocate the buffer as an array to avoid memory leaks
	delete[] buffer;
	return 0;
}

int wuFileSuffixSet( char *ftName, char *suffix, char *endName )
{
	int i;
	int len;
	int dotlen;
	char tempname[64];
	len = strlen(ftName);
	dotlen = 0;
	i = 0;
	while( i < len )
	{
		if(ftName[i] == '.')
		{
			dotlen = i;
			i = len;
		}
		else
		{
			dotlen = len;
		}
		i++;
	}
	//return dotlen;
	
	//some rad name sorting stuff
	if ( ftName[0] == '/' || ftName[0] == '\\' )
	{
		strcpy ( tempname, &ftName[1] );
		if ( ftName[dotlen] == '.' )
		{
			tempname[dotlen-1] = 0;
		}
	}
	else
	{
		strcpy ( tempname, ftName );
		if( ftName[dotlen] == '.' )
		{
			tempname[dotlen] = '\0';
		}
	}
	
	// replace all '\' with '/'
	for ( i = 0; tempname[i]; i++ )
	{
		if ( tempname[i] == '\\' )
		{
			tempname[i] = '/';
		}
	}
	StringSet( endName, "%s%s", tempname, suffix );
	return 0;
}

int wuWriteFontDat( wJAf_t *wfont, char *filename, int pt_size )
{
	FILE *fp;
	char magic[4];
	magic[0] = 0;
	magic[1] = 0;
	magic[2] = 0;
	magic[3] = 0;
	float height, ascend, descend;
	int winning_baseline;
	int winning_height;
	int i;
	int def = 0;
	fp = fopen(filename, "wb");
	// adnan - error check the fopen
	if (!fp)
	{
		printf("ERROR in wuWriteFontDat: Couldn't open %s !\n", filename);
		return 0;
	}
	fseek( fp, 0, SEEK_SET );
	i = 0;
	while( i < 256 )
	{
/*void putf( float number, FILE *file );
void puts( int number, FILE *file );*/
		puts( wfont->glyph[i].phWidth, fp );
		puts( wfont->glyph[i].phHeight, fp );
		puts( wfont->glyph[i].hAdv, fp );
		puts( wfont->glyph[i].hOfs, fp );
		putw( wfont->glyph[i].baseline, fp );
		putf( wfont->glyph[i].st1[0], fp );
		putf( wfont->glyph[i].st1[1], fp );
		putf( wfont->glyph[i].st2[0], fp );
		putf( wfont->glyph[i].st2[1], fp );
		//printf( "char %1c (%d) %d %d %d %d %d | %2.2f %2.2f %2.2f %2.2f\n", i, i, wfont->glyph[i].phWidth, wfont->glyph[i].phHeight, wfont->glyph[i].hAdv, wfont->glyph[i].hOfs, wfont->glyph[i].baseline, wfont->glyph[i].st1[0], wfont->glyph[i].st1[1], wfont->glyph[i].st2[0], wfont->glyph[i].st2[1] );
		i++;
	}
	
	/*
	puts( wfont->mPointSize, fp );
	puts( wfont->mHeight, fp );
	puts( wfont->mAscender, fp );
	puts( wfont->mDescender, fp );
	*/
	/*
	puts( 16, fp );
	puts( 18, fp );
	puts( 16, fp );
	puts( 12, fp );
	*/
	float size_scale = (float)(pt_size) / (float)(wfont->mHeight);
	height = (float)(pt_size);
	//ascend = ((float)(wfont->mAscender) / (float)(wfont->mHeight)) * pt_size;
	//descend = ((float)(wfont->mDescender) / (float)(wfont->mHeight)) * pt_size;
	ascend = ((float)(wfont->mAscender) * size_scale);
	descend = ((float)(wfont->mDescender) * size_scale);
	descend -= size_scale * 12.0;
	int iascend, idescend;
	iascend = (int)(ascend);
	idescend = (int)(descend);
	//ascend = ((float)(wfont->mAscender)) * size_scale;
	//descend = ((float)(wfont->mDescender)) * size_scale;
	
	//printf( "Font Junk: pt_size %d, height %d, Ascender %2.2f (%d), Descender %2.2f (%d)\n", pt_size, wfont->mHeight, ascend, iascend, descend, idescend );

	puts( iascend, fp );
	puts( pt_size, fp );
	puts( iascend, fp );
	puts( idescend, fp );
	//puts( 0, fp );
	//puts( 0, fp );
	puts( 0, fp );
	puts( 0, fp );
	fclose( fp );
}

extern int blitsizex;
extern int blitsizey;

//int wuCreateFont( wJAf_t *wfont, char *filename, int ptSize, char *fontname )
// adnan - added ability to set the width and height of the characters independently, in order to make widescreen fonts...
int wuCreateFont(wJAf_t *wfont, char *filename, int ptSizeHeight, int ptSizeWidth, char *fontname)
{
	char pngname[64];
	char fdname[64];
	char ftname[64];
	
	wuFileSuffixSet( fontname, "", ftname );
	wuFileSuffixSet( ftname, ".png", pngname );
	wuFileSuffixSet( ftname, ".fontdat", fdname );
	//printf ("%s %d %d\n\n", pngname, blitsizex, blitsizey );
	//return 1;
	
	FT_Library ftLib;
	FT_Face ftFont;
	
	FT_Init_FreeType( &ftLib );
	if (FT_New_Face(ftLib, filename, 0, &ftFont) != 0)
	{
		printf("ERROR: FreeType could not create font object.\n");
		return 0;
	}
	//FT_Set_Char_Size(ftFont, ptSize << 6, ptSize << 6, 96, 96);
	// adnan - pass in new width and height sizes independently
	FT_Set_Char_Size( ftFont, ptSizeWidth << 6, ptSizeHeight << 6, 96, 96);
	
	wuFontTexBufferCreate( ftFont, wfont, blitsizex, blitsizey, pngname );
	//wuWriteFontDat( wfont, fdname, ptSize );
	// adnan - pass in the height to determine ascenders and descenders
	wuWriteFontDat(wfont, fdname, ptSizeHeight);
	
	FT_Done_Face(ftFont);
	FT_Done_FreeType(ftLib);
	return 1;
}

#define MIN_GLYPH 32
#define MAX_GLYPH 255
#define GLYPH_SPACE 7

// adnan - added ability to set the width and height of the characters independently, in order to make optimally sized widescreen fonts...
int azCreateOptimalFont(wJAf_t *wfont, char *filename, int ptSizeHeight, int ptSizeWidth, char *fontname)
{
	char pngname[64];
	char fdname[64];
	char ftname[64];

	int area_ideal;
	int width_ideal_b2;
	int h_max_font;
	int w_max_font;
	int h_min_b2;
	int h_global_min_b2;
	int w_min_b2;
	int w_global_min_b2;
	int w_glyph, h_glyph;
	int w_optimal, w_optimal_b2;
	int area_current, area_global_min;

	int req_width, req_height, req_area;

	wuFileSuffixSet(fontname, "", ftname);
	wuFileSuffixSet(ftname, ".png", pngname);
	wuFileSuffixSet(ftname, ".fontdat", fdname);
	//printf ("%s %d %d\n\n", pngname, blitsizex, blitsizey );
	//return 1;

	FT_Library ftLib;
	FT_Face ftFont;

	FT_Init_FreeType(&ftLib);
	if (FT_New_Face(ftLib, filename, 0, &ftFont) != 0)
	{
		printf("ERROR: FreeType could not create font object.\n");
		return 0;
	}
	//FT_Set_Char_Size(ftFont, ptSize << 6, ptSize << 6, 96, 96);
	// adnan - pass in new width and height sizes independently
	FT_Set_Char_Size(ftFont, ptSizeWidth << 6, ptSizeHeight << 6, 96, 96);

	// adnan - find the optimal width and height:
	area_global_min = 0;

	// 0. iterate all glyphs to get the max width and height for each, and scale up to 256 to get ideal pixels
	h_max_font = w_max_font = 0;
	for (int i = MIN_GLYPH; i <= MAX_GLYPH; i++)
	{
		GetGlyphSize(ftFont, i, &w_glyph, &h_glyph);
		w_max_font = (w_glyph > w_max_font) ? w_glyph : w_max_font;
		h_max_font = (h_glyph > h_max_font) ? h_glyph : h_max_font;
	}
	area_ideal = (w_max_font + GLYPH_SPACE) * (h_max_font + GLYPH_SPACE) * (MAX_GLYPH - MIN_GLYPH);
	
	printf("  Font glyph extents: %d,%d\n", w_max_font + GLYPH_SPACE, h_max_font + GLYPH_SPACE);
	printf("  Font requires at most this pixel area = %d\n", area_ideal);

	// 1. calculate the ideal square side length, rounded up to the nearest integer
	width_ideal_b2 = ceil(log2(sqrt(area_ideal)));
	//printf("  W_ideal = %f, log2(W_ideal) = %d\n", pow(2, width_ideal_b2), width_ideal_b2);

	// 2. calculate the minimum image height based on the largest font character
	h_min_b2 = ceil(log2(h_max_font));

	// 3. Pick heights from the minimum to the ideal
	for (int i = h_min_b2; i < width_ideal_b2; i++)
	{
		
		//  3.0 calculate the number of rows that can fit in this height
		int potential_rows = (1 << i) / h_max_font;

		//  3.1 find the optimal width based on one row
		req_width = req_area = 0;
		req_height = (1 << i);
		for (int j = MIN_GLYPH; j <= MAX_GLYPH; j++)
		{
			GetGlyphSize(ftFont, j, &w_glyph, &h_glyph);
			req_width += (w_glyph + GLYPH_SPACE);
		}
		
		//  3.2 find the optimal width based on the number of rows
		req_width /= potential_rows;
		
		//  3.3 find the area using the optimal width, and try this area to get the optimal base 2 width
		area_ideal = (1<<i) * req_width;

		//  3.4 for each height, calculate the optimal width
		w_optimal_b2 = ceil(log2(area_ideal / (1<<i)));
		w_optimal = (1<<w_optimal_b2);

		//  3.5 compute the actual area
		area_current = w_optimal * (1<<i);

		printf("   > Trying %d x %d == %d...\n", (1 << i), w_optimal, area_current);

		//  3.6 if area_actual <= minimum_area_actual, store the width and height
		if (area_global_min == 0 || area_current <= area_global_min)
		{
			area_global_min = area_current;
			h_global_min_b2 = i;
			w_global_min_b2 = w_optimal_b2;
		}
	}
	
	
	blitsizex = pow(2, w_global_min_b2);
	blitsizey = pow(2, h_global_min_b2);

	printf("  Optimal font width x height = area: %d x %d = %d\n", blitsizex, blitsizey, blitsizex*blitsizey);

	wuFontTexBufferCreate(ftFont, wfont, blitsizex, blitsizey, pngname);
	//wuWriteFontDat( wfont, fdname, ptSize );
	// adnan - pass in the height to determine ascenders and descenders
	wuWriteFontDat(wfont, fdname, ptSizeHeight);

	FT_Done_Face(ftFont);
	FT_Done_FreeType(ftLib);

	printf("  Successfully created font files %s and %s!\n", pngname, fdname);

	return 1;
}
