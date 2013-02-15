// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#ifndef METHODWRAPPER
#define METHODWRAPPER

#include "ConversionBase.h"


//#include "g_runmethod.inc"



#define CATCHALL() \
	catch (PyException&) {                    \
	}                                           \
	catch (std::exception& e) {                 \
		PyErr_SetString(PyExc_RuntimeError, e.what()); \
	}                                           \
	catch (std::exception* e) {                 \
		PyErr_SetString(PyExc_RuntimeError, e->what()); \
	}                                           \
	catch (...) {                               \
		PyErr_SetString(PyExc_RuntimeError, "Unknown Exception"); \
	}                                           \

	

#include "g_callPyMethod.inc"


#endif
