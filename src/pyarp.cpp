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
 
#include "pyarp.h"

using Tins::ARP;

PyARP::PyARP(ipaddress_type target_ip, ipaddress_type sender_ip, 
  const hwaddress_type &target_hw, const hwaddress_type &sender_hw)
: PyPDU(new ARP(target_ip, sender_ip, target_hw, sender_hw))
{
    
}

PyARP::PyARP(Tins::PDU *pdu)
: PyPDU(pdu)
{
    
}

void PyARP::python_register() {
    using namespace boost::python;
    
    class_<PyARP, bases<PyPDU> >("ARP")
        .def(init<optional<ipaddress_type, ipaddress_type, hwaddress_type, hwaddress_type> >())
        PYTINS_MAKE_ATTR(uint16_t, ARP, hw_addr_format)
        PYTINS_MAKE_ATTR(uint16_t, ARP, prot_addr_format)
        PYTINS_MAKE_ATTR(uint8_t, ARP, hw_addr_length)
        PYTINS_MAKE_ATTR(uint8_t, ARP, prot_addr_length)
        PYTINS_MAKE_ATTR2(uint16_t, ARP::Flags, ARP, opcode)
        PYTINS_MAKE_ATTR(ipaddress_type, ARP, sender_ip_addr)
        PYTINS_MAKE_ATTR(ipaddress_type, ARP, target_ip_addr)
        PYTINS_MAKE_ATTR2(hwaddress_type, const hwaddress_type&, ARP, target_hw_addr)
        PYTINS_MAKE_ATTR2(hwaddress_type, const hwaddress_type&, ARP, sender_hw_addr)
    ;
    
    enum_<ARP::Flags>("ARPFlags")
        .value("REQUEST", ARP::REQUEST)
        .value("REPLY", ARP::REPLY)
    ;
}
