// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#define MODPREFIX "Wt.Chart"

#include <Python.h>
#include "Typedefs.h"


#include <Wt/WObject>


#include "gwt_Conversion.h"
#include "gwtchart_Conversion.h"
#include "ComplexConversion.h"

#include "ChartProxy.h"
#include "gwtchart_includeTypes.h"
#include "Compare.h"

using namespace Wt;
using namespace Wt::Chart;
#include "gwtchart_includeWrapperImpl.h"
