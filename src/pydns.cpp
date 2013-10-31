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

#include <vector>
#include <tins/dns.h>
#include "pydns.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

   
PyDNS::PyDNS(Tins::PDU *pdu) 
: ClonablePyPDU<PyDNS>(pdu)
{
    
}
    
PyDNS::PyDNS() 
: ClonablePyPDU<PyDNS>(new Tins::DNS())
{
    
}

namespace Tins {
bool operator==(const Tins::DNS::Query &lhs, const Tins::DNS::Query &rhs)
{
    return lhs.query_class() == rhs.query_class() &&
            lhs.type() == rhs.type() &&
            lhs.dname() == rhs.dname();
}
}

void PyDNS::python_register() {
    using namespace boost::python;
    using Tins::DNS;
    
    class_<PyDNS, bases<PyPDU> >("DNS", init<>())
        PYTINS_MAKE_ATTR(uint16_t, DNS, id)
        PYTINS_MAKE_ATTR(DNS::QRType, DNS, type)
        PYTINS_MAKE_ATTR(uint8_t, DNS, opcode)
        PYTINS_MAKE_ATTR(uint8_t, DNS, authoritative_answer)
        PYTINS_MAKE_ATTR(uint8_t, DNS, truncated)
        PYTINS_MAKE_ATTR(uint8_t, DNS, recursion_available)
        PYTINS_MAKE_ATTR(uint8_t, DNS, recursion_desired)
        PYTINS_MAKE_ATTR(uint8_t, DNS, z)
        PYTINS_MAKE_ATTR(uint8_t, DNS, authenticated_data)
        PYTINS_MAKE_ATTR(uint8_t, DNS, checking_disabled)
        PYTINS_MAKE_ATTR(uint8_t, DNS, rcode)
        PYTINS_MAKE_ATTR(uint8_t, DNS, questions_count)
        PYTINS_MAKE_ATTR(uint8_t, DNS, answers_count)
        PYTINS_MAKE_ATTR(uint8_t, DNS, authority_count)
        PYTINS_MAKE_ATTR(uint8_t, DNS, additional_count)
        .def("add_query", PYTINS_SETTER_FUN(const DNS::Query&, DNS, add_query))
        .def("queries", PYTINS_GETTER_FUN(std::list<DNS::Query>, DNS, queries))
    ;
    
    class_<DNS::Query>("DNSQuery", init<const std::string &, DNS::QueryType, DNS::QueryClass>())
        .add_property(
            "dname", 
            make_function(
                (const std::string &(DNS::Query::*)() const)(&DNS::Query::dname), 
                return_value_policy<copy_const_reference>()
            )
        )
        .add_property("type", (DNS::QueryType(DNS::Query::*)() const)(&DNS::Query::type))
        .add_property("query_class", (DNS::QueryClass(DNS::Query::*)() const)(&DNS::Query::query_class))
    ;
    
    class_<DNS::Resource>("DNSResource", init<const std::string &, const std::string &, DNS::QueryType, DNS::QueryClass, uint32_t>())
        .add_property(
            "dname", 
            make_function(
                (const std::string &(DNS::Resource::*)() const)(&DNS::Resource::dname), 
                return_value_policy<copy_const_reference>()
            )
        )
        .add_property(
            "data", 
            make_function(
                (const std::string &(DNS::Resource::*)() const)(&DNS::Resource::data), 
                return_value_policy<copy_const_reference>()
            )
        )
        .add_property("type", (DNS::QueryType(DNS::Resource::*)() const)(&DNS::Resource::type))
        .add_property("query_class", (DNS::QueryClass(DNS::Resource::*)() const)(&DNS::Resource::query_class))
        .add_property("ttl", (uint32_t(DNS::Resource::*)() const)(&DNS::Resource::ttl))
    ;
    
    enum_<DNS::QueryClass>("DNSQClass")
        .value("IN", DNS::IN)
        .value("CH", DNS::CH)
        .value("HS", DNS::HS)
        .value("ANY", DNS::ANY)
    ;
    
    enum_<DNS::QueryType>("DNSQType")
        .value("A", DNS::A)
        .value("NS", DNS::NS)
        .value("MD", DNS::MD)
        .value("MF", DNS::MF)
        .value("CNAME", DNS::CNAME)
        .value("SOA", DNS::SOA)
        .value("MB", DNS::MB)
        .value("MG", DNS::MG)
        .value("MR", DNS::MR)
        .value("NULL_R", DNS::NULL_R)
        .value("WKS", DNS::WKS)
        .value("PTR", DNS::PTR)
        .value("HINFO", DNS::HINFO)
        .value("MINFO", DNS::MINFO)
        .value("MX", DNS::MX)
        .value("TXT", DNS::TXT)
        .value("RP", DNS::RP)
        .value("AFSDB", DNS::AFSDB)
        .value("X25", DNS::X25)
        .value("ISDN", DNS::ISDN)
        .value("RT", DNS::RT)
        .value("NSAP", DNS::NSAP)
        .value("NSAP_PTR", DNS::NSAP_PTR)
        .value("SIG", DNS::SIG)
        .value("KEY", DNS::KEY)
        .value("PX", DNS::PX)
        .value("GPOS", DNS::GPOS)
        .value("AAAA", DNS::AAAA)
        .value("LOC", DNS::LOC)
        .value("NXT", DNS::NXT)
        .value("EID", DNS::EID)
        .value("NIMLOC", DNS::NIMLOC)
        .value("SRV", DNS::SRV)
        .value("ATMA", DNS::ATMA)
        .value("NAPTR", DNS::NAPTR)
        .value("KX", DNS::KX)
        .value("CERT", DNS::CERT)
        .value("A6", DNS::A6)
        .value("DNAM", DNS::DNAM)
        .value("SINK", DNS::SINK)
        .value("OPT", DNS::OPT)
        .value("APL", DNS::APL)
        .value("DS", DNS::DS)
        .value("SSHFP", DNS::SSHFP)
        .value("IPSECKEY", DNS::IPSECKEY)
        .value("RRSIG", DNS::RRSIG)
        .value("NSEC", DNS::NSEC)
        .value("DNSKEY", DNS::DNSKEY)
        .value("DHCID", DNS::DHCID)
        .value("NSEC3", DNS::NSEC3)
        .value("NSEC3PARAM", DNS::NSEC3PARAM)
    ;
    
    class_<std::vector<DNS::Query>>("vector_dnsquery")
        .def(vector_indexing_suite<std::vector<DNS::Query>>())
    ;
}
