#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

int clip_value(int v, int min, int max){
    // clip value v
    assert(min <= max);
    if (v < min) v = min;
    if (v >= max) v = max - 1;
    return v;
}

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    // use clamp padding strategies
    c = clip_value(c, 0, im.c);
    y = clip_value(y, 0, im.h);
    x = clip_value(x, 0, im.w);

    return im.data[x + im.w*y + im.w*im.h*c];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    im.data[x + im.w*y + im.w*im.h*c] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    for (int c=0; c<im.c; c++){
        for (int h=0; h<im.h; h++){
            for (int w=0; w<im.w; w++){
                float v = get_pixel(im, w, h, c);
                set_pixel(copy, w, h, c, v);
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float R = get_pixel(im, w, h, 0);
            float G = get_pixel(im, w, h, 1);
            float B = get_pixel(im, w, h, 2);
            float v_gray = 0.299*R + 0.587*G + 0.114*B;
            set_pixel(gray, w, h, 0, v_gray);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float value = get_pixel(im, w, h, c);
            set_pixel(im, w, h, c, value+v);
        }
    }
}

void scale_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float value = get_pixel(im, w, h, c);
            set_pixel(im, w, h, c, value*v);
        }
    }
}

void clamp_image(image im)
{

    for (int c=0; c<im.c; c++){
        for (int h=0; h<im.h; h++){
            for (int w=0; w<im.w; w++){
                float v = get_pixel(im, w, h, c);
                v = v > 1.0 ? 1.0 : v;  
                v = v < 0.0 ? 0.0 : v; 
                set_pixel(im, w, h, c, v);
            }
        }
    }    
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float S, V, H;
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float R = get_pixel(im, w, h, 0); 
            float G = get_pixel(im, w, h, 1); 
            float B = get_pixel(im, w, h, 2);

            // compute Value
            V = three_way_max(R, G, B);

            // compute Saturation
            float m = three_way_min(R, G, B);
            float C = V - m;
            if (V > 0){
                S = C / V;
            } else {
                S = 0;
            } 

            // compute Hue
            float H_hatch = 0;
            if (C == 0) {
               H = 0; 
            } else {
                if (V == R) H_hatch = (G - B) / C;
                if (V == G) H_hatch = (B - R) / C + 2;
                if (V == B) H_hatch = (R - G) / C + 4;
                H = H_hatch / 6.;
                if (H_hatch < 0) H = H + 1;
            }
            set_pixel(im, w, h, 0, H);
            set_pixel(im, w, h, 1, S);
            set_pixel(im, w, h, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{

    // TODO Fill this in
    float R, G, B;
    for (int h=0; h<im.h; h++){
        for (int w=0; w<im.w; w++){
            float H = get_pixel(im, w, h, 0); 
            float S = get_pixel(im, w, h, 1); 
            float V = get_pixel(im, w, h, 2);

            float C = S * V;
            float m = V - C;

            float H_ = H * 6.;
            float X = C * (1 - fabs(fmod(H_, 2) - 1)); 

            if (0. <= H_ && H_ <=1.){
                R = C;
                G = X;
                B = 0;
            } else if(1. <= H_ && H_ <= 2.){
                R = X;
                G = C;
                B = 0;
            } else if(2. <= H_ && H_ <= 3.){
                R = 0;
                G = C;
                B = X;
            } else if(3. <= H_ && H_ <= 4.){
                R = 0;
                G = X;
                B = C;
            } else if(4. <= H_ && H_ <= 5.){
                R = X;
                G = 0;
                B = C;
            } else if(5. <= H_ && H_ <= 6.){
                R = C;
                G = 0;
                B = X;
            } else {
                R = 0;
                G = 0;
                B = 0;
            }
            R = R + m;
            G = G + m;
            B = B + m;
            set_pixel(im, w, h, 0, R);
            set_pixel(im, w, h, 1, G);
            set_pixel(im, w, h, 2, B);

        }
    }
}
