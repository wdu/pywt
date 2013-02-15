// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
// This is an automatically generated file. Do not edit.

#include "PyException.h"

/************************* 0 Arguments ******************/
template <class R>
inline R callPyMethod0(PyObject* instance, const char* name) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* args = PyTuple_New(0);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


inline void callPyMethodV0(PyObject* instance, const char* name) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* args = PyTuple_New(0);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 1 Arguments ******************/
template <class R, class A1>
inline R callPyMethod1(PyObject* instance, const char* name, A1 a1) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, a1p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1>
inline void callPyMethodV1(PyObject* instance, const char* name, A1 a1) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, a1p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 2 Arguments ******************/
template <class R, class A1, class A2>
inline R callPyMethod2(PyObject* instance, const char* name, A1 a1, A2 a2) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2>
inline void callPyMethodV2(PyObject* instance, const char* name, A1 a1, A2 a2) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 3 Arguments ******************/
template <class R, class A1, class A2, class A3>
inline R callPyMethod3(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* args = PyTuple_New(3);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3>
inline void callPyMethodV3(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* args = PyTuple_New(3);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 4 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4>
inline R callPyMethod4(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* args = PyTuple_New(4);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4>
inline void callPyMethodV4(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* args = PyTuple_New(4);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 5 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4, class A5>
inline R callPyMethod5(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* args = PyTuple_New(5);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4, class A5>
inline void callPyMethodV5(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* args = PyTuple_New(5);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 6 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline R callPyMethod6(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* args = PyTuple_New(6);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4, class A5, class A6>
inline void callPyMethodV6(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* args = PyTuple_New(6);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 7 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline R callPyMethod7(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* args = PyTuple_New(7);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline void callPyMethodV7(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* args = PyTuple_New(7);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 8 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline R callPyMethod8(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* a8p = C2Python<A8>::arg(a8);
	PyObject* args = PyTuple_New(8);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyTuple_SetItem(args, 7, a8p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline void callPyMethodV8(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* a8p = C2Python<A8>::arg(a8);
	PyObject* args = PyTuple_New(8);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyTuple_SetItem(args, 7, a8p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


/************************* 9 Arguments ******************/
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline R callPyMethod9(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	if (PyErr_Occurred()) throw PyException();
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* a8p = C2Python<A8>::arg(a8);
	PyObject* a9p = C2Python<A9>::arg(a9);
	PyObject* args = PyTuple_New(9);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyTuple_SetItem(args, 7, a8p);
	PyTuple_SetItem(args, 8, a9p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	R r = Python2C<R>::ret(pyr);
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
	return r;
}


template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline void callPyMethodV9(PyObject* instance, const char* name, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
	PyObject* f = PyObject_GetAttrString(instance, name);
	PyObject* a1p = C2Python<A1>::arg(a1);
	PyObject* a2p = C2Python<A2>::arg(a2);
	PyObject* a3p = C2Python<A3>::arg(a3);
	PyObject* a4p = C2Python<A4>::arg(a4);
	PyObject* a5p = C2Python<A5>::arg(a5);
	PyObject* a6p = C2Python<A6>::arg(a6);
	PyObject* a7p = C2Python<A7>::arg(a7);
	PyObject* a8p = C2Python<A8>::arg(a8);
	PyObject* a9p = C2Python<A9>::arg(a9);
	PyObject* args = PyTuple_New(9);
	PyTuple_SetItem(args, 0, a1p);
	PyTuple_SetItem(args, 1, a2p);
	PyTuple_SetItem(args, 2, a3p);
	PyTuple_SetItem(args, 3, a4p);
	PyTuple_SetItem(args, 4, a5p);
	PyTuple_SetItem(args, 5, a6p);
	PyTuple_SetItem(args, 6, a7p);
	PyTuple_SetItem(args, 7, a8p);
	PyTuple_SetItem(args, 8, a9p);
	PyObject* pyr = PyObject_Call(f, args, 0);
	if (!pyr) {throw PyException();}
	Py_XDECREF(pyr);
	Py_XDECREF(f);
	Py_XDECREF(args);
}


