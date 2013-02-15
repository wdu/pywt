// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#define MODPREFIX "Wt.Chart"

#include <Python.h>
#include <Wt/WObject>
#include "Enum.h"

#include "gwtchart_Conversion.h"

#include "gwtchart_includeTypes.h"
#include "includeTemplateTypes.h"


#include "TypeMacros.h"

PyObject* Chartinit() {
	PyObject *m;

	m = Py_InitModule("Wt.Chart", 0);
	if (m == NULL) return 0;



#include "gwtchart_TypeDecl.h"
#include "gwtchart_includeEnums.h"

	return m;
}



