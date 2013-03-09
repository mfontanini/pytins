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
 
#include <tins/dhcp.h>
#include "pydhcp.h"

using Tins::DHCP;

PyDHCP::PyDHCP(Tins::PDU *pdu) 
: ClonablePyPDU<PyDHCP>(pdu)
{
    
}

PyDHCP::PyDHCP() 
: ClonablePyPDU<PyDHCP>(new DHCP())
{
    
}

void PyDHCP::python_register() {
    using namespace boost::python;
    typedef DHCP::ipaddress_type ipaddress_type;
    typedef DHCP::chaddr_type chaddr_type;
    
    class_<PyDHCP, bases<PyPDU> >("DHCP", init<>())
        PYTINS_MAKE_ATTR(uint8_t, DHCP, opcode)
        PYTINS_MAKE_ATTR(uint8_t, DHCP, htype)
        PYTINS_MAKE_ATTR(uint8_t, DHCP, hlen)
        PYTINS_MAKE_ATTR(uint8_t, DHCP, hops)
        PYTINS_MAKE_ATTR(uint32_t, DHCP, xid)
        PYTINS_MAKE_ATTR(uint16_t, DHCP, secs)
        PYTINS_MAKE_ATTR(uint16_t, DHCP, padding)
        PYTINS_MAKE_ATTR(ipaddress_type, DHCP, ciaddr)
        PYTINS_MAKE_ATTR(ipaddress_type, DHCP, yiaddr)
        PYTINS_MAKE_ATTR(ipaddress_type, DHCP, siaddr)
        PYTINS_MAKE_ATTR(ipaddress_type, DHCP, giaddr)
        PYTINS_MAKE_ATTR2(chaddr_type, const chaddr_type&, DHCP, chaddr)
        .add_property( 
            "sname",
            PYTINS_GETTER_FUN_ARG(const uint8_t*, DHCP, sname, 64), 
            PYTINS_SETTER_FUN_ARG(const uint8_t*, DHCP, sname, 64)
        )
        .add_property( 
            "file",
            PYTINS_GETTER_FUN_ARG(const uint8_t*, DHCP, file, 128), 
            PYTINS_SETTER_FUN_ARG(const uint8_t*, DHCP, file, 128)
        )
        PYTINS_MAKE_GETTER_SETTER(DHCP::ipaddress_type, DHCP, server_identifier)
        PYTINS_MAKE_GETTER_SETTER(uint32_t, DHCP, lease_time)
        PYTINS_MAKE_GETTER_SETTER(uint32_t, DHCP, renewal_time)
        PYTINS_MAKE_GETTER_SETTER(uint32_t, DHCP, rebind_time)
        PYTINS_MAKE_GETTER_SETTER(DHCP::ipaddress_type, DHCP, subnet_mask)
        PYTINS_MAKE_GETTER_SETTER2(std::list<ipaddress_type>, const std::list<ipaddress_type>&, DHCP, routers)
        PYTINS_MAKE_GETTER_SETTER2(std::list<ipaddress_type>, const std::list<ipaddress_type>&, DHCP, domain_name_servers)
        PYTINS_MAKE_GETTER_SETTER(DHCP::ipaddress_type, DHCP, broadcast)
        PYTINS_MAKE_GETTER_SETTER(DHCP::ipaddress_type, DHCP, requested_ip)
        PYTINS_MAKE_GETTER_SETTER2(std::string, const std::string&, DHCP, domain_name)
    ;
}
