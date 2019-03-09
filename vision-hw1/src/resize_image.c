#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{

    // take closest int
    int rounded_x = round(x);
    int rounded_y = round(y);

    float value = get_pixel(im, rounded_x, rounded_y, c);

    return value;
}

image nn_resize(image im, int w, int h)
{

    image resized_im = make_image(w, h, im.c);
    
    float a_w = (float) im.w / w;
    float b_w = -0.5 + 0.5 * a_w;

    float a_h = (float) im.h / h;
    float b_h = -0.5 + 0.5 * a_h;

    for (int i=0; i<w; i++){
    	for (int j=0; j<h; j++){
    		for (int k=0; k<im.c; k++){
    			// map new coordinates to old
	    		float old_x = i*a_w + b_w;
	    		float old_y = j*a_h + b_h;
	    		float interpolated_value = nn_interpolate(im, old_x, old_y, k);
	    		set_pixel(resized_im, i, j, k, interpolated_value);
	    	}
    	}
    }

    return resized_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
	int floor_x = floor(x);
	int ceil_x = ceil(x);

	int floor_y = floor(y);
	int ceil_y = ceil(y);
    
	float q1 = (y - floor_y) * get_pixel(im, floor_x, ceil_y, c) + (ceil_y - y) * get_pixel(im, floor_x, floor_y, c);
	float q2 = (y - floor_y) * get_pixel(im, ceil_x, ceil_y, c) + (ceil_y - y) * get_pixel(im, ceil_x, floor_y, c);
    
	float value = (ceil_x - x) * q1 + (x - floor_x) * q2;
    return value;
}

image bilinear_resize(image im, int w, int h)
{

    image resized_im = make_image(w, h, im.c);
    
    float a_w = (float) im.w / w;
    float b_w = -0.5 + 0.5 * a_w;

    float a_h = (float) im.h / h;
    float b_h = -0.5 + 0.5 * a_h;

    for (int i=0; i<w; i++){
    	for (int j=0; j<h; j++){
    		for (int k=0; k<im.c; k++){
    			// map new coordinates to old 
	    		float old_x = i*a_w + b_w;
	    		float old_y = j*a_h + b_h;
	    		float interpolated_value = bilinear_interpolate(im, old_x, old_y, k);
	    		set_pixel(resized_im, i, j, k, interpolated_value);
	    	}
    	}
    }

    return resized_im;
}

