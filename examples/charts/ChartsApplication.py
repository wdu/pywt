#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#


from Wt import *
from ChartsExample import *
import sys

class ChartsApplication(WApplication):
	def __init__(self, env):
		WApplication.__init__(self, env)
  		self.setTitle(WString("Charts example"));
                self.setCssTheme("polished");
  		self.messageResourceBundle().use("charts");

  		self.root().setPadding(WLength(10));
  		self.root().resize(WLength.Auto, WLength.Auto);

  		ChartsExample(self.root());

  		self.useStyleSheet(WLink("charts.css"));


def createApplication(env):
	return ChartsApplication(env)


if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)


