// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef ARG_H
#define ARG_H


#include <Python.h>
#include "ConversionBase.h"

template <class TYPE>
class Arg {
	public:
		inline Arg(PyObject* obj): v(Python2C<TYPE>::arg(obj)) {
		}
		inline Arg(PyObject* obj, TYPE def): v((obj)?(Python2C<TYPE>::arg(obj)):def) {
		}
		typename Python2C<TYPE>::S v;
};


//template <>
template <class TYPE>
class Arg<const TYPE&> {
	public:
		inline Arg(PyObject* obj): v_ptr(0), v(Python2C<const TYPE&>::arg(obj, &v_ptr)) {
		}
		inline Arg(PyObject* obj, const TYPE& def): v_ptr(0), v((obj)?(Python2C<const TYPE&>::arg(obj, &v_ptr)):def) {
		}
		TYPE* v_ptr;

		typename Python2C<const TYPE&>::S v;

		inline ~Arg() {
			delete v_ptr;
		}
};




#endif
