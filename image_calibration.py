import cv2
import sys
import numpy as np

args = np.load('cameramtx&distcoef.npz')
mtx, dist = args['arr_0'], args['arr_1']

img = cv2.imread(sys.argv[1])
h, w = img.shape[:2]
newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))

# undistort
dst = cv2.undistort(img, mtx, dist, None, newcameramtx)

# crop the image
x,y,w,h = roi
dst = dst[y:y+h, x:x+w]
cv2.imwrite('calibresult.png',dst)
