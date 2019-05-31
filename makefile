

filestat: filestat.c filestatConf.c rw_file.c
	gcc -o filestat filestat.c filestatConf.c rw_file.c
