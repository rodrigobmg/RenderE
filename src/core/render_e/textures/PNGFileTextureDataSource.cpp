/*
 *  RenderE
 *
 *  Created by Morten Nobel-Jørgensen ( http://www.nobel-joergnesen.com/ ) 
 *  License: LGPL 3.0 ( http://www.gnu.org/licenses/lgpl-3.0.txt )
 */

#include "PNGFileTextureDataSource.h"

#ifndef RENDER_E_NO_PNG

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <sstream>
#include "../Log.h"

namespace render_e {


PNGFileTextureDataSource::PNGFileTextureDataSource() {
}

PNGFileTextureDataSource::PNGFileTextureDataSource(const PNGFileTextureDataSource& orig) {
}

PNGFileTextureDataSource::~PNGFileTextureDataSource() {
}

TextureLoadStatus PNGFileTextureDataSource::LoadTexture(const char* name, unsigned int &outWidth, unsigned int &outHeight, TextureFormat &outFormat, unsigned char **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;

    FILE *fp = fopen(name, "rb");
	using std::endl;
	if (!fp){
        std::stringstream ss;
		ss<<"Cannot open png "<<name;
        ERROR(ss.str());
		return ERROR_READING_FILE;
	}

	
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
	
	if (png_ptr == NULL) {
        fclose(fp);
        return ERROR;
    }
	

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, 
			(png_infopp)NULL, (png_infopp)NULL);
        return ERROR;
    }

	

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, 
			(png_infopp)NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return INVALID_FORMAT;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

	
	/*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_INTERLACE_NONE, NULL);
	outWidth = info_ptr->width;
	outHeight = info_ptr->height;

	switch (info_ptr->color_type) {
		case PNG_COLOR_TYPE_RGBA:
			outFormat = RGBA;
			break;
		case PNG_COLOR_TYPE_RGB:
			outFormat = RGB;
			break;
		default:
            std::stringstream ss;
			ss << "Color type " << info_ptr->color_type << " not supported";
            ERROR(ss.str());
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			return INVALID_FORMAT;
	}
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*) malloc(row_bytes * outHeight);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
		
	for (int i = 0; i < outHeight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
	}
	
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return OK;
}
}
#endif // RENDER_E_PNG
