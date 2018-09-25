#include <unistd.h>
#include <fcntl.h>
#include <string.h>

const int stdout_fd = 1;
const int max_size  = 0x20000;

#define filename_expected \
	"Please specify filename as a first parameter\n"

#define tag_expected \
	"Can't find build_epoch tag\n"

#define magic1 \
	"\xab\xcd\xefMaxMind.com"

#define magic2 \
	"build_epoch\x04\x02"


int main(int argc, char **argv)
{
	unsigned char buffer[max_size], *p = 0, result[16];
	int  i, fd;
	long age;

	if (argc < 2) {
		write(
			stdout_fd,
			filename_expected,
			sizeof(filename_expected) - 1
		);
		_exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	lseek(fd, -max_size, SEEK_END);
	read(fd, buffer, max_size);

	for (i = max_size - sizeof(magic1); i >= 0; i--)
		if (!memcmp(buffer + i, magic1, sizeof(magic1) - 1))
			break;

	if (!i)
		i = max_size;

	for (; i < max_size - (int)sizeof(magic2); i++) {
		if (0 == memcmp(buffer + i, magic2, sizeof(magic2) - 1)) {
			p = buffer + i + sizeof(magic2) - 1;
		}
	}

	if (!p) {
		write(
			stdout_fd,
			tag_expected,
			sizeof(tag_expected) - 1
		);
		_exit(1);
	}

	age = *p++;
	age = age * 0x100 + *p++;
	age = age * 0x100 + *p++;
	age = age * 0x100 + *p++;

	/* lose printf from resulting binary */
	result[15] = '\n';
	for (i = 14; age; i--) {
		result[i] = '0' + age % 10;
		age /= 10;
	}

	write(stdout_fd, result + i, 16 - i);

	/* this kills cleanup code */
	_exit(0);
}
