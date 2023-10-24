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

GtkBuilder		  *builder; 

static void draw_brush (GtkWidget *widget, gdouble    x, gdouble    y);

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
} gend;

struct lines {
  struct Point *head; // starting pointer 
  struct lines *next; // next in line
} *heads, *heads2, *lnstart; 

void		  on_destroy(); 
bool activated = FALSE; //shows wether or not to make the line

char buffer[50];
int x,y;

int main(int argc, char *argv[]) { 

  p1  = start = NULL;
  heads = lnstart = NULL;

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

  g_object_unref(builder);

  gtk_widget_set_events(draw1, GDK_POINTER_MOTION_MASK | GDK_BUTTON_MOTION_MASK 
                          | GDK_BUTTON_PRESS_MASK);                      

  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

void	on_destroy() {gtk_main_quit();}

gboolean on_draw1_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data ) { 

  cairo_set_line_width(cr, 1.0);

  //if (start == NULL || heads == NULL) return FALSE; //Oregano. This needs to be expanded.

  // Lot of caviots on when and when not to draw. this attempts to capture. 
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

  //juuuuust to make sure this isnt breaking everything. 
  if(activated == TRUE && lnstart -> head != NULL){
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
    if (bevent->button == 1) {
      
      if(activated == TRUE){//note to self: make AND operand with these 2 for loops 
        sprintf(buffer,"button clicked");
        gtk_label_set_text (GTK_LABEL(label3),buffer);
	      draw_brush (widget, bevent->x, bevent->y);
      }
    }
    else if (bevent->button == 3) {
      activated = FALSE;
      sprintf(buffer,"line inactive.");
      gtk_label_set_text (GTK_LABEL(label3),buffer);
    }

  return FALSE;
}

gboolean on_draw1_event(GtkWidget *widget, GdkEventButton *event) {

  if(activated == TRUE){
    gend.coordx = event->x;
    gend.coordy = event->y;
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
  //for some reason this does NOT work. 
 
  gtk_widget_queue_draw (draw1);
}

void on_button1_clicked(GtkButton* b){  
  activated = TRUE;
  start = NULL; // reset.

  heads = malloc(sizeof(struct lines)); //Heads is now a valid pointer.
  if (heads == NULL) { printf("error making heads: out of memory\n"); abort(); }

  heads -> next = lnstart; //heads now has an effective link to where start is stored. note that on the first click start is NULL, and after it is the second to latest creation of heads. 
  heads -> head = NULL; //ensure the new heads is not pointing to garbage.
  
  lnstart = heads; //lnstart now points to the latest creation of a line.
  

  sprintf(buffer,"line active.");
  gtk_label_set_text (GTK_LABEL(label3),buffer);

}

void	on_clear_clicked(GtkWidget *b1) { // clears (1) line, only use when activated = true 
  
  p1 = start;
  while (p1) { p2 = p1 -> next; free(p1); p1 = p2; } //clear the line drawn, beginning to end 
  start = NULL;
 
  heads = lnstart;  //delete the most recent line head 
  heads2 = heads -> next; free(heads); heads = heads2;
  lnstart = NULL;
    
  gtk_widget_queue_draw (draw1);

}
