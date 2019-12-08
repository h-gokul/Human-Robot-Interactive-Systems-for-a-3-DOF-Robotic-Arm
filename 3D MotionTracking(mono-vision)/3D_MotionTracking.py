import serial
import struct
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time

arduino= serial.Serial('com9',9600)

x=0
y=0
x1=0
y1=0
radius=0
rad=0
rad1=0


    
    



ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
        help="path to the video file")
ap.add_argument("-b", "--buffer", type=int, default=32,
        help="max buffer size")
args = vars(ap.parse_args())


redLower = (0, 191, 110)
redUpper = (10, 255, 255)

rad=0
direct=0
pts = deque(maxlen=args["buffer"])
pts1 = deque(maxlen=args["buffer"])
counter = 0
counter1 = 0
(dX, dY)= (0, 0)
(dX1,dY1)=(0,0)
(dx,dy)=(0,0)
direction = ""
direction1 = ""
close = ""
vs = VideoStream(src=0).start()


time.sleep(2.0)

while True:
    frame = vs.read()
    
    frame= frame[1] if args.get("video", False) else frame

    if frame is None:
        break

    frame = imutils.resize(frame, width=600, height=400)
    blurred = cv2.GaussianBlur(frame, (11,11), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv, redLower, redUpper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    center = None

    

    if len(cnts) > 0:

        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center= (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        if radius > 10:

            cv2.circle(frame, (int(x), int(y)), int(radius),
                    (0, 255, 255), 2)
            cv2.circle(frame, center, 5, (0, 0, 255), -1)
            pts.appendleft(center)
            cv2.circle(frame, (300,200), 5, (0, 255, 0), -1)
            

    

    for i in np.arange(1, len(pts)):

        if pts[i-1] is None or pts[i] is None:
            continue

        if counter >= 10 and i == 1 and pts[-10] is not None:

            dX = pts[-10][0] - pts[i][0]
            dY = pts[-10][1] - pts[i][1]
            (dirX, dirY) = ("", "")
            
            if np.abs(dX) > 20:
                dirX="Right" if np.sign(dX) == 1 else "Left"

            if np.abs(dY) > 20:
                dirY="Up" if np.sign(dY) == 1 else "Down"

            if dirX != "" and dirY != "":
                direction = "{}-{}".format(dirY,dirX)

            else:
                direction = dirX if dirX != "" else dirY

        thickness = int(np.sqrt(args["buffer"] / float(i+1)) * 2.5)
        cv2.line(frame, pts[i-1], pts[i], (0,0,255), thickness)

    
    x2=int(x*0.2)
    y2=int(y*0.3)+40
    
    rad=radius
    if radius>90:
        rad=90
    if radius<20 :
        rad=20
    if(rad<=90 and rad>=20) :
        rad1=140-int(rad*0.6)
     
    print(x2,y2,rad1)
    cv2.putText(frame, "dir: {}".format(direction),
            (10,30), cv2.FONT_HERSHEY_SIMPLEX,
            0.65, (0,0,255), 3)
    
    
    cv2.putText(frame, "x: {},y: {} ,rad: {}".format(x2, y2,radius),
            (10, frame.shape[0] -10), cv2.FONT_HERSHEY_SIMPLEX,
            0.35, (0, 0, 255), 1)
    
    d= struct.pack('>BBB',x2,y2,rad1)
    #print(d)
    arduino.write(d)
    
    
    cv2.imshow("FRAME", frame)
    
    key = cv2.waitKey(1) & 0xFF
    counter += 1
    

    if key == ord("q"):
        break

if not args.get("video", False):
    vs.stop()
    
else:
    vs.release()
    

cv2.destroyAllWindows()


    

                
                
        
