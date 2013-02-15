// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include "SpecialConversion.h"
#include "CPPClassConversion.inc"
#include "Enum.h"

template <class T>
PyObject* C2Python_signed<T>::arg(const T b) {
	if (b >= 0 && b <= PyInt_GetMax()) {
		return PyInt_FromLong((long)b);
	}
	// b < 0, so b+1 is certainly in the range of T
	else if ((b+1) >= -PyInt_GetMax()) {
		return PyInt_FromLong((long)b);
	}
	else {
		return PyLong_FromLongLong((long long)b);
	}
}

template <class T>
PyObject* C2Python_signed<T>::ret(const T b) {
	return arg(b);
}



template <class T>
PyObject* C2Python_unsigned<T>::arg(const T b) {
	if (static_cast<long long>(b) <= static_cast<long long>(PyInt_GetMax())) {
		return PyInt_FromLong(long(b));
	}
	else {
		return PyLong_FromUnsignedLongLong((long long)b);
	}
}

template <class T>
PyObject* C2Python_unsigned<T>::ret(const T b) {
	return arg(b);
}

template <class T>
T Python2C_signed<T>::arg(PyObject* o) {
	if (PyInt_Check(o)) {
		// succeeds always, as long is the internal rep
		long v = PyInt_AS_LONG(o);
		T v2 = (T)v;
                if (v == std::numeric_limits<long>::max())
                	v2 = std::numeric_limits<T>::max();
                else if (v != v2) {
			PyErr_Format(PyExc_ValueError, "Value %ld is out of range.", v);
			throw PyException();
		}
		return v2;
	}
	else if (PyLong_Check(o)) {
		// may fail because out of range
		long long v = PyLong_AsLongLong(o);
		if (PyErr_Occurred()) throw PyException(); 
		T v2 = (T)v;
                if (v == std::numeric_limits<long long>::max())
                	v2 = std::numeric_limits<T>::max();
                else if (v != v2) {
			PyErr_Format(PyExc_ValueError, "Value %lld is out of range.", v);
			throw PyException();
		}
		return v2;
	}
	else if (Enum_Check(o)) {
		Enum* e = (Enum*)o;
		return e->value;
	}
	else {
		typeError(o, "integer");
		return T(); // silence compile warning
	}
}

template <class T>
const T& Python2C_signed<T>::arg(PyObject* o, T** r) {
	*r = new T();
	**r = Python2C_signed<T>::arg(o);
	return **r;
}

template <class T>
T Python2C_signed<T>::ret(PyObject* o) {
	return arg(o);
}

template <class T>
bool Python2C_signed<T>::acceptarg(PyObject* o) {
	return PyInt_Check(o) || PyLong_Check(o) || Enum_Check(o);
}
						 


template <class T>
T Python2C_unsigned<T>::arg(PyObject* o) {
	if (PyInt_Check(o)) {
		// succeeds always, as long is the internal rep
		long v = PyInt_AS_LONG(o);
		T v2 = (T)v;
		if (v != v2) {
			PyErr_Format(PyExc_ValueError, "Value %ld is out of range.", v);
			throw PyException();
		}
		return v2;
	}
	else if (PyLong_Check(o)) {
		// may fail because out of range
		unsigned long long v = PyLong_AsUnsignedLongLong(o);
		if (PyErr_Occurred()) throw PyException(); 
		T v2 = (T)v;
		if (v != v2) {
			PyErr_Format(PyExc_ValueError, "Value %lld is out of range.", v);
			throw PyException();
		}
		return v2;
	}
	else if (Enum_Check(o)) {
		Enum* e = (Enum*)o;
		return e->value;
	}
	else {
		typeError(o, "integer");
		return T(); // unreachable - silence compiler
	}
}

template <class T>
T Python2C_unsigned<T>::ret(PyObject* o) {
	return arg(o);
}

template <class T>
bool Python2C_unsigned<T>::acceptarg(PyObject* o) {
	return PyInt_Check(o) || PyLong_Check(o) || Enum_Check(o);
}
						 

#define DEF_INTCONV_INST(T) \
template class C2Python_signed< T >; \
template class C2Python_unsigned< unsigned T >; \
template class Python2C_signed< T >; \
template class Python2C_unsigned< unsigned T >; \

DEF_INTCONV_INST(short int);
DEF_INTCONV_INST(int);
DEF_INTCONV_INST(long int);
DEF_INTCONV_INST(long long int);

template class Python2C_unsigned<unsigned char>;





const std::wstring& Python2C<const std::wstring&>::arg(PyObject* o, std::wstring** r) {
	if (!PyUnicode_Check(o)) {
		typeError(o, "unicode string");
	}
	Py_ssize_t len = PyUnicode_GET_SIZE(o);
	wchar_t* chars = new wchar_t[len+1];
	PyUnicode_AsWideChar((PyUnicodeObject*)o, chars, len+1);
	chars[len] = 0;
	*r = new std::wstring(chars);
	delete[] chars;
	return **r;
}

bool Python2C<const std::wstring&>::acceptarg(PyObject* o) {
	return PyUnicode_Check(o);
}


PyObject* C2Python<const std::string&>::ret(const std::string& o) {
	return PyString_FromString(o.c_str());
}
PyObject* C2Python<const std::string&>::arg(const std::string& o) {
	return PyString_FromString(o.c_str());
}

PyObject* C2Python<const std::string>::ret(const std::string& o) {
	return PyString_FromString(o.c_str());
}

// a bit dangerous - we assume we don't need to do any memory mgmt
PyObject* C2Python<const char *>::arg(const char *o) {
	return PyString_FromString(o);
}


PyObject* C2Python<std::string>::arg(const std::string& o) {
	return PyString_FromString(o.c_str());
}
PyObject* C2Python<std::string>::ret(const std::string& o) {
	return PyString_FromString(o.c_str());
}


PyObject* C2Python<std::wstring>::arg(const std::wstring& o) {
	return PyUnicode_FromWideChar(o.c_str(), o.size());
}
PyObject* C2Python<std::wstring>::ret(const std::wstring& o) {
	return PyUnicode_FromWideChar(o.c_str(), o.size());
}


// bool
PyObject* C2Python<bool>::ret(bool b) {
	if (b)  Py_RETURN_TRUE;
	Py_RETURN_FALSE;
}
PyObject* C2Python<bool>::arg(bool b) { 
	return ret(b); 
} 

PyObject* C2Python<double>::arg(double b) { return PyFloat_FromDouble(b);}
PyObject* C2Python<double>::ret(double b) { return PyFloat_FromDouble(b);}
PyObject* C2Python<const double>::arg(const double b) { return PyFloat_FromDouble(b);}
PyObject* C2Python<const double>::ret(const double b) { return PyFloat_FromDouble(b);}


PyObject* C2Python<char>::ret(char b) { 
	char v[2];
	v[0] = b;
	v[1] = 0;
	return PyString_FromString(v);
}


#include "PyException.h"
// for non python arguments we create a new python object
// now we 'emulate' the same thing by incrementing the ref count
PyObject* C2Python<PyObject*>::arg(PyObject* b) {Py_INCREF(b); return b;}



// std::string
std::string Python2C<const std::string&>::arg(PyObject* o, std::string**) {
	if (!PyString_Check(o)) {
		typeError(o, "string");
	}
	return PyString_AsString(o);
}
bool Python2C<const std::string&>::acceptarg(PyObject* o) {
	return PyString_Check(o);
}

std::string Python2C<const std::string>::ret(PyObject* o) {
	if (!PyString_Check(o)) {
		typeError(o, "string");
	}
	return PyString_AsString(o);
}
std::string Python2C<const std::string>::arg(PyObject* o) {return ret(o);}
bool Python2C<const std::string>::acceptarg(PyObject* o) {
	return PyString_Check(o);
}


std::string Python2C<std::string>::arg(PyObject* o) {
	if (!PyString_Check(o)) {
		typeError(o, "string");
	}
	return PyString_AsString(o);
}
std::string Python2C<std::string>::ret(PyObject* o) {return arg(o);}
bool Python2C<std::string>::acceptarg(PyObject* o) {
	return PyString_Check(o);
}


const char* Python2C<const char*>::arg(PyObject* o) {
	if (!PyString_Check(o)) {
		typeError(o, "string");
	}
	return PyString_AsString(o);
}

bool Python2C<const char*>::acceptarg(PyObject* o) {
	return PyString_Check(o);
}


const wchar_t* Python2C<const wchar_t*>::arg(PyObject* o) {
	if (!PyUnicode_Check(o)) {
		typeError(o, "unicode string");
	}
	return (const wchar_t*)PyUnicode_AsUnicode(o);
}

bool Python2C<const wchar_t*>::acceptarg(PyObject* o) {
	return PyUnicode_Check(o);
}


char Python2C<char>::arg(PyObject* o) {
	if (!PyString_Check(o)) {
		typeError(o, "string");
	}
	if (PyString_GET_SIZE(o) != 1) {
		PyErr_SetString(PyExc_TypeError, "Expected a string of length 1");
		throw PyException();
	}
	return PyString_AsString(o)[0];
}
bool Python2C<char>::acceptarg(PyObject* o) {
	return PyString_Check(o);
}

// bool
bool Python2C<bool>::arg(PyObject* o) {
	return 0 != PyObject_IsTrue(o);
}
bool Python2C<bool>::ret(PyObject* o) {return arg(o);}
bool Python2C<bool>::acceptarg(PyObject* o) {
	return true;
}

// double

bool Python2C<const double>::acceptarg(PyObject* o) {
	return 0 != PyNumber_Check(o);
}

const double Python2C<const double>::arg(PyObject* o) {
	if (!PyNumber_Check(o)) {
		typeError(o, "double");
	}
	PyObject* floatobj = PyNumber_Float(o);
	if (floatobj) {
		double r = PyFloat_AS_DOUBLE(floatobj);
		Py_DECREF(floatobj);
		return r;
	}
	else {
		throw PyException();
	}
}

const double& Python2C<const double>::arg(PyObject* o, double** r) {
	*r = new double;
	**r = arg(o);
	return **r;

}



#include <ostream>
template class Python2C_ptr< std::ostream >;
template class Python2C_ref< std::ostream >;
template class C2Python_ptr< std::ostream >;
template class C2Python_ref< std::ostream >;
#include <istream>
template class C2Python_ref< std::istream >;

std::locale Python2C<const std::locale&>::arg(PyObject* obj, std::locale** r) {
	if (!PyString_Check(obj)) {
		typeError(obj, "locale (string)");
	}
	try {
		*r = new std::locale(PyString_AsString(obj));
		return **r;
	}
	catch (const std::runtime_error& e) {
		PyErr_SetString(PyExc_RuntimeError, e.what());
		throw PyException();
	}
}

bool Python2C<const std::locale&>::acceptarg(PyObject* obj) {
	return PyString_Check(obj) != 0;
}





