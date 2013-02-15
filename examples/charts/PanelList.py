#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#



from Wt import *

class PanelList(WContainerWidget):
	def __init__(self, parent):
		WContainerWidget.__init__(self, parent)
		self.wasExpanded = -1

	def addWidget(self, text, w):
		p = WPanel()
		p.setTitle(text)
		p.setCentralWidget(w)
		self.addPanel(p)
		return p

	def addPanel(self, panel):
		panel.setCollapsible(True)
		panel.collapse()
		panel.expanded().connect(self.onExpand)
		WContainerWidget.addWidget(self, panel)

	def onExpand(self):
		panel = self.sender()

		self.wasExpanded = -1
		cnt = 0
		for p in self.children():
			if p != panel:
				if not p.isCollapsed(): wasExpanded = cnt
				p.collapse()
			cnt = cnt+1




