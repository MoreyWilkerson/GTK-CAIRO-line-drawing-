# line_git

GTK canvas that draws multiple lines through the use of nested linked lists. Click with the left button on the mouse to draw lines, click with the right to stop.  

<video width="320" height="240" controls>
  <source src="working_line_system2.mp4" type="video/mp4">
</video>

##LS/linux compiling

```bash
gcc -Wno-format -o motrk motrk.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
```

alternatively, you can type in 
```bash
./compile.script
```
while in the folder directory of this project.

##learning GTK

If you are trying to learn GTK, I highly recomend kevin o' kanes glade videos on youtube. very informative, and sorts out the mess of GTK one video at a time. 
https://www.cs.uni.edu/~okane/youtube.html

##Known bugs 

- When the button is pressed, then the right mouse is clicked without drawing a line; a seg fault occurs. this can be fixed with some if then clauses 

## Future plans
- add a button for deleting the previous line drawn. Button is to free memory used by line. 

- add a feature that when control is press, line is locked to only degrees of 30,45,60,90, and so on and cannot deviate from this. 

##tags 
GTK, Glade, Canvas, Cairo, Drawing, Mspaint