#!/bin/bash
make
echo "Copying binaries to user's lv2 directory... done!"
yes | cp -rf ./bin/noise-repellent.lv2 $HOME/.lv2
make clean