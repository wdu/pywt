// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#include <Wt/WLength>
bool ImplicitConvert<Wt::WLength>::accepts(PyObject* in) {
	return Python2C<int>::acceptarg(in);
}
const Wt::WLength& ImplicitConvert<Wt::WLength>::convert(PyObject* in, Wt::WLength** ptr) {
	*ptr = new Wt::WLength(Python2C<int>::arg(in));
	return **ptr;
}


#include <Wt/WString>
bool ImplicitConvert<Wt::WString>::accepts(PyObject* in) {
	return Python2C<const char*>::acceptarg(in) ||
	       Python2C<const wchar_t*>::acceptarg(in);
}
const Wt::WString& ImplicitConvert<Wt::WString>::convert(PyObject* in, Wt::WString** ptr) {
	if (Python2C<const char*>::acceptarg(in)) {
		*ptr = new Wt::WString(Python2C<const char*>::arg(in));
		return **ptr;
	}
	else if (Python2C<const wchar_t*>::acceptarg(in)) {
		*ptr = new Wt::WString(Python2C<const wchar_t*>::arg(in));
		return **ptr;
	}
	throw PyException();
}



