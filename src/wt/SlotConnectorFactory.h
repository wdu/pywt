// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#ifndef SLOTCONNECTORFACTORY
#define SLOTCONNECTORFACTORY

#include "SlotConnector.h"


template <class SIGNAL,
		 class A1=Wt::NoClass, class A2=Wt::NoClass,
		 class A3=Wt::NoClass, class A4=Wt::NoClass,
		 class A5=Wt::NoClass, class A6=Wt::NoClass>
class SlotConnectorFactory {
	public:
		static SlotConnectorBase* createConnector(SIGNAL* s, PyObject* slot) {
			return new SlotConnector<SIGNAL, A1, A2, A3, A4, A5, A6>(s, slot);
		}
};


#endif

