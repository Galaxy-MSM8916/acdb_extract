#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "acdb_data.h"

typedef char* (*platform_get_snd_device_name_t)(int);

int main(int argc, char** argv) {
	void *handle;
	platform_get_snd_device_name_t platform_get_snd_device_name;

	if (argc != 2) {
		printf("%s: [audio hal path]\n", argv[0]);
		exit(1);
	}

	handle = dlopen(argv[1], RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		return 1;
	}

	platform_get_snd_device_name = (platform_get_snd_device_name_t) dlsym(handle, "platform_get_snd_device_name");

	if (!platform_get_snd_device_name)  {
		fprintf(stderr, "%s\n", dlerror());
		return 1;
	}

	printf("static char * device_table[SND_DEVICE_MAX] = {\n");
	int dev;
	for (dev = 0; dev < SND_DEVICE_MAX; dev++)
		printf("\t[%s] = \"%s\",\n", device_table[dev], platform_get_snd_device_name(dev));
	printf("};\n");

	dlclose(handle);
	return 0;
}
