#ifndef PYTINS_RAWPDU_H
#define PYTINS_RAWPDU_H

#include <string>
#include <tins/pdu.h>
#include <tins/rawpdu.h>
#include "pypdu.h"
#include "utils.h"

class PyRawPDU : public PyPDU {
public:
    typedef Tins::RawPDU::payload_type::iterator iterator;

    PyRawPDU(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
    
    PyRawPDU(const std::string &str)
    : PyPDU(new Tins::RawPDU(str)) {}
    
    iterator begin() {
        return static_cast<Tins::RawPDU*>(pdu())->payload().begin();
    }

    iterator end() {
        return static_cast<Tins::RawPDU*>(pdu())->payload().end();
    }
    
    std::string to_string() {
        return std::string(
            (const char *)&begin()[0], 
            (const char *)&end()[0]
        );
    }
    
    std::string repr() {
        return "Raw(" + PyUtils::string_repr(to_string()) + ")";
    }
};

#endif // PYTINS_RAWPDU_H
