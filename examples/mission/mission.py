# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

import sys
import Wt


class CountDownWidget(Wt.WText):
	def __init__(self, start, stop, msec, parent):
		Wt.WText.__init__(self, "",  Wt.XHTMLText,parent)
		self.start = start
		self.stop = min(start-1, stop)
		self.current = start
		self.timer = Wt.WTimer(self)
		self.timer.setInterval(msec)
		self.timer.timeout().connect(self.timerTick)
		self.timer.start()
		self.setText(str(self.current))
		# FIXME: can we use Wt's signal/slots in python?
		#self.done = Wt.Signal0();


	def cancel(self): 
		self.timer.stop()

	def timerTick(self):
		self.current = self.current-1
		self.setText(str(self.current))
		
		if self.current <= self.stop:
			self.timer.stop()
			Wt.WApplication.instance().quit()
			#self.done.emit()


def createApplication(env):
	appl = Wt.WApplication(env)

	Wt.WText("<h1>Your mission</h1>",  Wt.XHTMLText, appl.root())
	secret = Wt.WText("Your mission, Jim, should you accept, is to create solid " +
				"web applications.",  Wt.XHTMLText, appl.root())

	Wt.WText("This program will quit in ",  Wt.XHTMLText, appl.root())
	countdown = CountDownWidget(10, 0, 1000, appl.root());
	Wt.WText(" seconds.",  Wt.XHTMLText, appl.root())

	Wt.WBreak(appl.root())
	Wt.WBreak(appl.root())

	cancelButton = Wt.WPushButton("Cancel!", appl.root())
	quitButton = Wt.WPushButton("Quit", appl.root())
	quitButton.clicked().connect(appl.quit)

	#countdown.done.connect(appl.quit);
	cancelButton.clicked().connect(countdown.cancel);
	cancelButton.clicked().connect(cancelButton.disable);
	cancelButton.clicked().connect(secret.hide);

	return appl


if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)

