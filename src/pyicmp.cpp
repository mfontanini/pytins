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
 
#include "pyicmp.h"

using Tins::ICMP;

PyICMP::PyICMP(ICMP::Flags flag) 
: PyPDU(new ICMP(flag))
{
    
}

PyICMP::PyICMP(Tins::PDU *pdu) 
: PyPDU(pdu)
{
    
}

void PyICMP::python_register() {
    using namespace boost::python;
    
    class_<PyICMP, bases<PyPDU> >("ICMP", init<optional<ICMP::Flags> >())
        PYTINS_MAKE_ATTR(uint8_t, ICMP, code)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, check)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, id)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, mtu)
        PYTINS_MAKE_ATTR(uint8_t, ICMP, pointer)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, check)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, sequence)
        PYTINS_MAKE_ATTR(uint32_t, ICMP, gateway)
        PYTINS_MAKE_ATTR(ICMP::Flags, ICMP, type)
    ;
    
    enum_<ICMP::Flags>("ICMPFlags")
        .value("ECHO_REPLY", ICMP::ECHO_REPLY)
        .value("DEST_UNREACHABLE", ICMP::DEST_UNREACHABLE)
        .value("SOURCE_QUENCH", ICMP::SOURCE_QUENCH)
        .value("REDIRECT", ICMP::REDIRECT)
        .value("ECHO_REQUEST", ICMP::ECHO_REQUEST)
        .value("TIME_EXCEEDED", ICMP::TIME_EXCEEDED)
        .value("PARAM_PROBLEM", ICMP::PARAM_PROBLEM)
        .value("INFO_REQUEST", ICMP::INFO_REQUEST)
        .value("INFO_REPLY", ICMP::INFO_REPLY)
    ;
}
