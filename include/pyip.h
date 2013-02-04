#ifndef PYTINS_IP_H
#define PYTINS_IP_H

#include <tins/pdu.h>
#include "pypdu.h"

class PyIP : public PyPDU {
public:
    PyIP(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
};

#endif // PYTINS_IP_H
