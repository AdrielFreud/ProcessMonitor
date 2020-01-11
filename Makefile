all:
	gcc process_monitor.c -o monitor -Wall `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm process_monitor.c monitor
install:
	gcc process_monitor.c -o monitor -Wall
	install monitor /bin/monitor
	chmod a+x /bin/monitor
uninstall:
	rm -f /bin/monitor
