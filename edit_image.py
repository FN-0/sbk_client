# -*- coding: utf-8 -*-
# !/usr/bin/env python

import sys
from PIL import Image

for image in sys.argv[1:]:
  img = Image.open(image)
  transposed  = img.transpose(Image.ROTATE_180)
  transposed.save('image.png')
