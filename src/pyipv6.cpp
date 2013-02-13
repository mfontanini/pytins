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
 
#include "pyipv6.h"

using namespace Tins;

PyIPv6::PyIPv6(const address_type &dst_addr, const address_type &src_addr) 
: ClonablePyPDU<PyIPv6>(new IPv6(dst_addr, src_addr))
{
      
}

PyIPv6::PyIPv6(Tins::PDU *pdu) 
: ClonablePyPDU<PyIPv6>(pdu) {
    
}

void PyIPv6::python_register() {
    using namespace boost::python;
    
    class_<PyIPv6, bases<PyPDU> >("IPv6", init<optional<address_type, address_type> >())
        PYTINS_MAKE_ATTR2(IPv6::address_type, const IPv6::address_type&, IPv6, src_addr)
        PYTINS_MAKE_ATTR2(IPv6::address_type, const IPv6::address_type&, IPv6, dst_addr)
        PYTINS_MAKE_ATTR(small_uint<4>, IPv6, version)
        PYTINS_MAKE_ATTR(uint8_t, IPv6, traffic_class)
        PYTINS_MAKE_ATTR(small_uint<20>, IPv6, flow_label)
        PYTINS_MAKE_ATTR(uint16_t, IPv6, payload_length)
        PYTINS_MAKE_ATTR(uint8_t, IPv6, next_header)
        PYTINS_MAKE_ATTR(uint8_t, IPv6, hop_limit)
        PYTINS_MAKE_ATTR(small_uint<4>, IPv6, version)
        .setattr("pdu_type", Tins::PDU::IPv6)
    ;
}
