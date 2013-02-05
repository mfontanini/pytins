#ifndef PYTINS_IP_H
#define PYTINS_IP_H

#include <tins/pdu.h>
#include "pypdu.h"

struct ip_tag {};
typedef generic_pdu<ip_tag> PyIP;

#endif // PYTINS_IP_H
