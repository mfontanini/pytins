#ifndef PYTINS_ETHERNETII_H
#define PYTINS_ETHERNETII_H

#include <tins/pdu.h>
#include "pypdu.h"

class PyEthernetII : public PyPDU {
public:
    PyEthernetII(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
};

#endif // PYTINS_ETHERNETII_H
