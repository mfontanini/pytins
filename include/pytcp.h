#ifndef PYTINS_TCP_H
#define PYTINS_TCP_H

#include <tins/pdu.h>
#include "pypdu.h"

struct tcp_tag {};
typedef generic_pdu<tcp_tag> PyTCP;

#endif // PYTINS_TCP_H
