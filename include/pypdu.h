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

#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <tins/pdu.h>
#include <tins/small_uint.h>
#include <tins/packet.h>

class PyPDU { 
public:
    static PyPDU *from_pdu(Tins::PDU *pdu);
    static void python_register();

    PyPDU(Tins::PDU *pdu_ptr);
    
    PyPDU() {}
    
    virtual ~PyPDU();

    uint32_t header_size() const;
    
    Tins::PDU* clone() const;
    
    Tins::PDU::PDUType pdu_type() const;
    
    Tins::PDU::serialization_type serialize();
    
    const Tins::PDU *pdu() const;
    
    Tins::PDU *pdu();
    
    PyPDU *inner_pdu();
    
    PyPDU operator/(const PyPDU &rhs);
    PyPDU &operator/=(const PyPDU &rhs);
protected:
    void set_pdu(Tins::PDU *apdu);
private:
    boost::shared_ptr<Tins::PDU> pdu_;
};

template<typename T>
class generic_pdu : public PyPDU {
public:
    generic_pdu(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
};

template<typename T>
struct type_filter {
    typedef T type;
};

template<size_t n>
struct type_filter<Tins::small_uint<n> > {
    typedef typename Tins::small_uint<n>::repr_type type;
};


template<typename Result, typename Class>
struct getter_wrapper {
    typedef typename type_filter<Result>::type result_type;
    typedef Result (Class::*function_type)() const;
    
    getter_wrapper(function_type function)
    : function(function) { }

    result_type operator()(const PyPDU *pdu) const {
        const Class *cl = static_cast<const Class*>(pdu->pdu());
        return (cl->*function)();
    }

    function_type function;
};

template<typename ArgType, typename Class>
struct setter_wrapper {
    typedef typename type_filter<ArgType>::type arg_type;
    typedef void (Class::*function_type)(ArgType);
    
    setter_wrapper(function_type function)
    : function(function) { }

    void operator()(PyPDU *pdu, arg_type arg) const {
        Class *cl = static_cast<Class*>(pdu->pdu());
        (cl->*function)(arg);
    }

    function_type function;
};

namespace boost { namespace python { namespace detail {

    template <class Result, typename Class>
    inline boost::mpl::vector<typename getter_wrapper<Result, Class>::result_type, const PyPDU*>
    get_signature(getter_wrapper<Result, Class>, const PyPDU* = 0)
    {
        return boost::mpl::vector<
            typename getter_wrapper<Result, Class>::result_type, 
            const PyPDU*
        >();
    }
    
    template <class ArgType, typename Class>
    inline boost::mpl::vector<void, PyPDU*, typename setter_wrapper<ArgType, Class>::arg_type>
    get_signature(setter_wrapper<ArgType, Class>, PyPDU* = 0)
    {
        return boost::mpl::vector<
            void, 
            PyPDU*, 
            typename setter_wrapper<ArgType, Class>::arg_type
        >();
    }

} } }

template<typename Result, typename Class>
getter_wrapper<Result, Class> 
make_getter_wrapper(Result (Class::*fun)() const) {
    return getter_wrapper<Result, Class>(fun);
}

template<typename ArgType, typename Class>
setter_wrapper<ArgType, Class> 
make_setter_wrapper(void (Class::*fun)(ArgType)) {
    return setter_wrapper<ArgType, Class>(fun);
}

class PyPacket {
public:
    PyPacket(const Tins::PtrPacket &pck) 
    : pdu_(PyPDU::from_pdu(const_cast<Tins::PDU*>(pck.pdu()))) { 
    }
    
    PyPacket(const Tins::PDU &pck) 
    : pdu_(PyPDU::from_pdu(pck.clone())) { 
    }
    
    PyPDU& pdu() {
        return *pdu_;
    }
private:
    boost::shared_ptr<PyPDU> pdu_;
};

#include <boost/python.hpp>
#include <boost/python/def.hpp>

#define PYTINS_GETTER_FUN(RETURN, CLASS, NAME) \
    make_getter_wrapper((RETURN (CLASS::*)() const)(&CLASS::NAME))

#define PYTINS_SETTER_FUN(ARGTYPE, CLASS, NAME) \
    make_setter_wrapper((void (CLASS::*)(ARGTYPE))(&CLASS::NAME))

#define PYTINS_MAKE_ATTR(ATTRTYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(ATTRTYPE, CLASS, NAME), PYTINS_SETTER_FUN(ATTRTYPE, CLASS, NAME))

#define PYTINS_MAKE_ATTR2(GETTER_TYPE, SETTER_TYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(GETTER_TYPE, CLASS, NAME), PYTINS_SETTER_FUN(SETTER_TYPE, CLASS, NAME))

#endif // PYTINS_PYPDU_H
