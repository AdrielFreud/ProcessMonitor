all:
	gcc process_monitor.c -o monitor -Wall
clean:
	rm process_monitor.c monitor
install:
	gcc process_monitor.c -o monitor -Wall
	install monitor /bin/monitor
	chmod a+x /bin/monitor
uninstall:
	rm -f /bin/monitor
