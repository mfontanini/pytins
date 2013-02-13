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

#include <stdexcept>
#include "pyethernetII.h"
#include "pyip.h"
#include "pytcp.h"
#include "pyudp.h"
#include "pyicmp.h"
#include "pyrawpdu.h"
#include "pypdu.h"
#include "pyarp.h"
#include "pyipv6.h"

using Tins::PDU;

class NoInnerPDU : public std::exception {
public:
    const char *what() const throw() {
        return "No inner PDU set";
    }
};

class PDUNotFound : public std::exception {
public:
    const char *what() const throw() {
        return "PDU not found";
    }
};

PyPDU::PyPDU(const std::string &str) 
: pdu_(new Tins::RawPDU(str)) 
{
    
}

PyPDU::PyPDU(Tins::PDU *pdu_ptr) 
: pdu_(pdu_ptr) 
{ 
    
}

PyPDU::PyPDU(const PyPDU &rhs) {
    *this = rhs;
}

PyPDU& PyPDU::operator=(const PyPDU &rhs) {
    owns_pdu_ = false;
    pdu_.reset(rhs.pdu()->clone());
    return *this;
}

PyPDU::~PyPDU() {
    if(!owns_pdu_)
        pdu_.release();
}
    
uint32_t PyPDU::header_size() const {
    return pdu()->header_size();
}
    
PDU* PyPDU::clone() const {
    return pdu()->clone();
}
    
PDU::PDUType PyPDU::pdu_type() const {
    return pdu()->pdu_type();
}
    
PDU::serialization_type PyPDU::serialize() {
    return pdu()->serialize();
}

const PDU *PyPDU::pdu() const {
    return pdu_.get();
}
    
PDU *PyPDU::pdu() {
    return pdu_.get();
}

void PyPDU::set_owns_pdu(bool value) {
    owns_pdu_ = value;
}

PyPDU *PyPDU::find_pdu_by_type(PDU::PDUType type) {
    Tins::PDU *pdu_found = pdu_->find_pdu<PDU>(type);
    if(!pdu_found)
        throw PDUNotFound();
    PyPDU *result = PyPDU::from_pdu(pdu_found);
    result->set_owns_pdu(false);
    return result;
}
    
PyPDU *PyPDU::get_inner_pdu() {
    if(pdu_->inner_pdu()) {
        auto result = PyPDU::from_pdu(pdu_->inner_pdu());
        result->set_owns_pdu(false);
        return result;
    }
    else 
        throw NoInnerPDU();
}

void PyPDU::set_inner_pdu(PyPDU *pdu) {
    pdu_->inner_pdu(pdu->pdu()->clone());
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
        case Tins::PDU::ARP:
            return new PyARP(pdu);
        case Tins::PDU::RAW:
            return new PyRawPDU(pdu);
        case Tins::PDU::IPv6:
            return new PyIPv6(pdu);
        default:
            return 0;
    };
}

PyPDU *PyPDU::clone() {
    return clone_impl(pdu_->clone());
}

PyPDU *PyPDU::operator/(const PyPDU &rhs) {
    PyPDU *copy = clone();
    *copy /= rhs;
    return copy;
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
    
    class_<PyPDU, boost::noncopyable>("PDU", no_init)
        .def("serialize", make_getter_wrapper(&PyPDU::serialize))
        .add_property(
            "inner_pdu", 
            make_function(&PyPDU::get_inner_pdu, return_value_policy<manage_new_object>()), 
            &PyPDU::set_inner_pdu
        )
        .def(self /= other<PyPDU>())
        .def("__div__", &PyPDU::operator/, return_value_policy<manage_new_object>())
        .def("find_pdu_by_type", &PyPDU::find_pdu_by_type, return_value_policy<manage_new_object>())
    ;
    
    
    enum_<PDU::PDUType>("PDUType")
        .value("RAW", PDU::RAW)
        .value("ETHERNET_II", PDU::ETHERNET_II)
        .value("LLC", PDU::LLC)
        .value("SNAP", PDU::SNAP)
        .value("ARP", PDU::ARP)
        .value("TCP", PDU::TCP)
        .value("UDP", PDU::UDP)
        .value("ICMP", PDU::ICMP)
        .value("BOOTP", PDU::BOOTP)
        .value("DHCP", PDU::DHCP)
        .value("EAPOL", PDU::EAPOL)
        .value("RC4EAPOL", PDU::RC4EAPOL)
        .value("RSNEAPOL", PDU::RSNEAPOL)
        .value("DNS", PDU::DNS)
        .value("LOOPBACK", PDU::LOOPBACK)
        .value("IPv6", PDU::IPv6)
        .value("ICMPv6", PDU::ICMPv6)
        .value("SLL", PDU::SLL)
        .value("DHCPv6", PDU::DHCPv6)
    ;
    
    class_<PDUNotFound>("PDUNotFound", no_init)
        .def("__str__", &PDUNotFound::what)
    ;
    
    class_<NoInnerPDU>("NoInnerPDU", no_init)
        .def("__str__", &NoInnerPDU::what)
    ;
    
    implicitly_convertible<std::string, PyPDU>();
}
