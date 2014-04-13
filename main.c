#include <gtk/gtk.h>
#include <stdio.h>
GtkWidget *empty_but;
GtkWidget *dialog;
int pos [16];

int test (void) 
{
	int n;

	for (n=1; n<=16; n++)
	{
		if (pos[n-1] != n) return FALSE;
	}
	return TRUE;
}
		

static void but_click (GtkWidget *widget, gpointer data)
{
	gint x = widget->allocation.x;
	gint y = widget->allocation.y;
	gint e_x = empty_but->allocation.x;
	gint e_y = empty_but->allocation.y;
	GtkWidget *layout = gtk_widget_get_parent (widget);
	int i,n, e_i, e_n;

	if ( ((x == e_x) && ((y == (e_y-25)) || (y == (e_y+25)))) ||
			((y == e_y) && ((x == (e_x-25)) || (x == (e_x+25))))) 
	{
		gtk_layout_move (GTK_LAYOUT (layout), widget, e_x, e_y);
		gtk_layout_move (GTK_LAYOUT (layout), empty_but, x, y);

		n = 4*((y-10) / 25) + ((x-10) / 25) + 1;
		i = 0;
		while (pos[i++] != n);
		 
		e_n = 4*((e_y-10) / 25) + ((e_x-10) / 25) + 1;
		e_i = 0;
		while (pos[e_i++] != e_n);

		pos[--e_i] = n;
		pos[--i] = e_n;
		
		if (test ())
		{
			if (!dialog)
			{
				dialog = gtk_message_dialog_new (GTK_WINDOW (gtk_widget_get_parent (layout)), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Yeeees!!!!");
				
			}
			gtk_dialog_run (GTK_DIALOG (dialog));
			gtk_widget_destroy (dialog);
			gtk_widget_destroy (gtk_widget_get_parent (layout));
		}
	}
	
}

int gen_pos (int value)
{
	int k;

	srand (getpid());
	while (TRUE) 
	{
		k = 1 + rand() % 16;
		if (0 == pos[k-1])
		{
			pos[k-1] = value;
			return k;	
		}
	}
	return -1;
}

int main (int argc, char *argv[]) 
{
	GtkWidget *window;
	GtkWidget *layout;
	GtkWidget *but;
	int i,j, n;
	int x,y;
	char text [256];
	
	for (i = 0; i < 16; i++) pos[i]=0;
	
	gtk_init (&argc, &argv);

	window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title (GTK_WINDOW (window),"Fifteen");
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	gtk_widget_set_size_request (window, 120,120);
	
	layout = gtk_layout_new (NULL, NULL);
	//gtk_layout_set_size (GTK_LAYOUT (layout), 120, 	120);
	gtk_container_add (GTK_CONTAINER (window), layout);
	gtk_widget_show (layout);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			n = gen_pos(4*i+j+1);
			x = j*25+10;
			y = i*25+10;
			if (n == 16)
			{
				empty_but = gtk_button_new ();
				gtk_layout_put (GTK_LAYOUT (layout), empty_but, x, y);
			} else {
				sprintf (text, "%d", n);
				but = gtk_button_new_with_label (text);
				gtk_widget_set_size_request (but, 25, 25);
			
				g_signal_connect (G_OBJECT (but), "pressed", G_CALLBACK (but_click), NULL);
				gtk_layout_put (GTK_LAYOUT (layout), but, x, y);
				gtk_widget_show (but);
			}
		}
	}

	gtk_widget_show (window); 
	gtk_main ();
	return 0;
}
