// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#include <Python.h>
#include "Typedefs.h"

using namespace Wt;


#include <Wt/WObject>



#include <Wt/WGoogleMap>
typedef std::pair<class Wt::WGoogleMap::Coordinate, class Wt::WGoogleMap::Coordinate> GoogleMapCoordinatePair;

#include "gwthttp_Conversion.h"
#include "gwt_Conversion.h"
#include "ComplexConversion.h"

#include "gwt_includeShell.h"
#include "PyCPPClassInstance.h"
#include "gwt_includeShellImpl.h"
