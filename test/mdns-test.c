#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include "query.h"

void ipv4_func(const ipv4_address_t *, void *);
void name_func(const char *, void *);

void usage(const char *pstrProgram)
{
    printf("Usage:\n"
           "  %s [opt] <domain name/ipv4>\n"
           "  -x   query ipv4\n"
           "Example:\n"
           "  %s localhost.local\n"
           "  %s -x 192.168.1.1\n", pstrProgram, pstrProgram, pstrProgram);
}

int main(int argc, char *argv[]) 
{
    int opt;
    int ret = 0;
    int fd = -1;
    int bIsQueryIPV4 = 0;
    const char *pstrQuery = NULL;

    while ((opt = getopt(argc, argv, "xh")) != -1)
    {
        switch (opt)
        {
            case 'x':
                bIsQueryIPV4 = 1;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default: /* '?' */
                usage(argv[0]);
                return -1;
        }
    }

    if (optind >= argc)
    {
        usage(argv[0]);
        return -1;
    }

    pstrQuery = argv[optind];

    if ((fd = mdns_open_socket()) < 0)
    {
        fprintf(stderr, "open socket fail\n");
        return -1;
    }

    if (bIsQueryIPV4)
    {
        ipv4_address_t ipv4;
        ipv4.address = inet_addr(pstrQuery);
        if (mdns_query_ipv4(fd, &ipv4, name_func, NULL) < 0) 
        {
            fprintf(stderr, "query %s fail\n", pstrQuery);
            ret = -1;
        }
    }
    else
    {
        if (mdns_query_name(fd, pstrQuery, &ipv4_func, NULL, NULL) < 0) 
        {
            fprintf(stderr, "query %s fail\n", pstrQuery);
            ret = -1;
        }
    }

    close(fd);
    return ret;
}

void ipv4_func(const ipv4_address_t *ipv4, void *userdata) 
{
    printf("%s\n", inet_ntoa(*(const struct in_addr*) &ipv4->address));
}

void name_func(const char *name, void *userdata) 
{
    printf("%s\n", name);
}
