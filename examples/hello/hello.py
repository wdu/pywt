#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

import sys
import Wt
import WtHttp

class MyApplication(Wt.WApplication):
	def __init__(self, env):
		Wt.WApplication.__init__(self, env)
		self.setTitle("Hello")

		Wt.WText("Your name please? ", Wt.XHTMLText, self.root())

		self.nameedit = Wt.WLineEdit("", self.root())
		self.nameedit.setFocus()

		button = Wt.WPushButton("Greet me!", self.root())
		button2 = Wt.WPushButton("Quit", self.root())

		Wt.WBreak(self.root())

		self.greeting = Wt.WText("", Wt.XHTMLText, self.root())

		button.clicked().connect(self.greet)

		self.nameedit.enterPressed().connect(self.greet)

		button2.clicked().connect(self.quit)


	def greet(self):
		self.greeting.setText("Hello there, " + self.nameedit.text())


def createApplication(env):
	return MyApplication(env)


if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)



