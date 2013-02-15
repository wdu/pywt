// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#ifndef PYJSIGNAL
#define PYJSIGNAL


#include "PyCPPClassType.h"
//#include "ConversionMacros.h"
#include "JSignal_shell.h"
#include "SlotConnectorFactory.h"

inline void sigToString(PyTypeObject** sig, char* buf) {
	if (sig == 0) {
		buf[0] = 0;
		return;
	}
	char* ptr = buf;
	for (int i = 0; i<7; ++i) {
		if (sig[i] == 0) continue;
		char* name = (char*)(sig[i]->tp_name);
		while (*name != 0) {
			*ptr = *name;
			ptr++;
			name++;
		}
		if (i < 6 && sig[i+1] != 0) {
			*ptr = ','; ptr++;
			*ptr = ' '; ptr++;
		}
	}
	*ptr = 0;
}


template <class A1=Wt::NoClass, class A2=Wt::NoClass, class A3=Wt::NoClass, class A4=Wt::NoClass, class A5=Wt::NoClass, class A6=Wt::NoClass>
class PyJSignal: public PyCPPClassType<Wt::JSignal<A1, A2, A3, A4, A5, A6> >,
	public SlotConnectorFactory<Wt::JSignal<A1, A2, A3, A4, A5, A6>, A1, A2, A3, A4, A5, A6>{
	public:
		PyTypeObject* signature[7];
	MethodDef mdefs[4];
	public:
		PyJSignal(const char* name, PyTypeObject** sig=0): PyCPPClassType<Wt::JSignal<A1, A2, A3, A4, A5, A6> >(name) { 
			char buf[200];
			sigToString(sig, buf);

			mdefs[0] = MethodDef("connect", &PyJSignal<A1, A2, A3, A4, A5, A6>::s_connect);
			mdefs[1] = MethodDef("emit", &PyJSignal<A1, A2, A3, A4, A5, A6>::s_emit);
			mdefs[2] = MethodDef("name", &PyJSignal<A1, A2, A3, A4, A5, A6>::s_name);
			PyJSignal<A1, A2, A3, A4, A5, A6>::setMethods(mdefs);
			PyCPPClassTypeBase::setConstructorPtr(&PyJSignal<A1, A2, A3, A4, A5, A6>::instanceConstructor);
			PyCPPClassTypeBase::makeInstanciatable();
			if (sig == 0) {
				for (int i = 0; i <7; ++i)  signature[i] = 0; 
			}
			else {
				for (int i = 0; i <7; ++i)  signature[i] = sig[i]; 
			}
			PyCPPClassType<Wt::JSignal<A1, A2, A3, A4, A5, A6> >::tp_repr = repr;
		}

		static PyObject* repr(PyObject* self) {
			char buffer[256];
			char buffer2[256];
			sigToString(((PyJSignal<A1, A2, A3, A4, A5, A6>*)(self->ob_type))->signature, buffer2);
			PyOS_snprintf(buffer, sizeof(buffer), "<JSignal[%s] at %p>", buffer2, self);
			return PyString_FromString(buffer);
		}



		static void instanceConstructor(PyCPPClassInstance* inst, PyObject* args, PyObject* kwargs, int flags);
		static PyObject* s_connect(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);
		static PyObject* s_emit(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);
		static PyObject* s_name(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);

};


#endif
