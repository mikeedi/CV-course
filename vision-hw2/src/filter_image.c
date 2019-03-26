#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{

    for (int c; c<im.c; c++){
        // factor is channel independent 
        float factor = 0;
        for (int w=0; w<im.w; w++){
            for (int h=0; h<im.h; h++){
                float pixel = get_pixel(im, w, h, c);
                factor += pixel;
            }
        }
        for (int w=0; w<im.w; w++){
            for (int h=0; h<im.h; h++){
                float pixel = get_pixel(im, w, h, c);
                set_pixel(im, w, h, c, pixel / factor);
            }
        }
    }
}

image make_box_filter(int w)
{
    image filter =  make_image(w, w, 1);
    // fill with 1
    for (int i=0;i<w*w;i++) filter.data[i] = 1.;
    l1_normalize(filter);
    return filter;
}

float get_pixel_with_pad(image im, int x, int y, int c)
{
    // use zero padding strategies
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
    } else if (filter.c == 1){
        numOfChannel = 1;
        if (preserve == 1) {
            numOfChannel = 3;
        }
    } 

    image convolved = make_image(im.w, im.h, numOfChannel);

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
                    // sum = sum > 1.0 ? 1.0 : sum; 
                    // sum = sum < 0.0 ? 0.0 : sum; 
                    set_pixel(convolved, w, h, c, sum);
                }
            }
            if (numOfChannel == 1){
                // sum = sum > 1.0 ? 1.0 : sum; 
                // sum = sum < 0.0 ? 0.0 : sum; 
                set_pixel(convolved, w, h, 0, sum);
            }
        }
    }    
    // clamp_image(convolved); 
    return convolved;
}

image make_highpass_filter()
{
    int numOfChannel = 1;
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
    int numOfChannel = 1;
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
    int numOfChannel = 1;
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

float dist_gauss(int x, int y, int center, float sigma){
    int sub_x = x - center;
    int sub_y = y - center;
    return 1. / (TWOPI * sigma * sigma) * exp(-1*(sub_x*sub_x + sub_y*sub_y) / (2*sigma*sigma));
}   

image make_gaussian_filter(float sigma)
{
    // TODO
    int size = ceil(sigma * 6);
    size = size % 2 == 0 ? size + 1: size; 
    int center = (int) size / 2;
    assert(size == center * 2. + 1.);
    image kernel = make_image(size, size, 1);

    for (int x=0; x<size; x++){
        for (int y=0; y<size; y++){
            for (int c=0; c<1; c++){
                set_pixel(kernel, x, y, c, dist_gauss(x, y, center, sigma));
            }
        }
    }
    l1_normalize(kernel);
    return kernel;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    for (int i=0;i<im.w*im.h*im.c;i++){
        im.data[i] = a.data[i] + b.data[i];
    }
    return im;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    for (int i=0;i<im.w*im.h*im.c;i++){
        im.data[i] = a.data[i] - b.data[i];
    }
    return im;
}


image make_gx_filter()
{
    int numOfChannel = 1;
    image sobel_filter = make_image(3, 3, numOfChannel);
    for (int c=0; c<numOfChannel; c++){
        set_pixel(sobel_filter, 0, 0, c, -1.);
        set_pixel(sobel_filter, 1, 0, c, 0.);
        set_pixel(sobel_filter, 2, 0, c, 1.);

        set_pixel(sobel_filter, 0, 1, c, -2.);
        set_pixel(sobel_filter, 1, 1, c, 0.);
        set_pixel(sobel_filter, 2, 1, c, 2.);

        set_pixel(sobel_filter, 0, 2, c, -1.);
        set_pixel(sobel_filter, 1, 2, c, 0.);
        set_pixel(sobel_filter, 2, 2, c, 1.);
    }
    return sobel_filter;
}

image make_gy_filter()
{
    int numOfChannel = 1;
    image sobel_filter = make_image(3, 3, numOfChannel);
    for (int c=0; c<numOfChannel; c++){
        set_pixel(sobel_filter, 0, 0, c, -1.);
        set_pixel(sobel_filter, 1, 0, c, -2.);
        set_pixel(sobel_filter, 2, 0, c, -1.);

        set_pixel(sobel_filter, 0, 1, c, 0.);
        set_pixel(sobel_filter, 1, 1, c, 0.);
        set_pixel(sobel_filter, 2, 1, c, 0.);

        set_pixel(sobel_filter, 0, 2, c, 1.);
        set_pixel(sobel_filter, 1, 2, c, 2.);
        set_pixel(sobel_filter, 2, 2, c, 1.);
    }
    return sobel_filter;
}

void feature_normalize(image im)
{
    // find max and min firstly
    float maxi = -INFINITY;
    float mini = +INFINITY;
    for (int i=0; i<im.w*im.h*im.c; i++) {
        maxi = fmax(im.data[i], maxi);
        mini = fmin(im.data[i], mini);
    }

    float range = maxi - mini;
    float normalizing_pixel;
    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            for (int c=0; c<im.c; c++){
                float pixel = get_pixel(im, w, h, c);
                if (range != 0.){ 
                    normalizing_pixel = (pixel - mini) / range;
                } else {
                    normalizing_pixel = 0;
                }
                set_pixel(im, w, h, c, normalizing_pixel);
            }
        }
    }
}

image *sobel_image(image im)
{
    
    image *mag_and_direct = calloc(2, sizeof(image));
    mag_and_direct[0] = make_image(im.w, im.h, 1);
    mag_and_direct[1] = make_image(im.w, im.h, 1);
    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image sobel_gx = convolve_image(im, gx, 0);
    image sobel_gy = convolve_image(im, gy, 0);

    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            for (int c=0; c<1; c++){
                float x = get_pixel(sobel_gx, w, h, c);
                float y = get_pixel(sobel_gy, w, h, c);
                set_pixel(mag_and_direct[0], w, h, 0, sqrt(x*x + y*y));
                set_pixel(mag_and_direct[1], w, h, 0, atan2(y, x));
            }
        }
    }
    return mag_and_direct;
}

image colorize_sobel(image im)
{
    image *mag_and_direct = sobel_image(im);
    image colorize_sobel = make_image(im.w, im.h, im.c);
    feature_normalize(mag_and_direct[0]);
    feature_normalize(mag_and_direct[1]);
    float S, V, H;
    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            S = get_pixel(mag_and_direct[0], w, h, 0);
            V = get_pixel(mag_and_direct[0], w, h, 0);
            H = get_pixel(mag_and_direct[1], w, h, 0);
            set_pixel(colorize_sobel, w, h, 0, H);
            set_pixel(colorize_sobel, w, h, 1, S);
            set_pixel(colorize_sobel, w, h, 2, V);
        }
    }
    hsv_to_rgb(colorize_sobel);
    return colorize_sobel;
}
