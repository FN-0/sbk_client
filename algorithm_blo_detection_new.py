import sys
import cv2
import math
import json
import numpy as np
from PIL import Image
from block_detector import unsharp_mask, qr_detector, update_radian
from block_detector import update_scale, get_point_by_radian
from block_position import relative_position_photo

BLO = (
  (252, 255, 236),
  (207, 247, 238),
  (147, 202, 190),
  (98, 162, 167)
)

rst_lst = {
  'BLO': ('-', '+', '++', '+++')
}

def std(lst, x):
  """Calculate the standard deviation with the list and x.

  Parameters:
    lst: A list of tuple.
    x: A number given to be calculated the deviation with the list.

  Returns:
    A float number.
  """
  if len(lst) != 0:
    return np.sqrt(np.mean(abs(np.array(lst) - x)**2))
  else:
    print('The lenght of list is 0.')

def make_int_aver(lst):
  """Calculate the average of the list and round.
 
  Parameters:
    lst: A flat list that only contains number.

  Returns:
    An integer.
  """
  if len(lst) != 0:
    return round(sum(lst)/len(lst))
  else:
    print('The lenght of list is 0.')
    
def color_approximation(src, dst):
  src, dst = map(np.array, (src, dst))
  x = np.dot(src, dst)/(np.sqrt(sum(src**2))*np.sqrt(sum(dst**2)))
  r = np.arccos(x)*(180/np.pi)
  return r

def distance(c1, c2):
  (r1,g1,b1) = c1
  (r2,g2,b2) = c2
  return math.sqrt((r1 - r2)**2 + (g1 - g2) ** 2 + (b1 - b2) **2)

def most_approx(src_color, dst_color):
  approx = [distance(src_color, dst) for dst in dst_color]
  return approx.index(min(approx))

def approx_seq(src_color, dst_color):
  approx = [distance(src_color, dst) for dst in dst_color]
  return [x for _,x in sorted(zip(approx,[0,1,2,3]))]

def get_rgb_in_square(im, box):
  """Get rgb data from the square region.

  Parameters:
    im: An image object.
    box: A square region with (posx, posy, sizex, sizey).

  Returns:
    A list contain rgb data. For example:
    [ (157, 152, 156), (157, 152, 156), (157, 152, 156),
      (157, 152, 156), (157, 152, 156), (157, 152, 156) ]
  """
  posx, posy, sizex, sizey = box
  #region = im.crop(box)
  lst = []
  for y in range(posy, posy+sizey):
    for x in range(posx, posx+sizex):
      #print(x,y)
      r, g, b = im.getpixel((x, y))
      rgb = (r, g, b)
      lst.append(rgb)
  return lst

def fetch_aver_rgb(lst):
  """Calculate the average rgb of the rgb list.

  Parameters:
    lst: A list contain rgb data. For example:
    [ (157, 152, 156), (157, 152, 156) ]

  Returns:
    A tuple of average rgb.
  """
  if len(lst) != 0:
    r, g, b = [ [item[i] for item in lst] for i in range(len(lst[0])) ]
    return (make_int_aver(r), make_int_aver(g), make_int_aver(b))
  else:
    print('The lenght of list is 0.')

def get_main_color(im, box):
  """Get the main color of the region.

  Parameters:
    im: An image object.
    box: Four values of position and size.

  Returns:
    Main rgb value of the region.
  """
  color_lst = get_rgb_in_square(im, box)
  if color_lst == 0:
    print('No color data.')
    return 0
  r, g, b = [[item[i] for item in color_lst] for i in range(len(color_lst[0]))]
  
  rmax, gmax, bmax = map(max, (r, g, b))
  #print(rmax, gmax, bmax)
  std_rm = std(r, rmax)
  std_gm = std(g, gmax)
  std_bm = std(b, bmax)
  #print((std_rm, std_gm, std_bm))
  
  ravg, gavg, bavg = map(make_int_aver, (r, g, b))
  #print(ravg, gavg, bavg)
  std_ra = std(r, ravg)
  std_ga = std(g, gavg)
  std_ba = std(b, bavg)
  #print((std_ra, std_ga, std_ba))
  rmain = rmax if std_ra > std_rm else ravg
  gmain = gmax if std_ga > std_gm else gavg
  bmain = bmax if std_ba > std_bm else bavg
  return (rmain, gmain, bmain)

def read_boxes_rgb(img, boxes):
  rgbs = []
  for box in boxes:
    c = get_main_color(img, box)
    #print(c)
    rgbs.append(c)
  return tuple(rgbs)

def get_position(img_path, relative_position_1, SCALE, RADIAN, src_point):
  img = cv2.imread(img_path)
  points = []
  for block, rd_value in relative_position_1.items():
    point = get_point_by_radian(rd_value['dist']*SCALE[1], rd_value['rad']+RADIAN, src_point)
    point = tuple([int(round(x)) for x in point])
    points.append(point)
  return points

def pos2box(pos_list, sizex, sizey):
  box_list = []
  for pos in pos_list:
    box = []
    box.extend([pos[0]-(sizex//2), pos[1]-(sizey//2), sizex, sizey])
    box_list.append(box)
  return box_list

def draw_rect(img_path, box_list):
  img = cv2.imread(img_path)
  for box in box_list:
    x1, y1 = box[0], box[1]
    x2, y2 = box[0]+box[2], box[1]+box[3]
    cv2.rectangle(img, (x1, y1), (x2, y2), (0,0,255), 1)
  cv2.imwrite('test.jpg', img)

def results2blocks(results, f):
  f.write(str(results[:10])+'\n')
  #f.write(str(results[10:14])+'\n')
  #f.write(str(results[14:21])+'\n')
  #f.write(str(results[21:23])+'         '+str(results[23:25])+'\n')
  #f.write(str(results[25:27])+'         '+str(results[27:29])+'\n')
  #f.write(str(results[29:31])+'         '+str(results[31:33])+'\n')
  #f.write(str(results[33:35])+'         '+str(results[35:37])+'\n')
  #f.write(str(results[37:39])+'         '+str(results[39:41])+'\n')
  #f.write(str(results[41:48])+'\n')
  #f.write(str(results[48:55])+'\n')
  #f.write(str(results[55:62])+'\n')

def results2report(results):
  blo_data = rst_lst['BLO'][max(results)]
  blo_json = json.dumps({'BLO':blo_data})
  return blo_json

def main():
  # data initialize
  DIST = [DIST_Y, DIST_X] = (315, 366.5)
  SCALE = [SCALE_Y, SCALE_X] = (1, 1)
  RADIAN = 0
  STEP_SIZE = 30
  SQUARE_SIZE = 170

  img_path = sys.argv[1]
  img = cv2.imread(img_path)
  #img = unsharp_mask(img)
  posn = qr_detector(img, STEP_SIZE, SQUARE_SIZE)
  print(posn)

  f = open('/home/pi/sbk_client/res.txt', 'w')
  #f = open('res.txt', 'w')
  f.write(str(posn)+'\n')

  if len(posn) == 4:
    square_size = 4
    img = Image.open(img_path).convert('RGB')
    SCALE = update_scale(posn, DIST)
    RADIAN = update_radian(posn[b'1'], posn[b'2'])
    f.write(str(RADIAN)+'\n')
    pos_list = get_position(img_path, relative_position_photo, SCALE, RADIAN, posn[b'1'])
    if pos_list:
      box_list = pos2box(pos_list, square_size, square_size)
      draw_rect(img_path, box_list)
      rgb_data = read_boxes_rgb(img, box_list)
      print(rgb_data)
      f.write(str(rgb_data)+'\n')
      results = []
      for val in rgb_data:
        results.append(most_approx(val, BLO))
        print(approx_seq(val, BLO))
        f.write(str(approx_seq(val, BLO))+'\n')
      results2blocks(results, f)
      f.write(str(results2report(results)))
  f.close()
  print('Done.')

if __name__ == "__main__":
  main()
