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

#include "utils.h"
#include "pyrawpdu.h"

using Tins::RawPDU;

PyRawPDU::PyRawPDU(Tins::PDU *pdu) 
: ClonablePyPDU<PyRawPDU>(pdu) 
{ 
    
}
    
PyRawPDU::PyRawPDU(const std::string &str)
: ClonablePyPDU<PyRawPDU>(new RawPDU(str)) 
{
    
}

PyRawPDU::iterator PyRawPDU::begin() {
    return static_cast<RawPDU*>(pdu())->payload().begin();
}

PyRawPDU::iterator PyRawPDU::end() {
    return static_cast<RawPDU*>(pdu())->payload().end();
}

std::string PyRawPDU::to_string() {
    return std::string(
        (const char *)&begin()[0], 
        (const char *)&end()[0]
    );
}

std::string PyRawPDU::repr() {
    return "Raw(" + PyUtils::string_repr(to_string()) + ")";
}

void PyRawPDU::python_register() {
    using namespace boost::python;
    
    class_<PyRawPDU, bases<PyPDU> >("RawPDU", init<const std::string&>())
        .def("__iter__", boost::python::iterator<PyRawPDU>())
        .def("__str__", &PyRawPDU::to_string)
        .def("__repr__", &PyRawPDU::repr)
        .setattr("pdu_type", Tins::PDU::RAW)
    ;
}
