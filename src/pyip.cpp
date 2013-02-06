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

#include "pyip.h"

using Tins::IP;
using Tins::small_uint;

PyIP::PyIP(const address_type &dst_addr, const address_type &src_addr) 
: PyPDU(new IP(dst_addr, src_addr))
{
    
}

PyIP::PyIP(Tins::PDU *pdu) 
: PyPDU(pdu)
{
    
}

void PyIP::python_register() {
    using namespace boost::python;
    
    class_<PyIP, bases<PyPDU> >("IP", init<optional<address_type, address_type> >())
        PYTINS_MAKE_ATTR(IP::address_type, IP, src_addr)
        PYTINS_MAKE_ATTR(IP::address_type, IP, dst_addr)
        PYTINS_MAKE_ATTR(small_uint<4>, IP, head_len)
        PYTINS_MAKE_ATTR(small_uint<4>, IP, version)
        PYTINS_MAKE_ATTR(uint8_t, IP, tos)
        PYTINS_MAKE_ATTR(uint16_t, IP, tot_len)
        PYTINS_MAKE_ATTR(uint16_t, IP, id)
        PYTINS_MAKE_ATTR(uint16_t, IP, frag_off)
        PYTINS_MAKE_ATTR(uint8_t, IP, ttl)
        PYTINS_MAKE_ATTR(uint8_t, IP, protocol)
        PYTINS_MAKE_ATTR(uint16_t, IP, check)
    ;
}
