#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	struct ifconf ifc;
	struct ifreq ifr[10];

	ifc.ifc_len = 10 * sizeof(struct ifreq);
	ifc.ifc_buf = (char*)ifr;

	ioctl(s, SIOCGIFCONF, (char*)&ifc);

	int if_count = ifc.ifc_len / sizeof(struct ifreq);
	for(int i=0; i < if_count; i++)
	{
		if(ioctl(s, SIOCGIFHWADDR, &ifr[i]) == 0)
		{
			unsigned char mac[6] = {0};
			memcpy(mac, ifr[i].ifr_hwaddr.sa_data, 6);
			printf("%s: %02x:%02x:%02x:%02x:%02x:%02x\n",
					ifr[i].ifr_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	}

	return 0;
}
