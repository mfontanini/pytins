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
 
#ifndef PYTINS_CPPWRAPPERS_H
#define PYTINS_CPPWRAPPERS_H

#include <vector>
#include <boost/mpl/vector.hpp>
#include <boost/python/list.hpp>

namespace Conversions {
    template<typename T>
    boost::python::list convert(const std::vector<T> &input) {
        boost::python::list result;
        for(size_t i = 0; i < input.size(); ++i)
            result.insert(i, input[i]);
        return result;
    }
    
    template<typename T>
    std::vector<T> convert(const boost::python::list &lst) {
        std::vector<T> result;
        result.reserve(len(lst));
        for (int i = 0; i < len(lst); ++i) {
            result.push_back(boost::python::extract<T>(lst[i]));
        }
        return result;
    }
}

template<typename T>
struct type_filter {
    typedef T type;
};

template<size_t n>
struct type_filter<Tins::small_uint<n> > {
    typedef typename Tins::small_uint<n>::repr_type type;
};

template<typename Class>
struct dereference_wrapper {
    static Class *dereference(PyPDU *input) {
        return static_cast<Class*>(input->pdu());
    }
};

template<>
struct dereference_wrapper<PyPDU> {
    static PyPDU *dereference(PyPDU *input) {
        return input;
    }
};

template<typename Result, typename Class, typename Function>
struct getter_wrapper {
    typedef typename type_filter<Result>::type result_type;
    typedef Function function_type;
    
    getter_wrapper(function_type function)
    : function(function) { }

    result_type operator()(PyPDU *pdu) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        return (cl->*function)();
    }

    function_type function;
};

template<typename T, typename Class, typename Function>
struct getter_wrapper<std::vector<T>, Class, Function> {
    typedef boost::python::list result_type;
    typedef Function function_type;
    
    getter_wrapper(function_type function)
    : function(function) { }

    result_type operator()(PyPDU *pdu) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        return Conversions::convert((cl->*function)());
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
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        (cl->*function)(arg);
    }

    function_type function;
};

template<typename Class>
struct setter_wrapper<void, Class> {
    typedef void arg_type;
    typedef void (Class::*function_type)();
    
    setter_wrapper(function_type function)
    : function(function) { }
    
    void operator()(PyPDU *pdu) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        (cl->*function)();
    }

    function_type function;
};

template<typename T, typename Class>
struct setter_wrapper<const std::vector<T>&, Class> {
    typedef boost::python::list arg_type;
    typedef void (Class::*function_type)(const std::vector<T>&);
    
    setter_wrapper(function_type function)
    : function(function) { }
    
    void operator()(PyPDU *pdu, const arg_type &lst) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        (cl->*function)(Conversions::convert<T>(lst));
    }

    function_type function;
};

namespace boost { namespace python { namespace detail {
    template <class Result, typename Class, typename Function>
    inline boost::mpl::vector<typename getter_wrapper<Result, Class, Function>::result_type, PyPDU*>
    get_signature(getter_wrapper<Result, Class, Function>, const PyPDU* = 0)
    {
        return boost::mpl::vector<
            typename getter_wrapper<Result, Class, Function>::result_type, 
            PyPDU*
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
    
    template <typename Class>
    inline boost::mpl::vector<void, PyPDU*>
    get_signature(setter_wrapper<void, Class>, PyPDU* = 0)
    {
        return boost::mpl::vector<
            void, 
            PyPDU*
        >();
    }

} } }

template<typename Result, typename Class>
getter_wrapper<Result, Class, Result (Class::*)() const> 
make_getter_wrapper(Result (Class::*fun)() const) {
    return getter_wrapper<Result, Class, Result (Class::*)() const>(fun);
}

template<typename Result, typename Class>
getter_wrapper<Result, Class, Result (Class::*)()> 
make_getter_wrapper(Result (Class::*fun)()) {
    return getter_wrapper<Result, Class, Result (Class::*)()>(fun);
}

template<typename ArgType, typename Class>
setter_wrapper<ArgType, Class> 
make_setter_wrapper(void (Class::*fun)(ArgType)) {
    return setter_wrapper<ArgType, Class>(fun);
}

template<typename Class>
setter_wrapper<void, Class> 
make_setter_wrapper(void (Class::*fun)()) {
    return setter_wrapper<void, Class>(fun);
}


#endif // PYTINS_CPPWRAPPERS_H
