#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include "acdb_data.h"

#define TABLE_SIZE (SND_DEVICE_MAX * sizeof(int))

int main(int argc, char** argv) {

	int *acdb_device_table;

	int max_acdb_id = 1000;

	struct stat stat_buf;

	/* check args */
	if (argc < 2) {
		printf("%s: [audio hal path] [max_acdb_id]\n", argv[0]);
		exit(1);
	}

	/* get the max acdb_id allowed if any */
	if (argc > 2) {
		max_acdb_id = strtol(argv[2], NULL, 10);
	}

	/* open the audio hal file */
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	/* get the file stats */
	if (fstat(fd, &stat_buf) < 0) {
		perror("stat");
		exit(1);
	}

	/* mmap the file to memory */
	char * hal_file = mmap(0, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (hal_file == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	/* pointers to beginning of table and end of file */
	char * curr = hal_file;
	char * end = hal_file + stat_buf.st_size - 1;

	int i;

	/* go through the whole file every SND_DEVICE_MAX*4 bytes */
	while((curr+TABLE_SIZE) < end) {

		acdb_device_table = (int*)curr;

		/* verify that the first entry is -1 */
		if (acdb_device_table[SND_DEVICE_NONE] != -1) {
			goto next;
		}

		for (i = 1; i < SND_DEVICE_MAX; i++) {
			//if ((acdb_device_table[i] < 0) || (acdb_device_table[i] > max_acdb_id)) goto next;
			if (acdb_device_table[i] > max_acdb_id) goto next;
		}

		/* print the table */
		printf("\t<acdb_ids>\n");
		for (i = 1; i < SND_DEVICE_MAX; i++)
			printf("\t\t<device name=\"%s\" acdb_id=\"%i\"/>\n", faux_device_table[i], acdb_device_table[i]);
		printf("\t</acdb_ids>\n");

next:
		/*advance to the next byte in the file */
		curr = curr + 1;
	}

	return 0;
}
