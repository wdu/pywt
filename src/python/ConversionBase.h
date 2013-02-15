// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef CONVERSIONBASE_H
#define CONVERSIONBASE_H

#include <Python.h>
#include "PyException.h"

inline void typeError(PyObject* arg, const char* expected) {
	PyErr_Format(PyExc_TypeError, "Wrong argument type: got '%s' while expecting '%s',", arg->ob_type->tp_name, expected);
	throw PyException();
}

template <class T> 
class Python2C {
};


template <class T> 
class C2Python {
};



#endif
