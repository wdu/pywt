// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef COMPARE_H
#define COMPARE_H

#include <Python.h>

// rich comparison based on ptr of the underlying core object
PyObject* compare_ptr(PyObject* a, PyObject* b, int op);

// rich comparison based on == operator of underlying core class T
// <, > comparison is undefined
template <class T>
PyObject* compare_val_eq(PyObject* a, PyObject* b, int op);

// rich comparison based on == and < operators of underlying core class T
template <class T>
PyObject* compare_val_eq_lt(PyObject* a, PyObject* b, int op);


#endif
