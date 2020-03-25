#!/bin/bash

git pull
git checkout xinzhi
apt-get -y install python-opencv
pip install pyzbar
shc -f grab_send_showqr.sh -o sbk
bash test.sh