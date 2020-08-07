CC=dcc
CFLAGS=-Werror -Wall

smips: smips.c

clean:
	rm -rf smips

give_remote:
	ssh cse "cd ~/1521/assignment2 && git pull && give cs1521 ass2_smips smips.c"

give:
	give cs1521 ass2_smips smips.c
