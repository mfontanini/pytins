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

#include "pypacketwriter.h"
#include "pypdu.h"

using namespace boost::python;

PyPacketWriter::PyPacketWriter(const std::string &path, LinkType lt)
: writer(path, lt)
{
    
}

void PyPacketWriter::write(PyPDU &pdu) {
    writer.write(*pdu.pdu());
}

void PyPacketWriter::write_all(boost::python::list &lst) {
    for (int i = 0; i < len(lst); ++i) {
        write(extract<PyPDU&>(lst[i]));
    }
}

void PyPacketWriter::python_register() {
    enum_<LinkType>("LinkType")
        .value("ETH2", LinkType::ETH2)
        .value("SLL", LinkType::SLL)
        .value("RADIOTAP", LinkType::RADIOTAP)
        .value("DOT11", LinkType::DOT11)
    ;
    
    class_<PyPacketWriter, boost::noncopyable >("PacketWriter", init<std::string, LinkType>())
        .def("write", &PyPacketWriter::write)
        .def("write_all", &PyPacketWriter::write_all)
    ;
}
