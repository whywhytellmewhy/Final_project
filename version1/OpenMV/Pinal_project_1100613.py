# Edge Impulse - OpenMV Image Classification Example

import pyb,sensor, image, time, os, tf, math

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.

#added-------------------------------------------------------------------------
EXPOSURE_TIME_SCALE = 1.25 #1.0
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...

#sensor.set_auto_gain(False)
#sensor.set_auto_whitebal(False)
# 需要让以上设置生效
sensor.skip_frames(time = 500)

current_exposure_time_in_microseconds = sensor.get_exposure_us()

sensor.set_auto_exposure(False, \
    exposure_us = int(current_exposure_time_in_microseconds * EXPOSURE_TIME_SCALE))
#------------------------------------------------------------------------------

net = "trained.tflite"
labels = [line.rstrip('\n') for line in open("labels.txt")]

clock = time.clock()

#---------------------------AprilTag----------------------------
f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)

def degrees(radians):
   return (180 * radians) / math.pi
#----------------------------------------------------------------

#added-------------------------------------------------------------------------
uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
#------------------------------------------------------------------------------

while(True):
    clock.tick()

    img = sensor.snapshot()

    # default settings just do one detection... change them to search the image...
    #for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
    for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
#        print("**********\nPredictions at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
        img.draw_rectangle(obj.rect())
        # This combines the labels and confidence values into a list of tuples
        predictions_list = list(zip(labels, obj.output()))

#        for i in range(len(predictions_list)):
#            print("%s = %f" % (predictions_list[i][0], predictions_list[i][1]))

    #print(clock.fps(), "fps")

#    print(max(obj.output()))
#    print(labels[obj.output().index(max(obj.output()))])
    if(max(obj.output())>0.75):
        print("This is : ",labels[obj.output().index(max(obj.output()))])
    #uart.write(("a" + labels[obj.output().index(max(obj.output()))]+"\r\n").encode())
        if(labels[obj.output().index(max(obj.output()))]!='u'):
            uart.write((labels[obj.output().index(max(obj.output()))]+"\r\n").encode ())
    #sensor.skip_frames(time = 100)#250

#--------------------------AprilTag---------------------------
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        # The conversion is nearly 6.2cm to 1 -> translation
        print_args = (tag.x_translation(), tag.y_translation(), tag.z_translation(), \
                degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation()))
        # Translation units are unknown. Rotation units are in degrees.

        #uart.write(("Tx: %f, Ty %f, Tz %f, Rx %f, Ry %f, Rz %f" % print_args).encode())
        print("ID %d\r\n" % tag.id())
        uart.write(("ID %d\r\n" % tag.id()).encode())
#--------------------------------------------------------------
