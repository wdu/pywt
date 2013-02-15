// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef IMPLICIT_CONVERSION_H
#define IMPLICIT_CONVERSION_H

#include <Python.h>

template <class T>
class ImplicitConvert {
	public:
		static bool accepts(PyObject* in) {
			return false;
		}
		static const T& convert(PyObject* in, T** ptr) {
			throw PyException();
		}
};

#include <Wt/WLength>
template <>
class ImplicitConvert<Wt::WLength> {
	public:
		static bool accepts(PyObject* in);
		static const Wt::WLength& convert(PyObject* in, Wt::WLength** ptr);
};

#include <Wt/WString>
template <>
class ImplicitConvert<Wt::WString> {
	public:
		static bool accepts(PyObject* in);
		static const Wt::WString& convert(PyObject* in, Wt::WString** ptr);
};


#endif
