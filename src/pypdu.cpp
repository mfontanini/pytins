/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "pyethernetII.h"
#include "pyip.h"
#include "pytcp.h"
#include "pyudp.h"
#include "pyicmp.h"
#include "pyrawpdu.h"
#include "pypdu.h"

PyPDU::PyPDU(Tins::PDU *pdu_ptr) : pdu_(pdu_ptr) { }

PyPDU::~PyPDU() {}
    
uint32_t PyPDU::header_size() const {
    return pdu()->header_size();
}
    
Tins::PDU* PyPDU::clone() const {
    return pdu()->clone();
}
    
Tins::PDU::PDUType PyPDU::pdu_type() const {
    return pdu()->pdu_type();
}
    
Tins::PDU::serialization_type PyPDU::serialize() {
    return pdu()->serialize();
}
    
const Tins::PDU *PyPDU::pdu() const {
    return pdu_.get();
}
    
Tins::PDU *PyPDU::pdu() {
    return pdu_.get();
}
    
PyPDU *PyPDU::inner_pdu() {
    return PyPDU::from_pdu(pdu_->inner_pdu());
}

void PyPDU::set_pdu(Tins::PDU *apdu) {
    pdu_.reset(apdu);
}

PyPDU *PyPDU::from_pdu(Tins::PDU *pdu) {
    switch(pdu->pdu_type()) {
        case Tins::PDU::ETHERNET_II:
            return new PyEthernetII(pdu);
        case Tins::PDU::IP:
            return new PyIP(pdu);
        case Tins::PDU::TCP:
            return new PyTCP(pdu);
        case Tins::PDU::UDP:
            return new PyUDP(pdu);
        case Tins::PDU::ICMP:
            return new PyICMP(pdu);
        default:
            return 0;
    };
}

PyPDU PyPDU::operator/(const PyPDU &rhs) {
    PyPDU copy(*this);
    return copy /= rhs;
}

PyPDU &PyPDU::operator/=(const PyPDU &rhs) {
    Tins::PDU *this_pdu = pdu();
    while(this_pdu->inner_pdu())
        this_pdu = this_pdu->inner_pdu();
    this_pdu->inner_pdu(rhs.pdu()->clone());
    return *this;
}

void PyPDU::python_register()
{
    using namespace boost::python;
    
    class_<PyPDU>("PDU", no_init)
        .def("serialize", &PyPDU::serialize)
        .def("inner_pdu", &PyPDU::inner_pdu, return_value_policy<manage_new_object>())
        .def(self /= other<PyPDU>())
    ;
}
