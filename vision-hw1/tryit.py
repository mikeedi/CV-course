from uwimg import *

1. Getting and resizing image with nn interpolation
im = load_image("data/dogsmall.jpg")
im_resized = nn_resize(im, im.w*4, im.h*4)
save_image(im_resized, "dog_nn_resized")

# 2. Getting and resizing image with bilinear interpolation
im = load_image("data/dogsmall.jpg")
im_resized = bilinear_resize(im, im.w*4, im.h*4)
save_image(im_resized, "dog_bil_resized")

# 3. convolution blur
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

# 4. Blur and Resize
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(thumb, "dogthumb")

# 5. Highpass
im = load_image("data/dog.jpg")
f = make_highpass_filter()
highpass = convolve_image(im, f, 0)
save_image(highpass, "dog_highpass")

# 5. Sharpen
im = load_image("data/dog.jpg")
f = make_sharpen_filter()
sharpen = convolve_image(im, f, 1)
save_image(sharpen, "dog_sharpen")

# 5. Emboss
im = load_image("data/dog.jpg")
f = make_emboss_filter()
emboss = convolve_image(im, f, 1)
save_image(emboss, "dog_emboss")