//STATUS: successfully made a line with the linked list method. 

#define _GNU_SOURCE

#include <string.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>

GtkWidget       *window;
GtkWidget       *fixed1;
GtkWidget       *draw1;
GtkWidget       *button1;
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

void		  on_destroy(); 
bool activated = FALSE; //shows wether or not to make the line
char buffer[50];

int main(int argc, char *argv[]) { 

  p1 = p2 = start = NULL;

  gtk_init(&argc, &argv); //init Gtk

  builder = gtk_builder_new_from_file ("part1.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
      g_signal_connect(window, "destroy",G_CALLBACK(on_destroy), NULL);  

  gtk_builder_connect_signals(builder, NULL);

  fixed1		  = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
  draw1		    = GTK_WIDGET(gtk_builder_get_object(builder, "draw1"));
  button1     = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
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
  if (start == NULL) return FALSE;

	int old_x = start->x;  
	int old_y = start->y;

	p1 = start->next;

	while ( p1 != NULL ) {

		cairo_move_to (cr, (double) old_x, (double) old_y);
		cairo_line_to (cr, (double) p1->x, (double) p1->y);
		cairo_stroke (cr);

		old_x = p1->x; // old_x is now the x value from the p1 struct.  
		old_y =  p1->y;
		p1 = p1 ->next; //p1 now points to the next value of the linked list 
	}


  if(activated == TRUE){
    cairo_move_to (cr, (double) start->x, (double) start->y);
    cairo_line_to(cr,gend.coordx,gend.coordy);
    cairo_stroke(cr);
  }

	return FALSE;
}

gboolean on_draw1_button_press_event(GtkWidget *widget, GdkEventButton *bevent) {
    if (bevent->button == 1) {
      if(activated == TRUE){//note to self: make AND operand with these 2 for loops 
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

//i dont know what this does. 
// gboolean on_draw1_motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer data) {
// 	if (event->state & GDK_BUTTON1_MASK ) draw_brush (widget, event->x, event->y);
// 	return TRUE;
// }


//this makes a new linked list, cool.
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y) {
	p1 = malloc (sizeof(struct Point)); //p1 now is a valid pointer. 
	if (p1 == NULL) { printf("out of memory\n"); abort(); }
	p1->x = x; //p1's x value is the location of the mouse click.
	p1->y = y; //p2's y value is now the location of the mouse click.
	p1->next = start; //p1 now has an effective link to where start is stored. note that on the first click start is NULL.
	start = p1; //start now points to p1. this creates a linked list.
	gtk_widget_queue_draw (draw1);
}

void on_button1_clicked(GtkButton* b){  
  activated = TRUE;
  sprintf(buffer,"line active.");
  gtk_label_set_text (GTK_LABEL(label3),buffer);
}