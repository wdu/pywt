// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef PYSTDOSTREAM
#define PYSTDOSTREAM


#include <ostream>
#include "PyCPPClassType.h"
#include "PyInstanceFactory.h"


class PyStdOStream: public PyCPPClassType< std::ostream > {
	public:
		NonPoly_PyInstanceFactory< std::ostream > theFactory;
		PyStdOStream();
};








#endif


