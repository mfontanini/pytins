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

#include <tins/hw_address.h>
#include "pyethernetII.h"

using Tins::EthernetII;
using Tins::NetworkInterface;

PyEthernetII::PyEthernetII(const address_type &dst_addr, 
const address_type &src_addr) 
: ClonablePyPDU<PyEthernetII>(new EthernetII(dst_addr, src_addr)) 
{
    
}

PyEthernetII::PyEthernetII(Tins::PDU *pdu) 
: ClonablePyPDU<PyEthernetII>(pdu) 
{ 
    
}

void PyEthernetII::python_register() {
    using namespace boost::python;
    
    class_<PyEthernetII, bases<PyPDU> >("EthernetII")
        .def(init<optional<address_type, address_type> >())
        PYTINS_MAKE_ATTR(uint16_t, EthernetII, payload_type)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, src_addr)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, dst_addr)
        .setattr("pdu_type", Tins::PDU::ETHERNET_II)
    ;
}
