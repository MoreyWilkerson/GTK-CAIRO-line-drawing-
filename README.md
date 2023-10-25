# Line_git

GTK 3.0 canvas that draws multiple lines through the use of nested linked lists. Click with the left button on the mouse to draw lines, click with the right to stop.  

https://github.com/MoreyWilkerson/GTK-CAIRO-line-drawing-/assets/95983976/3c957fa0-ce74-40fd-97f1-721d9217d6b5

##LS/linux compiling

```bash
gcc -Wno-format -o motrk motrk.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
```

alternatively, you can type in 
```bash
./compile.script
```
while in the folder directory of this project.

## Windows compiling:

With windows users , I recommend checking out WSL 2 - it allows you to use linux compiling without getting rid of windows. For whatever reason; getting GTK to work with windows is, well, a nightmare. if you DO want to do this, check the GTK website for proper implementation, and not that the Glade files will be treated as building files. 

Their is plenty of how to videos for setting up WSL 2 online. 

##learning GTK

If you are trying to learn GTK, I highly recomend kevin o' kanes glade videos on youtube. very informative, and sorts out the mess of GTK one video at a time. 
https://www.cs.uni.edu/~okane/youtube.html

##Known bugs 

None. if any are seen please shoot me a message. 

## Future plans
- Change Clear button such that it can delete a point it is near 

##tags 
GTK, Glade, Canvas, Cairo, Drawing, Mspaint