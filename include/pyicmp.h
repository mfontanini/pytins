#ifndef PYTINS_ICMP_H
#define PYTINS_ICMP_H

#include <tins/pdu.h>
#include "pypdu.h"

struct icmp_tag {};
typedef generic_pdu<icmp_tag> PyICMP;

#endif // PYTINS_ICMP_H
