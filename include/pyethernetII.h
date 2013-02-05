#ifndef PYTINS_ETHERNETII_H
#define PYTINS_ETHERNETII_H

#include <tins/pdu.h>
#include "pypdu.h"

struct eth2_tag {};
typedef generic_pdu<eth2_tag> PyEthernetII;

#endif // PYTINS_ETHERNETII_H
