//GOAL: allow for multiple linked lists. 
// program works quite well 

#include <string.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>

GtkWidget       *window;
GtkWidget       *fixed1;
GtkWidget       *draw1;
GtkWidget       *button1;
GtkWidget       *clear;
GtkWidget       *label1;
GtkWidget       *label2;
GtkWidget       *label3;
GtkWidget       *snaptgl;

GtkBuilder		  *builder; 

static void draw_brush (GtkWidget *widget, gdouble    x, gdouble    y);

void snapchange(GtkWidget *widget, cairo_t *cr); // allows mouse to "Snap" to points 
void clearline();
void straightline(); 


//points along the "chain"
struct Point {
	int x;
	int y;
	struct Point *next;
} *p1, *p2, *start; 

//end point - moves with mousclick
struct {
  int coordx;
  int coordy;
  int snapcoordx;// snaps store the location of the snap. 
  int snapcoordy; 
  bool snap; // will change the location if it detects a snap 
  bool cnrl; // true on control button press 
} gend; //note: this is not a pointer, while every other struct declaration is. thanks, I hate it. 

struct lines {
  struct Point *head; // starting pointer 
  struct lines *next; // next in line
} *heads, *heads2, *lnstart; 

void		  on_destroy(); 
bool lineatv = FALSE; //shows whether or not to make the line
bool snapatv = FALSE;


char buffer[50];
int x,y;

int main(int argc, char *argv[]) { 

  p1  = start = NULL;
  heads = lnstart = NULL;
  gend.snap = gend.cnrl = FALSE; 

  gtk_init(&argc, &argv); //init Gtk

  builder = gtk_builder_new_from_file ("part1.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
      g_signal_connect(window, "destroy",G_CALLBACK(on_destroy), NULL);  

  gtk_builder_connect_signals(builder, NULL);

  fixed1		  = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
  draw1		    = GTK_WIDGET(gtk_builder_get_object(builder, "draw1"));
  button1     = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
  clear       = GTK_WIDGET(gtk_builder_get_object(builder, "clear"));
	label1      = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
  label2      = GTK_WIDGET(gtk_builder_get_object(builder, "label2"));
  label3      = GTK_WIDGET(gtk_builder_get_object(builder, "label3"));  
  snaptgl     = GTK_WIDGET(gtk_builder_get_object(builder, "snaptgl"));  

  g_object_unref(builder);

  gtk_widget_set_events(draw1, GDK_POINTER_MOTION_MASK | GDK_BUTTON_MOTION_MASK 
                          | GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS | GDK_KEY_RELEASE);                      

  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

void	on_destroy() {gtk_main_quit();}

gboolean on_draw1_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data ) { 

  cairo_set_line_width(cr, 1.0);

  while (start == NULL){
    if (heads == NULL) {
    return FALSE;       
    }//case -program opened, line button not pressed. 
    else if (heads -> next == NULL){
    return FALSE;
    }//case - line button clicked, but nothing to draw.
    heads = heads -> next; //we now need to check the next line .
    start = heads -> head; //check to see if the head of the next line needs to be drawn.
  }


  if(lineatv == TRUE && lnstart -> head != NULL){ // the line drawn to the mouse. 
    
    snapchange(widget,cr);
  
    cairo_move_to (cr, (double) start->x, (double) start->y);
    cairo_line_to(cr,gend.coordx,gend.coordy);
    cairo_stroke(cr);
  }

  while (heads != NULL) {
    start = heads ->head; // ensure start is the latest line. 
    int old_x = start->x;  
    int old_y = start->y; //store the location of the start values 
    p1 = start -> next;//p1 pointed to second in line.
      while ( p1 != NULL ) {
         cairo_move_to (cr, (double) old_x, (double) old_y);
         cairo_line_to (cr, (double) p1->x, (double) p1->y);

         cairo_stroke (cr);

         old_x = p1->x; // old_x is now the x value from the p1 struct.  
         old_y = p1->y;
         p1 = p1 ->next; //p1 now points to the next value of the linked list 
      }    
      heads = heads->next; // move to the next point in the line.
  }  

  heads = lnstart; //reset head to point to lnstart. it should be already.
  start = heads ->head; //reset start to point to heads -> head. 
 	return FALSE;
 }

gboolean on_draw1_button_press_event(GtkWidget *widget, GdkEventButton *bevent) {
    if (bevent->button == 1 && lineatv == TRUE) {
        sprintf(buffer,"button clicked");
        gtk_label_set_text (GTK_LABEL(label3),buffer);
        if (gend.snap == TRUE) {draw_brush (widget, gend.snapcoordx , gend.snapcoordy);}
	      else { draw_brush(widget, gend.coordx , gend.coordy);}
    } // left mouse click

    else if (bevent->button == 3) {
      lineatv = FALSE;
      if (start == NULL){clearline();} // if no line made, delete associated head of line 
      sprintf(buffer,"line inactive.");
      gtk_label_set_text (GTK_LABEL(label3),buffer);
    } // right mouse click 

  return FALSE;
}

gboolean on_draw1_event(GtkWidget *widget, GdkEventButton *event) {

  if(lineatv == TRUE) {// Dont change if line is snapping
    gend.coordx = event->x;
    gend.coordy = event->y;
    if (gend.cnrl == TRUE) {straightline();}
  }

  sprintf(buffer,"X location: %.4f",event->x);
  gtk_label_set_text (GTK_LABEL(label1),buffer);
  sprintf(buffer,"Y location: %.4f",event->y);
  gtk_label_set_text (GTK_LABEL(label2),buffer);
  gtk_widget_queue_draw(draw1);
  return FALSE;
}

//this makes a new linked list, cool.
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y) {
	
  p1 = malloc(sizeof(struct Point)); //p1 now is a valid pointer.
	if (p1 == NULL) { printf("error making Point: - out of memory\n"); abort(); }

	p1->x = x; //p1's x value is the location of the mouse click.
	p1->y = y; //p1's y value is now the location of the mouse click.
	p1->next = start; //p1 now has an effective link to where start is stored. note that on the first click start is NULL, and after it the previously made P1 of the linked list.  
	start = p1; //start now points to the latest creation of p1. this is the START of the linked list, and the last point clicked by the user. .
  lnstart -> head = start; //the "head" feature of heads now points to the head (i.e,"Start") of p1. 
 
  gtk_widget_queue_draw (draw1);
}

void on_button1_clicked(GtkButton* b){  //line button.
  lineatv = TRUE;
  start = NULL; // reset start; a new line is being made.

  heads = malloc(sizeof(struct lines)); //Heads is now a valid pointer.
  if (heads == NULL) { printf("error making heads: out of memory\n"); abort(); }

  heads -> next = lnstart; //heads now has an effective link to where start is stored. note that on the first click start is NULL, and after it is the second to latest creation of heads. 
  heads -> head = NULL; //ensure the new heads is not pointing to garbage values.

  lnstart = heads; //lnstart now points to the latest creation of a line struct.
  sprintf(buffer,"line active.");
  gtk_label_set_text (GTK_LABEL(label3),buffer);

}

void	on_clear_clicked(GtkWidget *b1) { // clears (1) line
  if (lineatv == TRUE) return;
  clearline();
  gtk_widget_queue_draw (draw1);
}

void clearline(){
  if (heads != NULL){
    p1 = start;
    while (p1) { p2 = p1 -> next; free(p1); p1 = p2; } //clear the line drawn, beginning to end 
    start = NULL;

    heads2 = heads -> next; free(heads); heads = heads2;
    lnstart = heads;
    if (heads!=NULL) {start = heads->head;}
  }
}

void straightline(){ // on a cntrl+left mouse click, only do straight lines
  if (start == NULL) return;
  double xcheck, ycheck;
  double angle;

  xcheck = (double) gend.coordx - (double) start->x;
  ycheck = (double) gend.coordy - (double) start->y;
  
  if (xcheck != 0.0){  angle = atan(ycheck/xcheck)*180/3.1415;}
  else{ angle = 90;} // change, probably

  sprintf(buffer,"angle: %.2f ", angle);
  gtk_label_set_text (GTK_LABEL(label3),buffer);

  // xcheck = abs(xcheck);
  // ycheck = abs(ycheck);

   if (fabs(angle) < 22.5){ // horizontal
      gend.coordy = start->y;
    }
   else if(fabs(angle) > 67.5){ // vertical
     gend.coordx = start->x;
   }
   else { // all 45 degree angles
    double pyth = sqrt(pow(xcheck,2) + pow(ycheck,2));
    gend.coordx = ((int) (cos(M_PI/4) * pyth)) * abs(xcheck)/xcheck + start ->x; // abs(xcheck)/xcheck locks the sign
    gend.coordy = ((int) (sin(M_PI/4) * pyth)) * abs(ycheck)/ycheck + start ->y;
   }
}

void snapchange(GtkWidget *widget, cairo_t *cr){
  if (snapatv == TRUE){
    int xgap, ygap;
    int gap = 13;  

    heads2 = heads; // store heads 
    p2 = p1; //store point; idk if this is needed 

    while (heads != NULL) {
      p1 = heads -> head;//p1 pointed to first in line.
      //if (p1->next == NULL) return; // if first point in line don't snap
        while ( p1 != NULL ) {
          xgap = abs(gend.coordx - p1->x);
          ygap = abs(gend.coordy - p1->y);
          if (xgap < gap && ygap < gap){
            if (p1 == heads2->head) return; // Dont point to the most recent point clicked.  
            gend.coordx = p1->x;gend.snapcoordx = p1->x;
            gend.coordy = p1->y;gend.snapcoordy = p1->y;

            cairo_rectangle (cr, p1->x - (10), p1->y - (10), 20, 20);//point out snap location to user
            heads = heads2; //reset head to same value as beginning.
            p1 = p2; //reset p1 to same value as the beginning. 
            gend.snap = TRUE;
            return; //note : this return makes it such that the square points to the most recent point made.  
          }
          p1 = p1 ->next; //p1 now points to the next value of the linked list 
        }
      heads = heads->next; // move to the next point in the line.
    } // isn't this O(n^2)? thats rough buddy 

    heads = heads2; //reset head to same value as beginning.
    p1 = p2; //reset p1 to same value as the beginning. 
  }
  gend.snap = FALSE;
}

void	on_snaptgl_toggled(GtkToggleButton *b) {
	gboolean T = gtk_toggle_button_get_active(b);
	if (T) snapatv = TRUE;
	else   snapatv = FALSE;
	}

void on_window_key_press_event(GtkWidget *widget, GdkEventKey *cevent){
  sprintf(buffer,"Key pressed: %i ", cevent->keyval);
  gtk_label_set_text (GTK_LABEL(label3),buffer);

  if (cevent->keyval== 65507 || cevent -> keyval == 65508){
      gend.cnrl = TRUE;
  }
}

void on_window_key_release_event(GtkWidget *widget, GdkEventKey *devent){
  sprintf(buffer,"Key released: %i ", devent->keyval);
  gtk_label_set_text (GTK_LABEL(label3),buffer);

  if (devent->keyval== 65507 || devent -> keyval == 65508){
      gend.cnrl = FALSE;
  }
}