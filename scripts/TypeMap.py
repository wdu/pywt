# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

import re

typemap = {
	"Wt::EventSignal0 &": "Wt::EventSignal<> &",
	"Wt::EventSignal0 *": "Wt::EventSignal<> *",
	"Wt::Signal0 &": "Wt::Signal<> &",
	"std::map<std::string, std::vector<std::string> > const &": "ArgumentMap const &",

	"std::pair<double, double> const &": "double_pair const &",
	"std::map<int, boost::any> const &": "int_any_map const &",

	"std::map<std::string, Wt::Http::UploadedFile> const &": 
		"string_uploadfile_map const &",
#	"std::pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> const &":
#		"coordinate_pair const &",
	"std::multimap<std::string, Wt::Http::UploadedFile> const &": "UploadedFileMap const &",
}

import string

def getTypename(name):
	name = string.strip(name)
	name = re.sub(r"\bclass ", "", name);
	if typemap.has_key(name): 
		print "  CONVERTING ", name, " TO ", typemap[name]
		return typemap[name]
	return name

