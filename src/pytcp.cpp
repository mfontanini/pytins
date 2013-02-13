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

#include <tins/tcp.h>
#include "pytcp.h"

using Tins::TCP;
    
PyTCP::PyTCP(uint16_t dport, uint16_t sport) 
: ClonablePyPDU<PyTCP>(new TCP(dport, sport))
{
    
}

PyTCP::PyTCP(Tins::PDU *pdu) 
: ClonablePyPDU<PyTCP>(pdu)
{
    
}

void PyTCP::python_register() {
    using namespace boost::python;
    
    class_<PyTCP, bases<PyPDU> >("TCP", init<optional<uint16_t, uint16_t> >())
        PYTINS_MAKE_ATTR(uint16_t, TCP, sport)
        PYTINS_MAKE_ATTR(uint16_t, TCP, dport)
        PYTINS_MAKE_ATTR(uint32_t, TCP, ack_seq)
        PYTINS_MAKE_ATTR(uint32_t, TCP, seq)
        PYTINS_MAKE_ATTR(uint16_t, TCP, window)
        PYTINS_MAKE_ATTR(uint16_t, TCP, check)
        PYTINS_MAKE_ATTR(uint16_t, TCP, urg_ptr)
        PYTINS_MAKE_GETTER_SETTER(uint16_t, TCP, mss)
        PYTINS_MAKE_GETTER_SETTER(uint8_t, TCP, winscale)
        .def("has_sack_permitted", make_getter_wrapper(&TCP::has_sack_permitted))
        .def("set_sack_permitted", make_setter_wrapper(&TCP::sack_permitted))
        PYTINS_MAKE_GETTER_SETTER2(TCP::sack_type, const TCP::sack_type&, TCP, sack)
        PYTINS_MAKE_GETTER_SETTER(TCP::AltChecksums, TCP, altchecksum)
        .setattr("pdu_type", Tins::PDU::TCP)
    ;
    
    enum_<TCP::AltChecksums>("TCPAltChecksums")
        .value("CHK_TCP", TCP::CHK_TCP)
        .value("CHK_8FLETCHER", TCP::CHK_8FLETCHER)
        .value("CHK_16FLETCHER", TCP::CHK_16FLETCHER)
    ;
}
