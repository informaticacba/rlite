#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

using namespace std;

/* Arguments taken by the function:
 *
 * char *dev: the name of an interface (or '\0'). MUST have enough
 *            space to hold the interface name if '\0' is passed
 * int flags: interface flags (eg, IFF_TUN, IFF_NO_PI, IFF_TAP etc.)
 */
static int
tun_alloc(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("open(/dev/net/tun)");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

    if (*dev) {
        /* If a device name was specified, put it in the structure; otherwise,
         * the kernel will try to allocate the "next" device of the
         * specified type */
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /* Try to create the device */
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        return err;
    }

    /* Ff the operation was successful, write back the name of the
     * interface to the variable "dev", so the caller can know
     * it. Note that the caller MUST reserve space in *dev (see calling
     * code below) */
    strcpy(dev, ifr.ifr_name);

    /* this is the special file descriptor that the caller will use to talk
     * with the virtual interface */
    return fd;
}

void
alloc_tun()
{
    char tun_name[IFNAMSIZ];
    int tunfd;

    strcpy(tun_name, "tun1");
    tunfd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);
    close(tunfd);
}

static int
parse_conf(const char *path)
{
    ifstream fin(path);

    if (fin.fail()) {
        cerr << "Cannot open configuration file " << path << endl;
        return -1;
    }

    fin.close();

    return 0;
}

static void
usage(void)
{
    cout << "iporinad [OPTIONS]" << endl <<
        "   -h : show this help" << endl <<
        "   -c CONF_FILE: path to configuration file" << endl;
}

int main(int argc, char **argv)
{
    const char *confpath = "/etc/iporinad.conf";
    int opt;

    while ((opt = getopt(argc, argv, "hc:")) != -1) {
        switch (opt) {
            case 'h':
                usage();
                return 0;

            case 'c':
		confpath = optarg;
                break;

            default:
                printf("    Unrecognized option %c\n", opt);
                usage();
                return -1;
        }
    }

    if (parse_conf(confpath)) {
        return -1;
    }

    return 0;
}
