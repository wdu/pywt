// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#define MODPREFIX "Wt.Http"

#include <Python.h>
#include <Wt/WObject>
#include "Enum.h"

#include "gwthttp_Conversion.h"

#include "gwthttp_includeTypes.h"
#include "includeTemplateTypes.h"
//#include "gwthttp_EnumConversions.h"


#include "TypeMacros.h"

PyObject* Httpinit() {
	PyObject *m;

	m = Py_InitModule("Wt.Http", 0);
	if (m == NULL) return 0;



#include "gwthttp_TypeDecl.h"
#include "gwthttp_includeEnums.h"

	return m;
}



