#ifndef SRC_UTILS_DHCP_H_
#define SRC_UTILS_DHCP_H_


typedef struct dhcp dhcp_t;
typedef struct node node_t;
typedef unsigned int ip_t;

dhcp_t *DhcpCreate(ip_t net, int num_static_bits);

void DhcpDestroy(dhcp_t *dhcp);

ip_t DhcpGetNextIP(dhcp_t *dhcp);

ip_t DhcpGetIP(dhcp_t *dhcp, ip_t address);

void DhcpReleaseIP(dhcp_t *dhcp, ip_t address);

#endif /* SRC_UTILS_DHCP_H_ */

