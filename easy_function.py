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
    if np.mean(a) < 55:
      blk_lst.append(1)
    else:
      blk_lst.append(0)
  return blk_lst

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
