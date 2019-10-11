# -*- coding: utf-8 -*-

import numpy as np

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

def blk_lst_maker(rgb_arr):
  blk_lst = []
  for a in rgb_arr:
    blk_count = 0
    for px in a:
      if sum(px) < 55 * 3:
        blk_count += 1
    if blk_count > 20:
      blk_lst.append(1)
    else:
      blk_lst.append(0)
  return blk_lst

def is_black_in_center(blk_lst):
  half_lst = [
    blk_lst[:len(blk_lst)//2],
    blk_lst[len(blk_lst)//2:]
  ]
  edge_count1 = 0
  for clr in half_lst[0]:
    cng = False
    if clr == 0 and cng == False:
      edge_count1 += 1
    elif clr == 1 and cng == False:
      cng = True
    elif clr == 0 and cng == True:
      return False

  edge_count2 = 0
  for clr in half_lst[1]:
    cng = False
    if clr == 1 and cng == False:
      edge_count2 += 1
    elif clr == 0 and cng == False:
      cng == True
    elif clr == 1 and cng == True:
      return False
  
  if (is_similar(edge_count1, edge_count2) and 
        edge_count1 + edge_count2 < 50):
    return True
  else:
    return False

def is_similar(arg1, arg2):
  if abs(arg1 - arg2) < 10:
    return True
  else:
    return False

'''
def find_block(im):
  outbox_size = (25, 75)
  blkbox_size = (25, 21)
  nonblkbox_size = (25, 28)

def is_black_block(src_lst, first_find=False):
  src_lst = np.array(src_lst)
  for rgb in src_lst:
    if rgb.all() < 20:
      return True
'''
