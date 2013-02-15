# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

import sys
import Wt

Confirm = 0
Alert   = 1
Prompt  = 2


class Popup(Wt.WObject):
	def __init__(self, t, message, defaultValue, parent):
		Wt.WObject.__init__(self, parent)
		self.okPressed = Wt.JSignal[int](self, "ok");
		self.cancelPressed = Wt.JSignal[None](self, "cancel");
		self.t = t
		self.message = message
		self.defaultValue = defaultValue
		self.show = Wt.JSlot();
		self.setJavaScript();

	def setJavaScript(self):
		if self.t == Confirm:
			self.show.setJavaScript(
				"function(){ if (confirm('" + self.message + "')) {"
				"  Wt.emit('" + self.id() + "','" + self.okPressed.name() + "', 0);"
				"} else {"
				"  Wt.emit('" + self.id() + "','" + self.cancelPressed.name() + "');"
				"}}")
		elif self.t == Alert:
			self.show.setJavaScript(
				"function(){ alert('" + self.message + "');"
				"Wt.emit('" + self.id() + "','" + self.okPressed.name() + "', '');}")
		elif self.t == Prompt:
			self.show.setJavaScript("""
				function(){var n = prompt('%s', '%d');
					if (n != null) {
						Wt.emit('%s','%s', n);
					} else {
						Wt.emit('%s','%s'); 
					}
				} """ % (self.message, self.defaultValue,
			       self.id(), self.okPressed.name(),
				   self.id(), self.cancelPressed.name()))


	def setDefaultValue(self, defaultValue):
		self.defaultValue = defaultValue
		self.setJavaScript()

	def setMessage(self, message):
		self.message = message
		self.setJavaScript()

	def createPrompt(message, defaultValue, parent):
		return Popup(Prompt, message, defaultValue, parent)
	createPrompt = staticmethod(createPrompt)

	def createConfirm(message, parent):
		return Popup(Confirm, message, 0, parent);
	createConfirm = staticmethod(createConfirm)

	def createAlert(message, parent):
		return Popup(Alert, message, 0, parent);
	createAlert = staticmethod(createAlert)



class JavaScriptApplication(Wt.WApplication):
	def __init__(self, env):
		Wt.WApplication.__init__(self, env)

		self.setTitle("Javascript example")
		self.promptAmount = Popup.createPrompt("How much do you want to pay?", 0, self)
		self.promptAmount.okPressed.connect(self.setAmount)

		self.confirmPay = Popup.createConfirm("", self)
		self.confirmPay.okPressed.connect(self.confirmed)

		Wt.WText("<h2>Wt Javascript example</h2>"
	    "<p>Wt makes abstraction of Javascript, and therefore allows you"
	    " to develop web applications without any knowledge of Javascript,"
	    " and which are not dependent on Javascript."
	    " However, Wt does allow you to add custom Javascript code:</p>"
	    " <ul>"
	    "   <li>To call custom JavaScript code from an event handler, "
	    "connect the Wt::EventSignal to a Wt::JSlot.</li>"
	    "   <li>To call C++ code from custom JavaScript, use "
	    "Wt.emit() to emit a Wt::JSignal.</li>"
	    "   <li>To call custom JavaScript code from C++, use "
	    "WApplication::doJavascript() or Wt::JSlot::exec().</li>"
	    " </ul>"
	    "<p>This simple application shows how to interact between C++ and"
	    " JavaScript using the JSlot and JSignal classes.</p>", self.root())

		self.currentAmount = Wt.WText("Current amount: $%d" % self.promptAmount.defaultValue, self.root());

		amountButton = Wt.WPushButton("Change ...", self.root());
		amountButton.setMargin(Wt.WLength(10), Wt.Left | Wt.Right)

		Wt.WBreak(self.root());

		confirmButton = Wt.WPushButton("Pay now.", self.root());
		confirmButton.setMargin(Wt.WLength(10), Wt.Top | Wt.Bottom);

		amountButton.clicked().connect(self.promptAmount.show);
		confirmButton.clicked().connect(self.confirmPay.show);

		self.setAmount(1000);
					 
	def setAmount(self, amount):
		self.confirmPay.setMessage("Are you sure you want to pay $%d ?" % amount);
		self.promptAmount.setDefaultValue(amount)
		self.currentAmount.setText("Current amount: $%d" % self.promptAmount.defaultValue);

	def confirmed(self):
		Wt.WText("<br/>Just payed $%d." % self.promptAmount.defaultValue, self.root())



def createApplication(env):
	return JavaScriptApplication(env)




if __name__ == "__main__":
	import sys
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)


