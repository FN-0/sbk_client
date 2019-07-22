# -*- coding: utf-8 -*-
# !/usr/bin/env python

import RPi.GPIO as GPIO
import time

## 控制转动
def rotation(pin1, pin2, t):
    GPIO.output(pin1, GPIO.HIGH)
    GPIO.output(pin2, GPIO.LOW)
    time.sleep(t)
    GPIO.output(pin1, GPIO.LOW)
'''
## 开关闭合的处理
def on_switch_pressed():
    rotation(12, 11, 5)
## 设置GPIO输入模式, 使用GPIO内置的上拉电阻, 即开关断开情况下输入为HIGH
GPIO.setup(13, GPIO.IN, pull_up_down = GPIO.PUD_UP)
## 检测HIGH -> LOW的变化
GPIO.add_event_detect(13, GPIO.FALLING, bouncetime = 200)
'''
try:
    GPIO.setmode(GPIO.BOARD)
    ## 设置引脚输出
    GPIO.setup(12, GPIO.OUT)
    GPIO.setup(18, GPIO.OUT)
    rotation(12, 18, 4)
    '''
    while True:
        # 如果检测到电平FALLING, 说明开关闭合
        if GPIO.event_detected(13):
            on_switch_pressed()
        # 可以在循环中做其他检测
        time.sleep(0.01)     # 10毫秒的检测间隔
    '''
    time.sleep(4)
    rotation(18, 12, 4)
except Exception as e:
    print(e)

## 清除
GPIO.cleanup()
