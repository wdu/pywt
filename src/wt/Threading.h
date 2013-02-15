// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef THREADING_H
#define THREADING_H


#define THREADMODE_SINGLETHREAD 1
#define THREADMODE_LOCKONNOTIFY 2
#define THREADMODE_GIL          3

//#define THREADMODE THREADMODE_SINGLETHREAD
//#define THREADMODE THREADMODE_LOCKONNOTIFY
#define THREADMODE THREADMODE_GIL

#include <Python.h>
#include <exception>
#include "PyException.h"

inline void convertException(std::exception& e) {
	PyErr_SetString(PyExc_RuntimeError, e.what());
	throw PyException();
}

inline void convertException() {
	PyErr_SetString(PyExc_RuntimeError, "Unknown Error");
	throw PyException();
}

#define CATCH_EXCEPTIONS_FROM_CPP_CALL \
	catch (PyException& ) { \
	} \
	catch (std::exception& e) { \
		convertException(e); \
	} \
	catch (...) { \
		convertException(); \
	} \

/*********************************************************/

#if THREADMODE == THREADMODE_SINGLETHREAD
inline void OnNotifyStart(PyThreadState*) {}
inline void OnNotifyEnd(PyThreadState*) {}
inline void OnCreateAppStart(PyThreadState*& ts) {ts = 0;}
inline void OnCreateAppEnd(PyThreadState*) {}
inline void OnDestructorStart(PyThreadState*) {}
inline void OnDestructorEnd(PyThreadState*) {}
inline void InitThreading() {}
inline void FinalizeThreading() {}

#define START_CPPCALL \
	try \

#define END_CPPCALL \
	CATCH_EXCEPTIONS_FROM_CPP_CALL

#endif

/*********************************************************/

#if THREADMODE == THREADMODE_LOCKONNOTIFY

extern PyThreadState* globalThreadState;

inline void OnNotifyStart(PyThreadState* ts) {
	PyEval_AcquireLock();
	PyThreadState_Swap(ts);
}
inline void OnNotifyEnd(PyThreadState* ts) {
	PyThreadState_Swap(globalThreadState);
	PyEval_ReleaseLock();
}
inline void OnCreateAppStart(PyThreadState*& ts) {
	PyEval_AcquireLock();
	PyThreadState* ts = PyThreadState_New(globalThreadState->interp);
	PyThreadState_Swap(ts);
	PyEval_ReleaseLock();
}
inline void OnCreateAppEnd(PyThreadState* ts) {
	PyThreadState_Swap(globalThreadState);
	PyEval_ReleaseLock();
}
inline void OnDestructorStart(PyThreadState* ts) {
	PyEval_AcquireLock();
	PyThreadState_Swap(ts);
}
inline void OnDestructorEnd(PyThreadState* ts) {
	PyThreadState_Swap(globalThreadState);
	PyThreadState_Delete(ts);
	PyEval_ReleaseLock();
}

inline void InitThreading() {
	PyEval_InitThreads();
	globalThreadState = PyThreadState_Get();
	PyEval_ReleaseLock();
}

inline void FinalizeThreading() {
	PyEval_AcquireLock();
	PyThreadState_Swap(globalThreadState);
}
#define START_CPPCALL \
	try \

#define END_CPPCALL \
	CATCH_EXCEPTIONS_FROM_CPP_CALL

#endif

/*********************************************************/

#if THREADMODE == THREADMODE_GIL

#define START_GIL PyGILState_STATE gil = PyGILState_Ensure()
#define STOP_GIL PyGILState_Release(gil)

#define OnNotifyStart(ts) START_GIL;
#define OnNotifyEnd(ts) STOP_GIL;
#define OnCreateAppStart(ts) START_GIL; ts = 0;
#define OnCreateAppEnd(ts) STOP_GIL;
#define OnDestructorStart(ts) START_GIL; 
#define OnDestructorEnd(ts) STOP_GIL;

extern PyThreadState* globalThreadState;

inline void InitThreading() {
	PyEval_InitThreads();
	globalThreadState = PyThreadState_Get();
	PyEval_ReleaseLock();
}

inline void FinalizeThreading() {
	PyEval_AcquireLock();
	PyThreadState_Swap(globalThreadState);
}


#define START_CPPCALL \
	try \

#define END_CPPCALL \
	CATCH_EXCEPTIONS_FROM_CPP_CALL

#endif



#endif
