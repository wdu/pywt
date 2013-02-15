// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#ifndef CPPCLASSCONVERSION_H
#define CPPCLASSCONVERSION_H

#include <Python.h>
#include <Wt/WFlags>
#include "Enum.h"

template <class T> 
class Python2C_ptr { 
	public: static T* arg(PyObject* e); 
			static T* ret(PyObject* e); 
			static bool acceptarg(PyObject* e); 
			typedef T* S;
}; 

template <class T> 
class Python2C_cptr { 
	public: 
		static const T* arg(PyObject* e);
		static const T* ret(PyObject* e);
		static bool acceptarg(PyObject* e); 
		typedef const T* S;
}; 




template<class T>
class Python2C_cref {
	public: 
		static const T& arg(PyObject* e, T** ptr); 
		static bool acceptarg(PyObject* e); 
		typedef const T& S;
};



template<class T>
class Python2C_ref {
	public: 
		static T& arg(PyObject* e); 
		static T& ret(PyObject* e);  // potentially dangerous
		static bool acceptarg(PyObject* e); 
		typedef T& S;
};



template<class T>
class Python2C_val { 
	public: 
		static T arg(PyObject* e); 
		static T ret(PyObject* e); 
		static bool acceptarg(PyObject* e); 
		typedef T S;
}; 


template<class T>
class Python2C_cval { 
	public: 
		static const T arg(PyObject* e); 
		static bool acceptarg(PyObject* e); 
		typedef const T S;
}; 



/*******************************************************/

template<class T>
class C2Python_ptr { 
	public:  
		static PyObject* ret(const T* e); 
		static PyObject* arg(const T* e); 
}; 


template<class T>
class C2Python_cptr { 
	public:  
		static PyObject* ret(const T* e); 
		static PyObject* arg(const T* e); 
}; 

template<class T>
class C2Python_val { 
	public: static PyObject* arg(const T& e); 
			static PyObject* ret(const T& e); 
}; 


template<class T>
class C2Python_cval { 
	public: static PyObject* arg(const T& e); 
			static PyObject* ret(const T& e); 
}; 


template<class T>
class C2Python_ref { 
	public:
		static PyObject* arg(T& e); 
		static PyObject* ret(T& e); 
}; 



template<class T>
class C2Python_cref { 
	public:
		static PyObject* arg(const T& e); 
		static PyObject* ret(const T& e); 
}; 





/*******************************************************/
template<class T>
class Python2C_enum {  
	public: 
		static T arg(PyObject* e); 
		static T ret(PyObject* e); 
		static bool acceptarg(PyObject* e); 
		typedef T S;
}; 

template<class T>
class Python2C_flags {  
	public: 
		static Wt::WFlags<T> arg(PyObject* e); 
		static Wt::WFlags<T> arg(PyObject* e, Wt::WFlags<T>** f); 
		static Wt::WFlags<T> ret(PyObject* e); 
		static bool acceptarg(PyObject* e); 
		typedef T S;
}; 

template<class T>
class C2Python_enum {  
	public: 
		static PyObject* ret(const T e); 
		static PyObject* arg(const T e); 
}; 


template<class T>
class C2Python_flags {  
	public: 
		static PyObject* ret(const Wt::WFlags<T> e); 
		static PyObject* arg(const Wt::WFlags<T> e); 
}; 


/*******************************************************/
template<class T>
class Python2C_opaque_ref {  
	public: 
		static T arg(PyObject* e); 
		static T ret(PyObject* e); 
		static bool acceptarg(PyObject* e); 
		typedef T S;
}; 

template<class T>
class C2Python_opaque_ref {  
	public: 
		static PyObject* ret(T e); 
		static PyObject* arg(T e); 
}; 



#endif
