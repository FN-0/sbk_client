import sys
import cv2
import json
import numpy as np
from PIL import Image
from block_detector import crop_image, get_threshold, detect_processor
from block_detector import merge_near_rect, position_in_origin_img

BLO = (
    (151, 162, 149),
    #(148, 111, 45),
    (40, 86, 60),
    (16, 50, 29),
    (2, 25, 22)
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

def most_approx(src_color, dst_color):
  approx = [color_approximation(src_color, dst) for dst in dst_color]
  return approx.index(min(approx))

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
  region = im.crop(box)
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

def get_position(img_path, square_size):
  step_size = 2
  min_amount_of_black_px = 10
  margin = 450

  img = cv2.imread(img_path, 0)
  img = crop_image(img, margin)
  th = get_threshold(img)
  detect_block_pos = detect_processor(
    th, step_size, square_size, min_amount_of_black_px)
  if detect_block_pos:
    positions = merge_near_rect(detect_block_pos, square_size)
    positions = position_in_origin_img(positions, margin)
    return positions

def pos2box(pos_list, sizex, sizey):
  box_list = []
  for pos in pos_list:
    box = []
    box.extend([pos[1], pos[0], sizex, sizey])
    box_list.append(box)
  return box_list

def results2report(results):
  blo_data = rst_lst['BLO'][max(results)]
  blo_json = json.dumps({'BLO':blo_data})
  return blo_json

def main():
  img_path = sys.argv[1]
  square_size = 12
  img = Image.open(img_path).convert('RGB')
  pos_list = get_position(img_path, square_size)
  if pos_list:
    box_list = pos2box(pos_list, square_size, square_size)
    rgb_data = read_boxes_rgb(img, box_list)
    results = []
    for val in rgb_data:
      results.append(most_approx(val, BLO))
    #print(results)
    with open('/home/pi/sbk_client/res.txt', 'w') as f:
      f.write(results2report(results))
    f.close()

if __name__ == "__main__":
  main()
