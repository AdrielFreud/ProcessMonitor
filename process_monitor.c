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
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/inotify.h>
#include <sys/select.h>

#define TAMANHO_EVENTO (sizeof(struct inotify_event))
#define TAMANHO_BUF 1024*TAMANHO_EVENTO
#define LOGNAME "log.txt"

static GtkWidget *directory;
static GtkWidget *working;

void registry(char *filename, char *text);
int watcher_file_manager(void);
struct tm *data;
time_t segundos;

int watcher_file_manager(void){

	int fd, wd;
	int i, t, l;
	char buf[TAMANHO_BUF];

	char *dict_name = (char* )gtk_entry_get_text(GTK_ENTRY(directory));

	fd_set rfds;
	struct inotify_event *evento;
	
	if((fd = inotify_init()) < 0){
		perror("inotify_init");
		exit(1);
	}

	wd = inotify_add_watch(fd, dict_name, IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_OPEN) ;

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

			if(evento->mask & IN_ACCESS){
				registry(evento->name, "- Arquivo Acessado!\n");

			} else if(evento->mask & IN_ATTRIB){
				registry(evento->name, "- Alterando Permissões!\n");

			}else if(evento->mask & IN_CLOSE_WRITE){
				registry(evento->name, "- Arquivo fechado para Escrita!\n");

			}else if(evento->mask & IN_CREATE){
				registry(evento->name, "- Arquivo/Diretorio Criado!\n");

			}else if(evento->mask & IN_DELETE){
				registry(evento->name, "- Arquivo/Diretorio Excluido!\n");

			}else if(evento->mask & IN_MODIFY){
				registry(evento->name, "- Arquivo Modificado!\n");

			}else if(evento->mask & IN_MOVED_TO){
				registry(evento->name, "- Arquivo/Diretorio Movido ou Renomeado!\n");

			}else if(evento->mask & IN_OPEN){
				registry(evento->name, "- Arquivo/Diretorio Aberto!\n");

			}else{
				registry(evento->name, "- Acessado!\n");

			}

        	i += TAMANHO_EVENTO + evento->len;
			sleep(1);
        }
    }

    return 0;
}
int main(int argc, char **argv){
	GtkWidget *window, *label_diretorio, *btn_save, *grid;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);


	directory = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), directory, 0, 1, 1, 1);
	
	label_diretorio = gtk_label_new("Diretorio: ");
	gtk_grid_attach(GTK_GRID(grid), label_diretorio, 0, 0, 1, 1);	

	btn_save = gtk_button_new_with_label("Run");
	g_signal_connect(btn_save, "clicked", G_CALLBACK(watcher_file_manager), NULL);
	gtk_grid_attach(GTK_GRID(grid), btn_save, 0, 2, 2, 2);

	working = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(working), "Working....");
	gtk_grid_attach(GTK_GRID(grid), working, 0, 5, 3, 3);

	gtk_window_set_title(GTK_WINDOW(window), "File Monitor - Adriel Freud");
	gtk_window_set_default_size(GTK_WINDOW(window), 170, 130);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

void registry(char *filename, char *text){
	FILE *fp = fopen(LOGNAME, "a");
	time(&segundos);
	data = localtime(&segundos);
	fprintf(fp, "[Horario - %d:%d:%d][%s] %s", data->tm_hour, data->tm_min, data->tm_sec, filename, text);
	fclose(fp);
}

