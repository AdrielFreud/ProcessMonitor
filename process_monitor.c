/*
----------------------------------
# Desenvolvido por Adriel Freud!
# Contato: businessc0rp2k17@gmail.com
# FB: http://www.facebook.com/xrn401
#   =>DebutySecTeamSecurity<=
----------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <sys/inotify.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

#define TAMANHO_EVENTO (sizeof(struct inotify_event))
#define TAMANHO_BUF 1024*TAMANHO_EVENTO

static GtkWidget *command_compile;
static GtkWidget *command_run;
static GtkWidget *working;

void update(char *run[], char *comp[]);
int watcher_file_manager(void);

int watcher_file_manager(void){
	gtk_label_set_text(GTK_LABEL(working), "Working....");

	int fd, wd;
	int i, t, l;
	char buf[TAMANHO_BUF];

	char *comp[] = {(char* )gtk_entry_get_text(GTK_ENTRY(command_compile)), NULL};
	char *run[] = {(char *)gtk_entry_get_text(GTK_ENTRY(command_run)), NULL};

	fd_set rfds;
	struct inotify_event *evento;
	
	if((fd = inotify_init()) < 0){
		perror("inotify_init");
		exit(1);
	}

	wd = inotify_add_watch(fd, ".", IN_ACCESS | IN_CREATE | IN_DELETE) ;

	if(wd < 0){
        	perror("inotify_add_watch");
        	exit(1);
    	}

    	while(1){
    		FD_ZERO(&rfds);
	        FD_SET(fd, &rfds);

        t = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
        if(t<0) {
            perror("select");
            exit(1);
        }

        if(t == 0) continue;

        l = read(fd, buf, TAMANHO_BUF);

        i=0;

        while(i<l){
        	evento = (struct inotify_event *)&buf[i];

		if(evento->mask & IN_CREATE){
			update(run, comp);
		}else if(evento->mask & IN_DELETE){
			update(run, comp);
		}else{
			update(run, comp);
		}
        i += TAMANHO_EVENTO + evento->len;
        }
    }

    return 0;
}
int main(int argc, char **argv){
	GtkWidget *window, *label_command, *label_compile, *btn_save, *grid;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	command_compile = gtk_entry_new();

	gtk_grid_attach(GTK_GRID(grid), command_compile, 1, 1, 1, 1);

	command_run = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), command_run, 0, 1, 1, 1);
	
	label_command = gtk_label_new("Command to Run");
	gtk_grid_attach(GTK_GRID(grid), label_command, 0, 0, 1, 1);	

	label_compile = gtk_label_new("Command to Compile: ");
	gtk_grid_attach(GTK_GRID(grid), label_compile, 1, 0, 1, 1);

	btn_save = gtk_button_new_with_label("Run");
	g_signal_connect(btn_save, "clicked", G_CALLBACK(watcher_file_manager), NULL);
	gtk_grid_attach(GTK_GRID(grid), btn_save, 0, 2, 2, 2);

	working = gtk_label_new(NULL);
	gtk_grid_attach(GTK_GRID(grid), working, 0, 5, 3, 3);

	gtk_window_set_title(GTK_WINDOW(window), "Update the C0d3");
	gtk_window_set_default_size(GTK_WINDOW(window), 335, 150);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

void update(char *run[], char *comp[]){
	/*
	puts("Compilando...");
	execv(comp[0], comp); 
	puts("Executando...");
	execv(run[0], run);
	puts("Finished...");
	*/
	printf("comp: %s\n\nrun: %s\n", comp[0], run[0]);
}

