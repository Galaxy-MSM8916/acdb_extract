all: acdb_extract

acdb_extract: acdb_extract.o
	gcc -Wall -g -o acdb_extract $^

%.o : %.c acdb_data.h
	gcc -Wall -g -c $<

clean:
	rm -f *.o acdb_extract
