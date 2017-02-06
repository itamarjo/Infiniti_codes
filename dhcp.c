/*
 * dhcp.c
 *
 *  Created on: Sep 15, 2016
 *      Author: pizza
 */

#include <string.h>
#include <stdlib.h> /*for malloc */
#include <assert.h>
#include "dhcp.h"

struct dhcp
{
	node_t *root;
	ip_t net;
	int num_static_bits;
};

struct node
{
	int occupied;
	node_t *child[2];
};


/* recives a num and return its byte mirror using loop and bit wise  */
static int FlipBinary(int n, int bits_to_flip)
{
	int flip = 0;
	int i;

	for (i = 0; i < bits_to_flip ; ++i)	/* loop once for each bit in a byte */
	{
		flip <<= 1; /* make room for next num */
		if ( 1 & n)
		{
			flip += 1;	/* active last bit if needed */
		}
		n >>= 1;	/* get next bit from n */
	}

	return (flip);
}


static void RecDhcpReleaseIP(node_t *node, ip_t address, int height)
{
	if (0 == height)
	{
		node->occupied = 0;
		return;
	}

	RecDhcpReleaseIP(node->child[address & 1], address >> 1, --height);

	node->occupied = 0;
}

static node_t *CreateNode()
{
	node_t *creator = (node_t *)malloc(sizeof(node_t));
	if(0 == creator)
	{
		return(0);
	}

	creator->child[0] = 0;
	creator->child[1] = 0;
	creator->occupied = 0;

	return(creator);
}

static int RecDhcpGetIP(node_t *node, ip_t address, int height)
{
	int res = 0;

	if(node->occupied == 1)
	{
		return(0);
	}

	if(0 == height)
	{
		node->occupied = 1;
		return(1);
	}

	if(0 == node->child[address & 1])
	{
		node->child[address & 1] = CreateNode();
		if (0 == node->child[address & 1])
		{
			return(0);
		}
	}

	res = RecDhcpGetIP(node->child[address & 1], address >> 1, --height);

	if((node->child[0] != 0) && (node->child[1] != 0))
	{
		if ((node->child[0]->occupied == 1) && (node->child[1]->occupied == 1))
		{
			node->occupied = 1;
		}
	}

	return(res);
}


dhcp_t *DhcpCreate(ip_t net, int num_static_bits)
{
	dhcp_t *dhcp = malloc(sizeof(dhcp_t));
	int available_bits = (4 * 8) - num_static_bits;
	int i = 0;
	int j = available_bits;
	node_t *creator = 0;

	if (0 == dhcp)
	{
		return(0);
	}

	dhcp->net = net;
	dhcp->num_static_bits = num_static_bits;

	dhcp->root = (node_t *)malloc(sizeof(node_t));
	dhcp->root->occupied = 0;
	if(0 == dhcp->root)
	{
		return(0);
	}
	creator = dhcp->root;
	for(i = 0; i < 2; ++i)
	{

		for (j = 0; j < available_bits; ++j)
		{
			creator->child[i] = (node_t *)malloc(sizeof(node_t));
			if(0 == creator->child[i])
			{
				return(0);
			}

			creator = creator->child[i];
			creator->occupied = 0;
		}
		creator->occupied = 1;
	}

	return(dhcp);

}

void RecDestory(node_t *node)
{
	if (node->child[0] != 0)
	{
		RecDestory(node->child[0]);
	}
	else if (node->child[1] != 0)
	{
		RecDestory(node->child[1]);
	}

	free(node); /*in case there is no children */
}


void DhcpDestroy(dhcp_t *dhcp)
{
	if(0 != dhcp->root)
	{
		RecDestory(dhcp->root);
	}

	free(dhcp);
}




ip_t RecNextIp(node_t *node, int height)
{
	int direction = 0;
	int res = 0;

	if (0 == height)
	{
		node->occupied = 1;
		return(res);
	}

	if (node->child[0] == 0)
	{
		node->child[0] = CreateNode();
		if (node->child[0] == 0)/* check if malloc failed */
		{
			return(0);
		}
		direction = 0; /* left */
	}

	if (0 != node->child[0])
	{
		if (node->child[0]->occupied == 0)
		{
			direction = 0; /*left */
		}
		else
		{
			if (node->child[1] == 0)
			{
				node->child[1] = CreateNode();
				if(node->child[1] == 0)/*check if malloc failed */
				{
					return(0);
				}
			}
			direction = 1; /* right */
		}
	}

	res = RecNextIp(node->child[direction], --height);

	if ((node->child[0] != 0) && (node->child[1]!= 0))
	{
		if((node->child[0]->occupied == 1) && (node->child[1]->occupied == 1))
		{
			node->occupied = 1;
		}
	}
	return((res << 1) | direction);
}


ip_t DhcpGetNextIP(dhcp_t *dhcp)
{
	assert(dhcp != 0);

	return(dhcp->net | FlipBinary(RecNextIp (dhcp->root, 32 - dhcp->num_static_bits), 32 - dhcp->num_static_bits));
}



ip_t DhcpGetIP(dhcp_t *dhcp, ip_t address)
{
	int height = 32 - dhcp->num_static_bits;
	if(RecDhcpGetIP(dhcp->root, FlipBinary(address & ~dhcp->net, height ), height))
	{
		return(address);
	}
	else
	{
		return(DhcpGetNextIP(dhcp));
	}

}


void DhcpReleaseIP(dhcp_t *dhcp, ip_t address)
{
	int height = 32 - dhcp->num_static_bits;

	assert(dhcp != 0);

	RecDhcpReleaseIP(dhcp->root, FlipBinary(address & ~dhcp->net, height ), height);
}

