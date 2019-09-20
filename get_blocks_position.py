# -*- coding: utf-8 -*-

from __future__ import print_function
import sys
import numpy as np
from PIL import Image, ImageDraw
from easy_function import std, make_int_aver, blk_lst_maker
from src_data import boxesT as boxes

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

def edge_detect(rgb_lst):
  """Detect edge from the rgb list.

  Parameters:
    rgb_lst: A list contain rgb data.

  Returns:
    A boolean that if has edge in the region.
  """
  '''edge_info = edge_pos(rgb_lst)
  if 1 in edge_info['black_row'] or 1 in edge_info['black_col']:
    return True
  else:
    return False'''
  rgb_aver = fetch_aver_rgb(rgb_lst)
  unnormal_px = 0
  for rgb in rgb_lst:
    sub = np.subtract(rgb, rgb_aver)
    if sum(sub) > 25:
      unnormal_px += 1
  return True if unnormal_px > 14 else False

''' TODO: Need update
def pos_check(im):  
  """Check if the position is proper.

  Parameters:
    im: An image object.

  Returns:
    Return a boolean if the black block is in the proper place.
  """
  box = (935, 120, 40, 10)
  return True if sum(get_main_color(im, box)) < 45 else False'''

def edge_pos(rgb_lst):
  rgb_arr = np.array(rgb_lst)
  rgb_arr_C = np.reshape(rgb_arr, (10, 14, 3), order='C')
  rgb_arr_F = np.reshape(rgb_arr, (14, 10, 3), order='F')
  blk_row = blk_lst_maker(rgb_arr_C)
  blk_col = blk_lst_maker(rgb_arr_F)
  udlr = [
    blk_row[:len(blk_row)//2],
    blk_row[len(blk_row)//2:],
    blk_col[:len(blk_col)//2],
    blk_col[len(blk_col)//2:]
  ]
  edge_pos = []
  for p in udlr:
    if p.count(1) == 0:
      edge_pos.append(0)
    elif p.count(1) == len(p):
      edge_pos.append(2)
    else:
      edge_pos.append(1)

  try:
    first_blk_row = blk_row.index(1)
  except ValueError:
    first_blk_row = 0
  try:
    first_blk_col = blk_col.index(1)
  except ValueError:
    first_blk_col = 0
  try:
    last_blk_row = len(blk_row) - 1 - blk_row[::-1].index(1)
  except ValueError:
    last_blk_row = 0
  try:
    last_blk_col = len(blk_col) - 1 - blk_col[::-1].index(1)
  except ValueError:
    last_blk_col = 0

  return {
    'black_row': blk_row,
    'black_col': blk_col,
    'edge_position': edge_pos,
    'first_black_row': first_blk_row,
    'first_black_col': first_blk_col,
    'last_black_row': last_blk_row,
    'last_black_col': last_blk_col
  }

def position_adjustment(im, boxes):
  rgb_lst0 = get_rgb_in_square(im, boxes[0])
  edge_info = edge_pos(rgb_lst0)
  #print(edge_info)
  new_boxes = []
  for box in boxes:
    box = list(box)
    if (sum(edge_info['edge_position'][:2]) >= 2 or 
        sum(edge_info['edge_position'][2:]) >= 2):
      print('appear 2')
      return False 
    if edge_info['edge_position'][0] == 1:
      box[1] = box[1] + 12 + edge_info['last_black_row']
    if edge_info['edge_position'][1] == 1:
      box[1] = box[1] - 12 - (10 - edge_info['first_black_row'])
    if edge_info['edge_position'][2] == 1:
      box[0] = box[0] + 14 + edge_info['last_black_col']
    if edge_info['edge_position'][3] == 1:
      box[0] = box[0] - 14 - (14 - edge_info['first_black_col'])
    new_boxes.append(box)
  return new_boxes

def draw_rect(im, boxes, fn):
  for box in boxes:
    draw = ImageDraw.Draw(im)
    x, y, w, h = box
    draw.rectangle(((x, y), (w+x, h+y)), outline='red')
    del draw
  im = im.crop((842, 82, 1044, 994))
  im.save(fn+'.png', "PNG")
  return im

def get_right_block_position(im, boxes):
  im1 = im.copy()
  im2 = im.copy()
  im1 = draw_rect(im1, boxes, sys.argv[2])
  #del im1
  if edge_detect(get_rgb_in_square(im, boxes[0])):
    adjusted_boxes = position_adjustment(im, boxes)
  else:
    adjusted_boxes = boxes
  #print(adjusted_boxes)
  if adjusted_boxes:
    im2 = draw_rect(im2, adjusted_boxes, sys.argv[2]+'_调整后')
    im2.show('2')
    #del im2
    mca = get_rgb_in_square(im, adjusted_boxes[13])
    mca_block = edge_pos(mca)
    #print(mca_block)
    if 1 in mca_block['black_row'] or 1 in mca_block['black_col']:
      print('need adjust block distance')
      return False
    else:
      return adjusted_boxes
  else:
    im1.show('1')
    return False

def main():
  im = Image.open(sys.argv[1]).convert('RGB')
  block_pos = get_right_block_position(im, boxes)
  if block_pos:
    print(block_pos)
    of = open('block_pos.txt', 'w')
    of.write(str(block_pos))
  else:
    of = open('block_pos.txt', 'w')
    of.write("0")
  
if __name__ == "__main__":
  main()
