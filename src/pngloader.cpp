#include "pngloader.hpp"

#include <stdio.h>
#include <stdexcept>
#include <png.h>
#include <pngconf.h>

Texture textureData(const std::string &file) {

    FILE *texture = fopen(file.c_str(), "rb");
    if (!texture)
    {
        throw std::domain_error("Could not open texture file");
    }
    unsigned char header[8] = {0}; 
    fread(header, 1, 8, texture);
    bool is_png = !png_sig_cmp((png_bytep)header, 0, 8);
    
    if (!is_png)
    {
        throw std::domain_error("Texture doesn't appear to be a PNG");
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, [](png_structp, png_const_charp) {}, [](png_structp, png_const_charp){});
    
    if (!png_ptr)
    {
    	throw std::domain_error("Could not create read struct");
    }
    png_set_sig_bytes(png_ptr, 8);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        throw std::domain_error("Could not create info struct");
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        throw std::domain_error("Could not create end info struct");
    }

    png_init_io(png_ptr, texture);


    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    unsigned char *rgbdata = new unsigned char[(width*3)*height];

    int pixelStride = png_get_channels(png_ptr, info_ptr);
    
    int p = 0;
    for(int i = 0; i < height; i++)
    {
    	for(int j = 0; j < width * pixelStride; j+=pixelStride)
    	{
            rgbdata[p++] = row_pointers[i][j];
            rgbdata[p++] = row_pointers[i][j+1];
            rgbdata[p++] = row_pointers[i][j+2];
    	}
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    Texture textureData;
    textureData.data = rgbdata;
    textureData.width = width;
    textureData.height = height;
    
    return textureData;
}