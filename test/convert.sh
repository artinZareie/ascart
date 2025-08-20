#!/bin/bash

for jpg_file in *.jpg; do
    base_name="${jpg_file%.*}"
    magick "$jpg_file" -colorspace Gray -compress none "${base_name}.pgm"
done
