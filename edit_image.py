# -*- coding: utf-8 -*-
# !/usr/bin/env python

import sys
from PIL import Image

for image in sys.argv[1:]:
  img = Image.open(image)
  cropped = img.crop((1050, 0, 1400, 1080))  # (left, upper, right, lower)
  cropped.save(image)
