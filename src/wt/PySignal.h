// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#ifndef PYSIGNAL
#define PYSIGNAL


#include "PyCPPClassType.h"
//#include "ConversionMacros.h"
#include <Wt/WSignal>
#include "SlotConnectorFactory.h"

template <class A1=Wt::NoClass, class A2=Wt::NoClass, class A3=Wt::NoClass, class A4=Wt::NoClass, class A5=Wt::NoClass, class A6=Wt::NoClass>
class PySignal: public PyCPPClassType<Wt::Signal<A1, A2, A3, A4, A5, A6> >,
	public SlotConnectorFactory<Wt::Signal<A1, A2, A3, A4, A5, A6>, A1, A2, A3, A4, A5, A6>{
	MethodDef mdefs[3];
	public:
		PySignal(const char* name): PyCPPClassType<Wt::Signal<A1, A2, A3, A4, A5, A6> >(name) { 
			 PyCPPClassType<Wt::Signal<A1, A2, A3, A4, A5, A6> >::Type->Factory = new NonPoly_PyInstanceFactory< Wt::Signal<A1, A2, A3, A4, A5, A6> >;

			mdefs[0] = MethodDef("connect", &PySignal<A1, A2, A3, A4, A5, A6>::s_connect);
			mdefs[1] = MethodDef("emit", &PySignal<A1, A2, A3, A4, A5, A6>::s_emit);
			PySignal<A1, A2, A3, A4, A5, A6>::setMethods(mdefs);
			PyCPPClassTypeBase::setConstructorPtr(&PySignal<A1, A2, A3, A4, A5, A6>::instanceConstructor);
			PyCPPClassTypeBase::makeInstanciatable();
		}



		static void instanceConstructor(PyCPPClassInstance* inst, PyObject* args, PyObject* kwargs, int);
		static PyObject* s_connect(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);
		static PyObject* s_emit(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);

};


#endif
