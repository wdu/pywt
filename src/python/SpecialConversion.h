// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef SPECIALCONVERSION_H
#define SPECIALCONVERSION_H

#include <Python.h>
#include <ostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "ConversionBase.h"
#include "CPPClassConversion.h"
#include "PyException.h"



template<> class Python2C< std::ostream* >: public Python2C_ptr< std::ostream > {};
template<> class Python2C< std::ostream& >: public Python2C_ref< std::ostream > {};
template<> class C2Python< std::ostream* >: public C2Python_ptr< std::ostream > {};
template<> class C2Python< std::ostream& >: public C2Python_ref< std::ostream > {};
template<> class C2Python< std::istream& >: public C2Python_ref< std::istream > {};

// PyObject
// it is used as an argument, so increasing ref cnt is not necessary
template <>
class Python2C<PyObject*> {
	public: 
		static PyObject* arg(PyObject* o) {
			return o;
		}
		static bool acceptarg(PyObject* o) {
			return true;
		}
		typedef PyObject* S;
};

// std::string
template <>
class Python2C<const std::string&> {
	public: 
		static std::string arg(PyObject* o, std::string**);
		static bool acceptarg(PyObject* o);
		typedef std::string S;
};

template <>
class Python2C<const std::string> {
	public: 
		static std::string ret(PyObject* o);
		static std::string arg(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef std::string S;
};


template <>
class Python2C<std::string> {
	public: 
		static std::string arg(PyObject* o);
		static std::string ret(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef std::string S;
};


template <>
class Python2C<const char*> {
	public: 
		static const char* arg(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef const char* S;
};

template <>
class C2Python<const char*> {
	public: 
		static PyObject* arg(const char *b);
};

template <>
class Python2C<const wchar_t*> {
	public: 
		static const wchar_t* arg(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef const wchar_t* S;
};


template <>
class Python2C<char> {
	public: 
		static char arg(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef char S;
};


// std::wstring
template <>
class Python2C<const std::wstring&> {
	public: 
		static const std::wstring& arg(PyObject* o, std::wstring**);
		static bool acceptarg(PyObject* o);
		typedef std::wstring S;
};


// bool
template <>
class Python2C<bool> {
	public: static bool arg(PyObject* o);
	public: static bool ret(PyObject* o);
			static bool acceptarg(PyObject* o);
		typedef bool S;
};

template <class T>
class C2Python_signed {
	public: 
		static PyObject* arg(const T b);
		static PyObject* ret(const T b);
		typedef T S;
};


template <class T>
class C2Python_unsigned {
	public: 
		static PyObject* arg(const T b);
		static PyObject* ret(const T b);
		typedef T S;
};


template <class T>
class Python2C_signed {
	public: 
		static T arg(PyObject* o);
		static const T& arg(PyObject* o, T**);
		static T ret(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef T S;
};

template <class T>
class Python2C_unsigned {
	public: 
		static T arg(PyObject* o);
		static T ret(PyObject* o);
		static bool acceptarg(PyObject* o);
		typedef T S;
};


#define DEF_INTCONV(T) \
template<> class C2Python<T>: public C2Python_signed<T> {}; \
template<> class C2Python<const T>: public C2Python_signed<T> {}; \
template<> class C2Python<const T&>: public C2Python_signed<T> {}; \
template<> class Python2C<T>: public Python2C_signed<T> {}; \
template<> class Python2C<const T>: public Python2C_signed<T> {}; \
template<> class Python2C<const T&>: public Python2C_signed<T> {}; \
template<> class C2Python<unsigned T>: public C2Python_unsigned<unsigned T> {}; \
template<> class C2Python<const unsigned T>: public C2Python_unsigned<unsigned T> {}; \
template<> class C2Python<const unsigned T&>: public C2Python_unsigned<unsigned T> {}; \
template<> class Python2C<unsigned T>: public Python2C_unsigned<unsigned T> {}; \
template<> class Python2C<const unsigned T>: public Python2C_unsigned<unsigned T> {}; \
template<> class Python2C<const unsigned T&>: public Python2C_unsigned<unsigned T> {}; \

DEF_INTCONV(short int);
DEF_INTCONV(int);
DEF_INTCONV(long int);
DEF_INTCONV(long long int);

template<> class Python2C<unsigned char>: public Python2C_unsigned<unsigned char> {};

// double
template <>
class Python2C<const double> {
	public: 
		static const double arg(PyObject* o);
		static const double& arg(PyObject* o, double**);
		static bool acceptarg(PyObject* o);
		typedef const double S;
};

template <> class Python2C<const double&>: public Python2C<const double> {};
template <> class Python2C<double>: public Python2C<const double> {};
template <> class Python2C<float>: public Python2C<const double> {};




// std::string
template <>
class C2Python<const std::string&> {
	public: static PyObject* ret(const std::string& o);
	public: static PyObject* arg(const std::string& o);
};

template <>
class C2Python<const std::string> {
	public: static PyObject* ret(const std::string& o);
};


template <>
class C2Python<std::string> {
	public: static PyObject* arg(const std::string& o);
	public: static PyObject* ret(const std::string& o);
};


template <>
class C2Python<std::wstring> {
	public: static PyObject* arg(const std::wstring& o);
	public: static PyObject* ret(const std::wstring& o);
};


// bool
template <>
class C2Python<bool> {
	public: static PyObject* ret(bool b);
	public: static PyObject* arg(bool b);
};

template <>
class C2Python<char> {
	public: static PyObject* ret(char b);
};


template <>
class C2Python<double> {
	public: static PyObject* arg(double b);
	public: static PyObject* ret(double b);
};

template <>
class C2Python<const double> {
	public: static PyObject* arg(const double b);
	public: static PyObject* ret(const double b);
};


// for non python arguments we create a new python object
// now we 'emulate' the same thing by incrementing the ref count
template <>
class C2Python<PyObject*> {
	public: static PyObject* arg(PyObject* b);
};

template <>
class Python2C<void *> {
	public:
		static void* arg(PyObject* b) {
			if (b == Py_None) return 0;
			return (void*)b;
		}
		typedef void* S;
};

template <>
class C2Python<void *> {
	public:
		static PyObject* ret(void* o) {
			if (o == 0) Py_RETURN_NONE;
			PyObject* r = static_cast<PyObject*>(o);
			Py_INCREF(r);
			return r;
		}
};


#include <locale>
template <>
class Python2C<const std::locale&> {
	public:
		static std::locale arg(PyObject* obj, std::locale** r); 
		static bool acceptarg(PyObject* obj);
		typedef std::locale S;
};


#endif
