#include "PNG.h"

/*http://zarb.org/~gc/html/libpng.html*/

#define PNG_BYTES_TO_CHECK 8

Image* createImage(int width,int height){
	Image* image = (Image*)malloc(sizeof(Image));
    image->width    = width;
    image->height   = height;
    image->channels = 4;
    image->data = (unsigned char*) malloc(image->width*image->height*image->channels);
	memset(image->data,0,image->width*image->height*image->channels);
	return image;
}


Image* readPng(const char* file_name){
	/* open file and test for it being a png */
	FILE* fp;
	unsigned char buf[PNG_BYTES_TO_CHECK];

	if ((fp = fopen(file_name, "rb")) == NULL)
		return NULL;

	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
		return NULL;

	if(png_sig_cmp(&buf[0], (png_size_t)0, PNG_BYTES_TO_CHECK))
		return NULL;

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr || !png_ptr)
		return NULL;

	if (setjmp(png_jmpbuf(png_ptr)))
		return NULL;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	Image* img = createImage(png_get_image_width(png_ptr, info_ptr)
			,png_get_image_height(png_ptr, info_ptr));

	//Currently in another file
	//addAlpha(img);

	png_byte color_type  = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
	int number_of_passes = png_set_interlace_handling(png_ptr);

	if(bit_depth == 16)
		png_set_strip_16(png_ptr);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_PALETTE){
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
	}

	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);


	png_read_update_info(png_ptr, info_ptr);

	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * img->height);
	png_bytep pointer = (png_bytep)img->data;
	for (int y=0; y < img->height; ++y){
		row_pointers[y] = pointer;
		pointer += img->width*img->channels;
	}

	png_read_image(png_ptr, row_pointers);

	/*CONVERT TO FLAT ARRAY*/
	free(row_pointers);
	//delete stuff

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	fclose(fp);
	return img;
}

//http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-4
void writePng(const char* filename,Image* img){
	/* create file */
	FILE *fp = fopen(filename, "wb");
	if (!fp)
		return;

	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr || !png_ptr)
		return;

	//need to clean up memory on these errors

	if (setjmp(png_jmpbuf(png_ptr)))
		return;

	png_init_io(png_ptr, fp);

	int color_type = PNG_COLOR_TYPE_RGB;
	if(img->channels == 4){
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	}
	png_set_IHDR(png_ptr, info_ptr, img->width, img->height,
			8, color_type, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);
	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * img->height);
	png_bytep pointer = (png_bytep)img->data;
	for(int y = 0; y < img->height; ++y){
		row_pointers[y] = pointer;
		pointer += img->channels*img->width;
	}

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, NULL);

	free(row_pointers);

write_cleanup:
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);
}
