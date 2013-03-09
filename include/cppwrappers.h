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
#include <list>
#include <utility>
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
    boost::python::list convert(const std::list<T> &input) {
        boost::python::list result;
        size_t i = 0;
        for(auto iter = input.begin(); iter != input.end(); ++iter, ++i)
            result.insert(i, *iter);
        return result;
    }
    
    template<typename T>
    std::vector<T> convert2vector(const boost::python::list &lst) {
        std::vector<T> result;
        result.reserve(len(lst));
        for (int i = 0; i < len(lst); ++i) {
            result.push_back(boost::python::extract<T>(lst[i]));
        }
        return result;
    }
    
    template<typename T>
    std::list<T> convert2list(const boost::python::list &lst) {
        std::list<T> result;
        for (int i = 0; i < len(lst); ++i) {
            result.push_back(boost::python::extract<T>(lst[i]));
        }
        return result;
    }
    
    inline boost::python::list convert(const uint8_t *input, size_t sz) {
        boost::python::list result;
        for(size_t i = 0; i < sz; ++i)
            result.insert(i, input[i]);
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

template<typename _, typename Class, typename Function>
struct conversion_getter_wrapper {
    typedef boost::python::list result_type;
    typedef Function function_type;
    
    conversion_getter_wrapper(function_type function)
    : function(function) { }

    result_type operator()(PyPDU *pdu) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        return Conversions::convert((cl->*function)());
    }

    function_type function;
};


template<typename T, typename Class, typename Function>
struct getter_wrapper<std::vector<T>, Class, Function> 
: public conversion_getter_wrapper<T, Class, Function> 
{
    getter_wrapper(Function function)
    : conversion_getter_wrapper<T, Class, Function>(function) { }
};

template<typename T, typename Class, typename Function>
struct getter_wrapper<std::list<T>, Class, Function> 
: public conversion_getter_wrapper<T, Class, Function> 
{
    getter_wrapper(Function function)
    : conversion_getter_wrapper<T, Class, Function>(function) { }
};

template<typename Class, typename Function>
struct getter_wrapper<const uint8_t*, Class, Function> {
    typedef boost::python::list result_type;
    typedef Function function_type;
    
    getter_wrapper(function_type function, size_t sz)
    : function(function), size(sz) { }

    result_type operator()(PyPDU *pdu) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        return Conversions::convert((cl->*function)(), size);
    }

    function_type function;
    size_t size;
};


// ***************************** Setters *******************************

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
        (cl->*function)(Conversions::convert2vector<T>(lst));
    }

    function_type function;
};

template<typename T, typename Class>
struct setter_wrapper<const std::list<T>&, Class> {
    typedef boost::python::list arg_type;
    typedef void (Class::*function_type)(const std::list<T>&);
    
    setter_wrapper(function_type function)
    : function(function) { }
    
    void operator()(PyPDU *pdu, const arg_type &lst) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        (cl->*function)(Conversions::convert2list<T>(lst));
    }

    function_type function;
};

template<typename Class>
struct setter_wrapper<const uint8_t*, Class> {
    typedef boost::python::list arg_type;
    typedef void (Class::*function_type)(const uint8_t *);
    
    setter_wrapper(function_type function, size_t min_sz)
    : function(function), minimum_sz(min_sz) { }
    
    void operator()(PyPDU *pdu, const arg_type &lst) const {
        auto cl = dereference_wrapper<Class>::dereference(pdu);
        auto converted = Conversions::convert2vector<uint8_t>(lst);
        if(converted.size() < minimum_sz)
            throw std::runtime_error("Buffer too small");
        (cl->*function)(&converted[0]);
    }

    function_type function;
    size_t minimum_sz;
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

template<typename Result, typename Class, typename... Args>
getter_wrapper<Result, Class, Result (Class::*)() const> 
make_getter_wrapper(Result (Class::*fun)() const, Args&&... args) {
    return getter_wrapper<Result, Class, Result (Class::*)() const>(fun, std::forward<Args>(args)...);
}

template<typename Result, typename Class, typename... Args>
getter_wrapper<Result, Class, Result (Class::*)()> 
make_getter_wrapper(Result (Class::*fun)(), Args&&... args) {
    return getter_wrapper<Result, Class, Result (Class::*)()>(fun, std::forward<Args>(args)...);
}

template<typename ArgType, typename Class, typename... Args>
setter_wrapper<ArgType, Class> 
make_setter_wrapper(void (Class::*fun)(ArgType), Args&&... args) {
    return setter_wrapper<ArgType, Class>(fun, std::forward<Args>(args)...);
}

template<typename Class, typename... Args>
setter_wrapper<void, Class> 
make_setter_wrapper(void (Class::*fun)(), Args&&... args) {
    return setter_wrapper<void, Class>(fun, std::forward<Args>(args)...);
}



#include <boost/python.hpp>
#include <boost/python/def.hpp>

#define PYTINS_GETTER_FUN(RETURN, CLASS, NAME) \
    make_getter_wrapper((RETURN (CLASS::*)() const)(&CLASS::NAME))

#define PYTINS_GETTER_FUN_ARG(RETURN, CLASS, NAME, ARG) \
    make_getter_wrapper((RETURN (CLASS::*)() const)(&CLASS::NAME), ARG)

#define PYTINS_SETTER_FUN(ARGTYPE, CLASS, NAME) \
    make_setter_wrapper((void (CLASS::*)(ARGTYPE))(&CLASS::NAME))
    
#define PYTINS_SETTER_FUN_ARG(ARGTYPE, CLASS, NAME, ARG) \
    make_setter_wrapper((void (CLASS::*)(ARGTYPE))(&CLASS::NAME), ARG)

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


#endif // PYTINS_CPPWRAPPERS_H
