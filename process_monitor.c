
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/inotify.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

#define TAMANHO_EVENTO (sizeof(struct inotify_event))
#define TAMANHO_BUF 1024*TAMANHO_EVENTO

void log_register(char *mnsg, char *program);
int watcher_file_manager(char **argv);

int main(int argc, char **argv){
	if(argc < 2){
		printf("\n\tAdrielFreud\n\n");
		printf("Use: %s <arquivo/diretorio>\n", argv[0]);
		exit(1);
	}
	watcher_file_manager(argv);
	return 0;
}

int watcher_file_manager(char **argv){
	int fd, wd;
	int i, t, l;

	char buf[TAMANHO_BUF];
	char cwd[1024];

	fd_set rfds;
	struct inotify_event *evento;
	memset(cwd, 0, sizeof(cwd));

	if(getcwd(cwd, sizeof(cwd)) == NULL){
		perror("getcwd()");
		exit(1);
	}

	if((fd = inotify_init()) < 0){
		perror("inotify_init");
		exit(1);
	}

	wd = inotify_add_watch(fd, argv[1], IN_ACCESS | IN_CREATE | IN_DELETE) ;

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
			log_register("<< Arquivo Criado!", evento->name);

		}else if(evento->mask & IN_DELETE){
			log_register("<< Arquivo Deletado!", evento->name);

		}else{
			log_register("<< Arquivo Acessado!", evento->name);
		}
        i += TAMANHO_EVENTO + evento->len;
        }
    }

    return 0;
}

void log_register(char *mnsg, char *program){
        for(int i = 0; i<1; i++){
                if(sizeof(mnsg[i]) != 1 && sizeof(program[i] != 1)){
                        fprintf(stderr, "[!] This name of program are not Valid!\n");
                        exit(1);
                }else{
                        openlog(program, LOG_PID, LOG_USER);
                        syslog(LOG_INFO, "%s | getuid - %d\n", mnsg, getuid());
                        closelog();
                }
        }
}

