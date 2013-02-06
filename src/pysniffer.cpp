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

#include <boost/python/call.hpp>
#include "pysniffer.h"
#include "pypdu.h"

using namespace boost::python;

struct callback_proxy {
    PyObject* obj;
    
    callback_proxy(PyObject* obj) : obj(obj) { }
    
    bool operator()(Tins::RefPacket &pck) {
        PyPacket packet(pck.pdu());
        return call<bool>(obj, packet);
    }
};

PySniffer::PySniffer(const std::string &device, unsigned max_packet_size,
  bool promisc, const std::string &filter)
: sniffer(device, max_packet_size, promisc, filter) 
{
    
}
    
PyPacket PySniffer::next_packet() {
    return sniffer.next_packet();
}
    
void PySniffer::sniff_loop(PyObject* callback_obj, size_t max_packets)
{
    callback_proxy handler(callback_obj);
    sniffer.sniff_loop(handler, max_packets);
}

PyFileSniffer::PyFileSniffer(const std::string &file_name, const std::string &filter)
: sniffer(file_name, filter) 
{
    
}
    
PyPacket PyFileSniffer::next_packet() {
    return sniffer.next_packet();
}

void PyFileSniffer::sniff_loop(PyObject* callback_obj, size_t max_packets)
{
    callback_proxy handler(callback_obj);
    sniffer.sniff_loop(handler, max_packets);
}
