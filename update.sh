#!/bin/bash

git pull
shc -f grab_send_showqr.sh -o sbk
bash test.sh