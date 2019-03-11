#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float factor = (float) im.w * im.h;

    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            for (int c=0; c<im.c; c++){
                set_pixel(im, w, h, c, 1 / factor);
            }
        }
    } 
}

image make_box_filter(int w)
{
    image filter =  make_image(w, w, 1);
    l1_normalize(filter);
    return filter;
}

float get_pixel_with_pad(image im, int x, int y, int c)
{
    // TODO Fill this in
    // use clamp padding strategies
    if (x < 0 || y < 0){
        return 0.;
    } else if (x > im.w || y > im.h) {
        return 0.;
    } else {
        return im.data[x + im.w*y + im.w*im.h*c];
    }
}


image convolve_image(image im, image filter, int preserve)
{

    // Note that filter's size can'n be even, i.e 1x1, 3x3, 5x5 etc
    assert(im.c == filter.c || filter.c == 1);
    
    int numOfChannel;
    if (im.c == filter.c){
        if (preserve == 1){
            numOfChannel = im.c;
        } else {
            numOfChannel = 1;
        }
    } else {
        numOfChannel = im.c;
    } 

    // printf("%i %i %i\n", im.w, im.h, numOfChannel);
    // printf("%i %i %i\n", filter.w, filter.h, filter.c);

    image convolved = make_image(im.w, im.h, numOfChannel);
    // printf("%i\n", numOfChannel);

    int halfWidthOfFilter = filter.w / 2;
    int halfHeightOfFilter = filter.h / 2;
    float sum, x_im, y_im, filter_pixel, im_pixel;

    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            sum = 0;
            for (int c=0; c<im.c; c++){
                if (numOfChannel > 1){
                    sum = 0;
                }
                for (int filter_w=0; filter_w<filter.w; filter_w++){
                    x_im = w + filter_w - halfWidthOfFilter;
                    for (int filter_h=0; filter_h<filter.h; filter_h++){
                            y_im = h + filter_h - halfHeightOfFilter;
                            im_pixel = get_pixel(im, x_im, y_im, c);
                            if (filter.c > 1){
                                filter_pixel = get_pixel(filter, filter_w, filter_h, c);
                            } else {
                                filter_pixel = get_pixel(filter, filter_w, filter_h, 0);
                            }
                            sum = sum + im_pixel * filter_pixel;
                        }
                    }
                if (numOfChannel > 1){
                    sum = sum > 1.0 ? 1.0 : sum; 
                    sum = sum < 0.0 ? 0.0 : sum; 
                    set_pixel(convolved, w, h, c, sum);
                }
            }
            if (numOfChannel == 1){
                sum = sum > 1.0 ? 1.0 : sum; 
                sum = sum < 0.0 ? 0.0 : sum; 
                set_pixel(convolved, w, h, 0, sum);
            }
        }
    } 

    return convolved;
}

image make_highpass_filter()
{
    int numOfChannel = 3;
    image highpass = make_image(3,3,numOfChannel);
    
    for (int c=0; c<numOfChannel; c++){

        set_pixel(highpass, 0, 0, c, 0.);
        set_pixel(highpass, 1, 0, c, -1.);
        set_pixel(highpass, 2, 0, c, 0.);

        set_pixel(highpass, 0, 1, c, -1);
        set_pixel(highpass, 1, 1, c, 4.);
        set_pixel(highpass, 2, 1, c, -1.);

        set_pixel(highpass, 0, 2, c, 0.);
        set_pixel(highpass, 1, 2, c, -1.);
        set_pixel(highpass, 2, 2, c, 0.);
    }
    return highpass;
}

image make_sharpen_filter()
{
    int numOfChannel = 3;
    image sharpen = make_image(3,3,numOfChannel);
    
    for (int c=0; c<numOfChannel; c++){
        set_pixel(sharpen, 0, 0, c, 0.);
        set_pixel(sharpen, 1, 0, c, -1.);
        set_pixel(sharpen, 2, 0, c, 0.);

        set_pixel(sharpen, 0, 1, c, -1);
        set_pixel(sharpen, 1, 1, c, 5.);
        set_pixel(sharpen, 2, 1, c, -1.);

        set_pixel(sharpen, 0, 2, c, 0.);
        set_pixel(sharpen, 1, 2, c, -1.);
        set_pixel(sharpen, 2, 2, c, 0.);
    }
    return sharpen;
}

image make_emboss_filter()
{
    int numOfChannel = 3;
    image emboss = make_image(3,3,numOfChannel);
    
    for (int c=0; c<numOfChannel; c++){
        set_pixel(emboss, 0, 0, c, -2.);
        set_pixel(emboss, 1, 0, c, -1.);
        set_pixel(emboss, 2, 0, c, 0.);

        set_pixel(emboss, 0, 1, c, -1);
        set_pixel(emboss, 1, 1, c, 1.);
        set_pixel(emboss, 2, 1, c, 1.);

        set_pixel(emboss, 0, 2, c, 0.);
        set_pixel(emboss, 1, 2, c, 1.);
        set_pixel(emboss, 2, 2, c, 2.);
    }
    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
