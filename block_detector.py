import sys
import cv2
import numpy as np

def get_threshold(img):
  thres = 100
  img = cv2.medianBlur(img, 5)
  ret, th = cv2.threshold(img, thres, 255, cv2.THRESH_BINARY)
  return th

def is_black_or_white(th, x, y):
# 0 refers black, 255 refers white
  if th[x][y] == 0:
    return 0
  else:
    return 1

def is_all_white(th, x=-1, y=0, square_size=0):
  if x == -1:
    return (th == 255).all()
  else:
    sqa = th[x:x+square_size, y:y+square_size]
    return (sqa == 255).all()

def has_black_px(sqa, min_amount_of_black_px):
  amount_of_black_px = np.count_nonzero(sqa==0)
  if amount_of_black_px > min_amount_of_black_px:
    return True
  else:
    return False

def is_black_beside(th, pos, square_size, min_amount_of_black_px):
  # 如果周围的框内都有黑色，则取其位置
  udlr = {}
  x, y = pos
  u = th[x-square_size:x, y:y +square_size]
  d = th[x+square_size:x+2*square_size, y:y+square_size]
  l = th[x:x+square_size, y-square_size:y]
  r = th[x:x+square_size, y+square_size:y+2*square_size]
  for n, a in zip(('u', 'd', 'l', 'r'),(u, d, l, r)):
    udlr[n] = has_black_px(a, min_amount_of_black_px)
  if list(udlr.values()).count(True) == 4:
    return True
  else:
    return False

def detect_processor(th, step_size, square_size, min_amount_of_black_px):
  row, col = th.shape
  x = y = square_size
  around_tect_sig = False
  times_no_black = 0
  detect_block_pos = []
  while True:
    pos = [x, y]
    # 移动到边界后结束
    if y + step_size > row:
      break
    # 得到本次循环的区块数据
    sqa = th[x:x+square_size,y:y+square_size]
    # 发现一定量的黑色像素开始检测
    if has_black_px(sqa, min_amount_of_black_px):
      around_tect_sig = True
      times_no_black = 0
    # 检测不到周围黑色超过一定次数后关闭检测
    if times_no_black > 10 and around_tect_sig == True:
      around_tect_sig = False
    # 如果检测信号开启，区块全白，周围全有黑色则记录位置
    if around_tect_sig == True and is_all_white(sqa):
      if is_black_beside(th, pos, square_size, min_amount_of_black_px):
        detect_block_pos.append(pos)
    # 每次循环需要更新变量
    if x + step_size <= col:
      x = x + step_size
    else:
      y = y + step_size
      x = square_size
    times_no_black += 1
  return detect_block_pos

def merge_near_rect(origin_list, square_size, outcome=[]):
# 合并相近的检测框，取平均值，递归调用
  group_pos = outcome
  group_list = [origin_list[0]]
  for pos in origin_list[1:]:
    y, x = pos
    for grouped_pos in group_list:
      Y, X = grouped_pos
      if (abs(x - X) <= square_size and abs(y - Y) <= square_size
            and pos not in group_list):
        group_list.append(pos)
  group_pos.append(list(np.mean(np.array(group_list), axis=0, dtype=np.integer)))
  rest_list = [i for i in origin_list if i not in group_list]
  if rest_list:
    merge_near_rect(rest_list, square_size, group_pos)
  return group_pos

def draw_rect(img, detect_block_pos, square_size, margin, file_name):
  for pos in detect_block_pos:
    y, x = pos
    #x += margin
    start_point = (x, y)
    end_point = (x+square_size, y+square_size)
    color = (0, 0, 255)
    thickness = 1
    img = cv2.rectangle(img, start_point, end_point, color, thickness) 
  cv2.imwrite(file_name, img)
  #cv2.imshow('t', img)
  #cv2.waitKey()

def crop_image(img, margin):
  return img[:, margin:-margin]

def position_in_origin_img(pos_list, margin):
  real_pos_list = []
  for pos in pos_list:
    pos[1] += margin
    real_pos_list.append(pos)
  return real_pos_list

def main():
  # 按像素移动框体
  # 在框内检测到黑色时才开始判断
  # 检测周围是否有黑色时应取一个值作为最小黑色像素出现值以减少干扰
  # 检测周围黑色false超过一定次数后关闭检测，直到出现一定的黑色像素
  step_size = 2 # 检测框每次移动多少像素
  square_size = 12 # 80px for phone, 15px for 1080p
  min_amount_of_black_px = 10 # just test
  margin = 450

  img_path = sys.argv[1]
  img = cv2.imread(img_path, 0)
  img = crop_image(img, margin)
  th = get_threshold(img)
  detect_block_pos = detect_processor(
    th, step_size, square_size, min_amount_of_black_px)
  if detect_block_pos:
    positions = merge_near_rect(detect_block_pos, square_size)
    #print(positions)
    positions = position_in_origin_img(positions, margin)
    img = cv2.imread(img_path)
    draw_rect(img, positions, square_size, margin, 'position.jpg')

if __name__ == "__main__":
  main()
