// nmload - minimal LKM loader for nomount (KallRecon build).
//
// nomount.ko embeds KallRecon and, for an exact-KMI build, only references
// kernel symbols that are exported (resolvable by the normal module loader).
// So loading reduces to a single finit_module(2); no /proc/kallsyms patching
// (lkmloader) and no ksud/apd insmod wrapper are required.
//
// Usage: nmload <module.ko> [params]
//   params are passed to the kernel as module options (space separated).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifndef __NR_finit_module
#if defined(__aarch64__) || defined(__x86_64__)
#define __NR_finit_module 273
#endif
#endif

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <module.ko> [params]\n", argv[0]);
        return 2;
    }

    int fd = open(argv[1], O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        fprintf(stderr, "[-] open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    const char *params = (argc >= 3) ? argv[2] : "";
    long ret = syscall(__NR_finit_module, fd, params, 0);
    int err = errno;
    close(fd);

    if (ret == 0) {
        printf("[+] loaded: %s\n", argv[1]);
        return 0;
    }

    fprintf(stderr, "[-] finit_module %s failed: %s\n", argv[1], strerror(err));
    return 1;
}
