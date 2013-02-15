// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef TYPEMACROS_H
#define TYPEMACROS_H


#define TYPEVAR(t) t ## Type
#define PYTYPE(t) Py ## t
#define QUOTE(t) #t

#define PYTYPE_T T ## Type

#define ADD_TYPE(module, coretype, name) \
static PYTYPE(name) TYPEVAR(name); \
if (PyType_Ready((PyTypeObject*)&TYPEVAR(name)) < 0) return 0; \
Py_INCREF(&TYPEVAR(name)); \
PyModule_AddObject((module), #name, (PyObject *)&TYPEVAR(name));


#define ADD_TEMPL_TYPE(module, pytype, pyname) \
	static pytype TYPEVAR(pyname)(QUOTE(pyname)); \
if (PyType_Ready((PyTypeObject*)&TYPEVAR(pyname)) < 0) return 0; \
Py_INCREF(&TYPEVAR(pyname)); \
PyModule_AddObject((module), #pyname, (PyObject *)&TYPEVAR(pyname));

#define ADD_TEMPL_TYPE_T \
	static  T VAR ( NAME ); \
if (PyType_Ready((PyTypeObject*)&VAR) < 0) return 0; \
Py_INCREF(&VAR); \
PyModule_AddObject(M, NAME, (PyObject *)&VAR);

#endif


