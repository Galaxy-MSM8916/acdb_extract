all: acdb_extract get_snd_dev_names

acdb_extract: acdb_extract.o
	gcc -Wall -g -o acdb_extract $^

get_snd_dev_names: get_snd_dev_names.o
	gcc -Wall -g -o get_snd_dev_names -ldl $^

%.o : %.c acdb_data.h
	gcc -Wall -g -c $<

clean:
	rm -f *.o acdb_extract get_snd_dev_names
