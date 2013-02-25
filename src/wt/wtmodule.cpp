// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include <Python.h>
#include "PythonApplication.h"
#include "Threading.h"

// Avoid to have to deal with these in cmake
#include "../generated/wt/http/gwthttp_Strings.cpp"
#include "../generated/wt/chart/gwtchart_Strings.cpp"
#include "../generated/wt/gwt_Strings.cpp"
#include "../generated/wt/gwt_Compare.cpp"

PyObject* init();
PyObject* Httpinit();
PyObject* Chartinit();

PyMODINIT_FUNC initWt(void) {
	PyObject* m = init();

	PyObject_SetAttrString(m, "Http", Httpinit());
	PyObject_SetAttrString(m, "Chart", Chartinit());

	PyObject_SetAttrString(m, "__hook__", PyCObject_FromVoidPtr(&PythonApplication::hook, 0));

	PythonApplication::hook.createApplication = &PythonApplication::createApplication;
	PythonApplication::hook.setAppCreatorFunc = &PythonApplication::setAppCreatorFunc;
	PythonApplication::hook.initThreading = InitThreading;
	PythonApplication::hook.finalizeThreading = FinalizeThreading;

}




