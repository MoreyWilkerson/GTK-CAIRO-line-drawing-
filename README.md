# Line_git

GTK canvas that draws multiple lines through the use of nested linked lists. Click with the left button on the mouse to draw lines, click with the right to stop.  

https://github.com/MoreyWilkerson/Mohrs/assets/95983976/f2b6153c-77fd-4a3a-a5fa-ad7d3cf59818

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

None currently, let me know if any are found. 

## Future plans
- Change Clear button such that it can delet a point it is near 

- Allow for "snapping" I.E. if you get near a point the point locks in place. 

- add a feature that when control is press, line is locked to only degrees of 30,45,60,90, and so on and cannot deviate from this. 

##tags 
GTK, Glade, Canvas, Cairo, Drawing, Mspaint