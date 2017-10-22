# import the necessary packages
from picamera.array import PiYUVArray
from picamera import PiCamera
import time
import cv2
from math import atan
import numpy as np

offset = 0.2
white = 100
black = 100
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.crop = (0.0,0.0+offset,1.0,0.0667)
camera.sensor_mode = 7
camera.framerate = 60
rawCapture = PiYUVArray(camera, size=(640, 16))
#rawCapture = PiYUVArray(camera, size=(640, 480))
# allow the camera to warmup
time.sleep(2)
start = time.time()
count = 0
# capture frames from the camera
atans = np.arange(-320.,320.,1)
for x in xrange (atans.shape[0]):
	atans[x] = atan(0.0043261*(320-x))
for frame in camera.capture_continuous(rawCapture, format='yuv', use_video_port=True, resize=(640,16)):
#for frame in camera.capture_continuous(rawCapture, format='yuv', use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
	if count == 100:
		print "FPS:{0}".format(100/(time.time()-start))
		start = time.time()
		count = 0
	count=count+1
	image = frame.array[:,:,0]
	line = image [8:15, 0:640]
	lineb = cv2.medianBlur(line,7)
 
#	for x in xrange(lineb.shape[1]):
#		if lineb.item(3,x) > white :
#			lineb.itemset((3,x),255)
#			lineb [3,x] = 255
#		elif lineb.item(3,x) < black:
#			lineb.itemset((3,x),0)
#			lineb [3,x] = 0
#		else :
#			lineb.itemset((3,x),(lineb.item(3,x) - black) * 255/(white-black))
#			lineb [3,x] = (lineb [3,x] - black) * 255/(white-black)

	lineb[3,:][lineb[3,:] > white] = 255
	lineb[3,:][lineb[3,:] <= black] = 0
# 	np.clip(lineb[3,:], black, white, lineb[3,:])
	cv2.imwrite("LineB.jpg", lineb)
#	print lineb[3,:]
#	print atans

	results = (lineb[3,:]-255)*atans 
	error = np.sum(results)
		
#	print error
	# show the frame
#	cv2.imwrite("Line.jpg", line)

#	key = cv2.waitKey(1) & 0xFF
 
	# clear the stream in preparation for the next frame
	rawCapture.truncate(0)
# 	break
	
