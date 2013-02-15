// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <vector>
#include <map>
#include <string>
#include <boost/any.hpp>
#include <Wt/WGoogleMap>

typedef std::vector<std::string> ArgumentValues;
typedef std::map<std::string, ArgumentValues> ArgumentMap;
typedef std::pair<double, double> double_pair;
typedef std::map<int, class boost::any> int_any_map;

typedef std::pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> coordinate_pair;



namespace Wt {
	namespace Http {
		class UploadedFile;
	}
}
typedef std::map<std::string, class Wt::Http::UploadedFile> string_uploadfile_map;
typedef std::multimap<std::string, class Wt::Http::UploadedFile> UploadedFileMap;




#endif

