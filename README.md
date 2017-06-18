# VideoThumbnail

I created this project, because I used an existing thumbnail generator, but it could not be run on a Raspberry Pi.
If you're looking for a thumbnail generator, I would highly recommend [this one](http://moviethumbnail.sourceforge.net/).

I innitialy wrote a script that just used ffmpeg, but for some odd reason the seeking would takes multiple minutes on my Raspberry Pi.

Using the old script, it would take **~45 _minutes_** to generate the thumbnails.
This project only needs **~30 _seconds_**.

I'm not planning on making alot of changes to this code. If you need different dimensions or other commandline options, you could probably figure it out yourself. The code is not that complicated

Example:
--------

```
VideoThumbnail "Kingsman The Secret Service (2015).mp4"
```
Output:
![Example Thumbnail](http://i.imgur.com/6Aj4Cdf.jpg)





Requirements:
-------------
You will need OpenCV. (I used [this guide](https://www.youtube.com/watch?v=l4372qtZ4dc) for windows, and [this guide](http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/) for my Raspberry Pi 3)

Compiling on Raspberry Pi
-------------------------
```
g++ -std=c++0x $(pkg-config --libs --cflags opencv) -o VideoThumbnail Main.cpp
```

