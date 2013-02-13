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

#ifndef PYTINS_PYPDU_H
#define PYTINS_PYPDU_H

#include <memory>
#include <vector>
#include <tins/pdu.h>
#include <tins/small_uint.h>
#include <tins/packet.h>

class PyPDU { 
public:
    static PyPDU *from_pdu(Tins::PDU *pdu);
    static void python_register();

    PyPDU() : owns_pdu_(false) { }

    PyPDU(const std::string &str);

    PyPDU(Tins::PDU *pdu_ptr);
    
    PyPDU(const PyPDU &rhs);
    PyPDU& operator=(const PyPDU &rhs);
    
    // we haz dtor, so we defaultz da movz
    PyPDU(PyPDU&&) = default;
    
    PyPDU& operator=(PyPDU&&) = default;
    
    virtual ~PyPDU();

    uint32_t header_size() const;
    
    Tins::PDU* clone() const;
    
    Tins::PDU::PDUType pdu_type() const;
    
    Tins::PDU::serialization_type serialize();
    
    const Tins::PDU *pdu() const;
    
    Tins::PDU *pdu();
    
    PyPDU *get_inner_pdu();
    
    // pdu is cloned
    void set_inner_pdu(PyPDU *pdu);
    
    PyPDU *find_pdu_by_type(Tins::PDU::PDUType type);
    
    PyPDU *clone();
    
    PyPDU *operator/(const PyPDU &rhs);
    PyPDU &operator/=(const PyPDU &rhs);
protected:
    virtual PyPDU *clone_impl(Tins::PDU *pdu) {
        return new PyPDU(pdu);
    }
    void set_pdu(Tins::PDU *apdu);
private:
    void set_owns_pdu(bool value = true);

    std::unique_ptr<Tins::PDU> pdu_;
    bool owns_pdu_;
};

template<typename T>
class generic_pdu : public PyPDU {
public:
    generic_pdu(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
};

#include "cppwrappers.h"

template<typename Derived>
class ClonablePyPDU : public PyPDU {
public:
    ClonablePyPDU(Tins::PDU *pdu) : PyPDU(pdu) {}
    
    PyPDU *clone_impl(Tins::PDU *pdu) {
        return new Derived(pdu);
    }
};

class PyPacket {
public:
    PyPacket(const Tins::PtrPacket &pck) 
    : pdu_(PyPDU::from_pdu(const_cast<Tins::PDU*>(pck.pdu()))) { 
    }
    
    PyPacket(const PyPacket &rhs) {
        *this = rhs;
    }
    
    PyPacket& operator=(const PyPacket &rhs) {
        pdu_.reset(rhs.pdu_->clone());
        return *this;
    }
    
    PyPacket(PyPacket &&) = default;
    PyPacket& operator=(PyPacket &&) = default;
    
    PyPacket(const Tins::PDU &pck) 
    : pdu_(PyPDU::from_pdu(pck.clone())) { 
    }
    
    PyPDU& pdu() {
        return *pdu_;
    }
private:
    std::unique_ptr<PyPDU> pdu_;
};

#include <boost/python.hpp>
#include <boost/python/def.hpp>

#define PYTINS_GETTER_FUN(RETURN, CLASS, NAME) \
    make_getter_wrapper((RETURN (CLASS::*)() const)(&CLASS::NAME))

#define PYTINS_SETTER_FUN(ARGTYPE, CLASS, NAME) \
    make_setter_wrapper((void (CLASS::*)(ARGTYPE))(&CLASS::NAME))

#define PYTINS_MAKE_ATTR(ATTRTYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(ATTRTYPE, CLASS, NAME), PYTINS_SETTER_FUN(ATTRTYPE, CLASS, NAME))

#define PYTINS_MAKE_ATTR_POLICY(ATTRTYPE, CLASS, NAME, POLICY) \
    .add_property( \
        #NAME, \
        make_function(PYTINS_GETTER_FUN(ATTRTYPE, CLASS, NAME), POLICY), \
        make_function(PYTINS_SETTER_FUN(ATTRTYPE, CLASS, NAME), POLICY)\
    )

#define PYTINS_MAKE_ATTR2(GETTER_TYPE, SETTER_TYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(GETTER_TYPE, CLASS, NAME), PYTINS_SETTER_FUN(SETTER_TYPE, CLASS, NAME))

#define PYTINS_MAKE_GETTER_SETTER2(GETTER_TYPE, SETTER_TYPE, CLASS, NAME) \
    .def("get_"#NAME, PYTINS_GETTER_FUN(GETTER_TYPE, CLASS, NAME)) \
    .def("set_"#NAME, PYTINS_SETTER_FUN(SETTER_TYPE, CLASS, NAME))

#define PYTINS_MAKE_GETTER_SETTER(TYPE, CLASS, NAME) \
    PYTINS_MAKE_GETTER_SETTER2(TYPE, TYPE, CLASS, NAME)

#endif // PYTINS_PYPDU_H
