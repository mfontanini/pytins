#ifndef PYTINS_UDP_H
#define PYTINS_UDP_H

#include <tins/pdu.h>
#include "pypdu.h"

struct udp_tag {};
typedef generic_pdu<udp_tag> PyUDP;

#endif // PYTINS_UDP_H
