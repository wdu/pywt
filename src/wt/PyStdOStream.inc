// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <ostream>
template <>
PyCPPClassType< std::ostream >* PyCPPClassType< std::ostream >::Type = 0;


PyStdOStream::PyStdOStream(): PyCPPClassType< std::ostream >(MODPREFIX "StdOStream") {
			Type->Factory = &theFactory;
			// Inherited methods
};

