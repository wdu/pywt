// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef PYEXCEPTION_H
#define PYEXCEPTION_H

#include <Python.h>
#include <exception>
#include <string>

/***************** Python Exception *************/
// This exception is used report a python error to
// calling C++ code. The error information is 
// previously set by one of the PyErr_XXX functions

class PyException: public std::exception {
};

std::string getExceptionMessage(PyObject* _type, PyObject* _value, PyObject* trace);



#endif
