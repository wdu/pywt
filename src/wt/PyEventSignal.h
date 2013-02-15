// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#ifndef PYEVENTSIGNAL
#define PYEVENTSIGNAL


#include "PyCPPClassType.h"
//#include "ConversionMacros.h"
#include <Wt/WSignal>
#include "SlotConnectorFactory.h"
#include "PyInstanceFactory.h"

template <class T = Wt::NoClass>
class PyEventSignal: public PyCPPClassType<Wt::EventSignal<T> >,
	public SlotConnectorFactory<Wt::EventSignal<T>, T>{
	NonPoly_PyInstanceFactory<Wt::EventSignal<T> > theFactory;
	MethodDef mdefs[2];
	public:
		PyEventSignal(const char* name): PyCPPClassType<Wt::EventSignal<T> >(name) { 
			PyCPPClassType<Wt::EventSignal<T> >::Type->Factory = &theFactory;
			mdefs[0] = MethodDef("connect", &PyEventSignal<T>::s_connect);
			PyEventSignal<T>::setMethods(mdefs);
		}


		static PyObject* s_connect(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);
};


//template <>           
//template <class T>
//PY2C_TEMPL_PTR(Wt::EventSignal<T>);

//template <>
//template <class T>
//C2PY_TEMPL_PTR(Wt::EventSignal<T>);

//template <>
//template <class T>
//C2PY_TEMPL_REF(Wt::EventSignal<T>);

#endif
