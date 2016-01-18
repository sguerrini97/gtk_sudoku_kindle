/*
	Copyright ©2015 sguerrini97
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef BUILD_KINDLE
#define DATA_PATH "/mnt/us/extensions/sudoku/data"
#define GRID_X 60
#define GRID_Y 60
#define CMD_X 47
#define CMD_Y 43
#define TOOL_X 115
#define TOOL_Y 50
#else
#ifdef BUILD_PC
#define DATA_PATH "data"
#define GRID_X 50
#define GRID_Y 50
#define CMD_X 43
#define CMD_Y 43
#define TOOL_X 94
#define TOOL_Y 50
#else
#error "WTF are you building for"
#endif
#endif

#define APPLICATION_TITLE "- Kindle Sudoku -"
#define QUIT_BUTTON_TEXT "Quit"
#define LOAD_BUTTON_TEXT "Load"
#define SAVE_BUTTON_TEXT "Save"
#define NEW_BUTTON_TEXT "New"

#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

GtkWidget *window;
static GtkWidget *sudokuw[9][9];

int sudoku_total_files = 0;
char *sudoku_files[256];

// grid cell to act on, current sudoku file
int selected_x = 0, selected_y = 0, current_sudoku = -1;

// change grid cell
static void selection( GtkWidget *widget, gpointer data )
{
	selected_x = (int)((int*)data)[0];
	selected_y = (int)((int*)data)[1];
	// g_print("New selection: %i, %i\n", selected_x, selected_y);
}

// put number in grid cell
static void put_number( GtkWidget *widget, gpointer data )
{
	char *string = (char*)data;
	if( string[0] == 'X' )
		string[0] = 0;
	gtk_button_set_label( GTK_BUTTON( sudokuw[selected_x][selected_y] ), string );
}

static void save( GtkWidget *widget, gpointer data )
{
	// TODO
}

static void load( GtkWidget *widget, gpointer data )
{
	// TODO
}

// start a new sudoku
static void new( GtkWidget *widget, gpointer data )
{
	FILE *sudoku_file = NULL;
	char path[256];
	int i,j;
	char n[2] = { 0, '\0' };
	char c;
	
	int file = -1;
	
	do
	{
		file = rand()%sudoku_total_files;
	}while( file == current_sudoku ); // different from the current one

	current_sudoku = file;
	// g_print("New sudoku: %i: %s\n", file, sudoku_files[file]);
	
	// fill the grid
	sudoku_file = fopen( sudoku_files[file], "r" );
	for( i = 0; i < 9; i++ )
	{
		for( j = 0; j < 9; j++ )
		{
			c = fgetc( sudoku_file );
			if( c != '.' )
			{
				n[0] = c;
				gtk_widget_set_sensitive(sudokuw[i][j], FALSE);
			}
			else
			{
				n[0] = '\0';
				gtk_widget_set_sensitive(sudokuw[i][j], TRUE);
			}
			gtk_button_set_label( GTK_BUTTON(sudokuw[i][j]), n );
		}
	}
	fclose(sudoku_file);
}

// quit
static void quit( GtkWidget *widget, gpointer data )
{
	g_print("Quitting.. Bye\n");
	gtk_main_quit();
}

static gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
	return FALSE; // quit
}

static void destroy( GtkWidget *widget, gpointer data )
{
	gtk_main_quit();
}

int main( int argc, char *argv[] )
{
	GtkWidget *lTitle;												// title lable
	GtkWidget *bQuit, *bLoad, *bSave, *bNew;						// toolbox buttons
	GtkWidget *vbox, *hbox, *separator, *cmdbox, *toolbox;			// boxes
	GtkWidget *numbers[10];											// numbers buttons

	int i,j;
	int *data = NULL;
	char num[2] = { 0, '\0' };
	char *n = NULL;
	
	DIR *dir;
	struct dirent *ep;
	
	// count sudoku files and save paths to them
	dir = opendir(DATA_PATH);
	while( ep = readdir(dir) )
	{
		if( strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0 )
			continue;
		sudoku_files[sudoku_total_files] = (char*)malloc(sizeof(char)*256);
		sprintf( sudoku_files[sudoku_total_files], "%s/%s", DATA_PATH, ep->d_name );
		g_print("%s\n", sudoku_files[sudoku_total_files]);
		sudoku_total_files++;
	}
	closedir(dir);

	// random seed
	srand(time(NULL));

	gtk_init( &argc, &argv );

	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	g_signal_connect( window, "delete-event", G_CALLBACK(delete_event), NULL );
	g_signal_connect( window, "destroy", G_CALLBACK(destroy), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	lTitle = gtk_label_new( APPLICATION_TITLE );

	bQuit = gtk_button_new_with_label( QUIT_BUTTON_TEXT );
	gtk_widget_set_size_request( bQuit, TOOL_X, TOOL_Y );
	g_signal_connect (bQuit, "clicked", G_CALLBACK(quit), NULL);

	bLoad = gtk_button_new_with_label( LOAD_BUTTON_TEXT );
	gtk_widget_set_size_request( bLoad, TOOL_X, TOOL_Y );
	g_signal_connect (bLoad, "clicked", G_CALLBACK(load), NULL);
	gtk_widget_set_sensitive(bLoad, FALSE); // TODO

	bSave = gtk_button_new_with_label( SAVE_BUTTON_TEXT );
	gtk_widget_set_size_request( bSave, TOOL_X, TOOL_Y );
	g_signal_connect (bSave, "clicked", G_CALLBACK(save), NULL);
	gtk_widget_set_sensitive(bSave, FALSE); // TODO

	bNew = gtk_button_new_with_label( NEW_BUTTON_TEXT );
	gtk_widget_set_size_request( bNew, TOOL_X, TOOL_Y );
	g_signal_connect (bNew, "clicked", G_CALLBACK(new), NULL);

	vbox = gtk_vbox_new( FALSE, 0 );
	gtk_box_pack_start(GTK_BOX(vbox), lTitle, 0,0,5);
	
	// prepare the grid (with buttons)
	for( i=0; i<9; i++ )
	{
		hbox = gtk_hbox_new( FALSE, 0 );
		for( j=0; j<9; j++ )
		{
			sudokuw[i][j] = gtk_button_new();
			gtk_widget_set_sensitive(sudokuw[i][j], FALSE);
			gtk_widget_set_size_request( sudokuw[i][j], GRID_X, GRID_Y );
			data = (int*) malloc(sizeof(int)*2);
			data[0] = i; data[1] = j;
			g_signal_connect (sudokuw[i][j], "clicked", G_CALLBACK(selection), data);
			gtk_box_pack_start(GTK_BOX(hbox), sudokuw[i][j], 0,0,0);
			if( (j+1)%3 == 0 && j<8 )
			{
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox), separator, 0, 0, 5);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);
		if( (i+1)%3 == 0 && i<8 )
		{
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox), separator, 0, 0, 5);
		}
	}
	
	// prepare the number buttons
	cmdbox = gtk_hbox_new( FALSE, 0 );
	for( i = 0; i < 10; i++ )
	{
		if( i == 9 )
			num[0] = 'X';
		else
			num[0] = 0x31+i;
		numbers[i] = gtk_button_new_with_label( num );
		gtk_widget_set_size_request( numbers[i], CMD_X, CMD_Y );
		gtk_box_pack_start( GTK_BOX(cmdbox), numbers[i], 0, 0, 5 );
		n = (char*)malloc(sizeof(char)*2);
		n[1] = '\0';
		n[0] = num[0];
		g_signal_connect (numbers[i], "clicked", G_CALLBACK(put_number), n);
	}
	
	
	gtk_box_pack_start(GTK_BOX(vbox), cmdbox, 0, 0, 10 );
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	// prepare the toolbox
	toolbox = gtk_hbox_new( FALSE, 20 );
	gtk_box_pack_start(GTK_BOX(toolbox), bNew, 0, 0, 5);
	gtk_box_pack_start(GTK_BOX(toolbox), bSave, 0, 0, 5);
	gtk_box_pack_start(GTK_BOX(toolbox), bLoad, 0, 0, 5);
	gtk_box_pack_start(GTK_BOX(toolbox), bQuit, 0, 0, 5);
	
	gtk_box_pack_start(GTK_BOX(vbox), toolbox, 0, 0, 10 );

	gtk_window_set_title( GTK_WINDOW(window), "L:A_N:application_ID:sudoku");
	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
