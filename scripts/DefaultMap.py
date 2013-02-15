# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

defaultmap = {
	'(RepeatXY)' : ' (Wt::WCssDecorationStyle::RepeatXY)',
	'(Pixel)' : ' (Wt::WLength::Pixel)',
	'(LazyLoading)' : ' (Wt::WMenuItem::LazyLoading)',
	'(SupportsAjax)' : ' (Wt::Http::Client::SupportsAjax)',
	'(OrdinateAxis)' : ' (Wt::Chart::OrdinateAxis)',
	'(Editing)' : ' (Wt::WSuggestionPopup::Editing)',
	'(Y1Axis)' : ' (Wt::Chart::Y1Axis)',
	'(PointSeries)' : ' (Wt::Chart::PointSeries)',
	'(struct std::locale ())': '(std::locale ())',
	'(std::map<class std::string, std::vector<class std::string> > ())': '(ArgumentMap())',
	'(std::map<class std::string, class Wt::Http::UploadedFile> ())': '(string_uploadfile_map())',
 	'(std::multimap<class std::string, class Wt::Http::UploadedFile> ())': '(UploadedFileMap())',
}

import string

def getDefault(name):
	name = string.strip(name)
	if defaultmap.has_key(name):
		print "  CONVERTING ", name, " TO ", defaultmap[name]
		return defaultmap[name]
	return name



