#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#


import sys
import Wt

def createDragImage(url, smallurl, mimeType, p):
	result = Wt.WImage(Wt.WLink(url), "", p);
	dragImage = Wt.WImage(Wt.WLink(smallurl), "", p);

	result.setDraggable(mimeType, dragImage, True, None);

  	return result;


class Character(Wt.WText):
	def __init__(self, name, parent):
		Wt.WText.__init__(self, "",  Wt.XHTMLText, parent)
		self.name = name
		self.redDrops = 0
		self.blueDrops = 0

		self.setText(name + " got no pills")

		self.setStyleClass("character");
		self.acceptDrops("red-pill", "red-drop-site");
		self.acceptDrops("blue-pill", "blue-drop-site");
		self.setInline(False);

	def dropEvent(self, event):
		if event.mimeType() == "red-pill":
			self.redDrops = self.redDrops +1
		if event.mimeType() == "blue-pill":
			self.blueDrops = self.blueDrops +1

		text = self.name + " got ";

		if self.redDrops != 0:
			text = text + str(self.redDrops) + " red pill"
			if self.redDrops > 1: text = text + "s"

		if self.redDrops != 0 and self.blueDrops != 0:
			text = text + " and "

		if self.blueDrops != 0:
			text = text + str(self.blueDrops) + " blue pill"
			if self.blueDrops > 1: text = text + "s"
		self.setText(text);



	
def createApp(env):
	app = Wt.WApplication(env);
	app.setTitle("Drag &amp; drop");

	Wt.WPushButton("Quit", app.root()).clicked().connect(app.quit)


  	Wt.WText("<h1>Wt Drag &amp; drop example.</h1>",  Wt.XHTMLText, app.root());
  	Wt.WText("<p>Help these people with their decision by dragging one of the pills.</p>",  Wt.XHTMLText, app.root());


	if not env.javaScript():
   		Wt.WText("<i>This examples requires that javascript support is enabled.</i>",  Wt.XHTMLText, app.root());


	pills = Wt.WContainerWidget(app.root());
	pills.setContentAlignment(Wt.AlignCenter);

	createDragImage("icons/blue-pill.jpg",
					"icons/blue-pill-small.png",
					"blue-pill", pills);
	createDragImage("icons/red-pill.jpg",
					"icons/red-pill-small.png",
					"red-pill", pills);

	dropSites = Wt.WContainerWidget(app.root())

	Character("Neo", dropSites);
	Character("Morpheus", dropSites);
	Character("Trinity", dropSites);

	app.useStyleSheet(Wt.WLink("dragdrop.css"));

	return app



if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApp)

