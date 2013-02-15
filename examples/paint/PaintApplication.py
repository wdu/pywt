# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

import sys
import Wt


emweb = Wt.WColor(0x04, 0x7c, 0x93)


class ShapesWidget(Wt.WPaintedWidget):
	def __init__(self, parent = None):
		Wt.WPaintedWidget.__init__(self, parent)
		self.angle = 0
		self.size = 1
		self.resize(Wt.WLength(710), Wt.WLength(400))

	def setAngle(self, angle):
		angle = max(-30.0, min(30.0, angle))

		if self.angle != angle:
			self.angle = angle
			self.update();

	def setRelativeSize(self, size):
		size = max(0.1, min(1.0, size))

		if self.size != size:
			self.size = size
			self.update()

	def paintEvent(self, paintDevice):
		painter = Wt.WPainter(paintDevice)

		painter.setRenderHint(Wt.WPainter.Antialiasing)

		painter.translate(self.width().value()/2, self.height().value()/2);
		painter.rotate(self.angle);
		painter.scale(self.size, self.size);
		painter.translate(-self.width().value()/2 + 50, -self.height().value()/2 + 150);

		self.drawEmwebLogo(painter);

	def drawEmwebE(self, painter):
		p = Wt.WPainterPath()

		# Path copied from our SVG for half of the E 
		p.moveTo(46.835084,58.783624);
		p.cubicTo(45.700172,58.783624,35.350098,58.911502,24.656354,63.283309);
		p.cubicTo(8.7595992,69.78907,0,82.38499,0,98.809238);
		p.cubicTo(0,115.20152,08.7595992,127.82141,24.656354,134.31119);
		p.cubicTo(35.350098,138.69099,45.700172,138.81088,46.835084,138.81088);
		p.lineTo(94.509362,138.81088);
		p.lineTo(94.509362,117.58323);
		p.lineTo(46.835084,117.58323);
		p.cubicTo(46.811106,117.58323,39.466151,117.47134,32.608727,114.53815);
		p.cubicTo(25.095932,111.34122,21.747144,106.47389,21.747144,98.809238);
		p.cubicTo(21.747144,91.120612,25.095932,86.269265,32.608727,83.064338);
		p.cubicTo(39.466151,80.123159,46.811106,80.027251,46.89103,80.027251);
		p.lineTo(94.509362,80.027251);
		p.lineTo(94.509362,58.783624);
		p.lineTo(46.835084,58.783624);

		painter.drawPath(p);

		painter.save();
		painter.translate(0,-58.783624);
		painter.drawPath(p);
		painter.restore();

	
	def drawEmwebMW(self, painter):
		p = Wt.WPainterPath()

		#Path copied from our SVG for one fourth of the MW 

		p.moveTo(120.59634,24.072913);
		p.cubicTo(116.12064,34.518895,115.98477,44.605222,115.98477,45.732141);
		p.lineTo(115.98477,138.81088);
		p.lineTo(137.7399,138.81088);
		p.lineTo(137.7399,45.732141);
		p.cubicTo(137.7399,45.708164,137.83581,38.53904,140.84892,31.841463);
		p.cubicTo(144.14176,24.512492,149.113,21.235634,156.98545,21.235634);
		p.cubicTo(164.8499,21.235634,169.81314,24.512492,173.10599,31.841463);
		p.cubicTo(176.10311,38.53904,176.215,45.708164,176.215,45.780095);
		p.lineTo(176.215,80.41343);
		p.lineTo(197.97014,80.41343);
		p.lineTo(197.97014,45.732141);
		p.cubicTo(197.97014,44.605222,197.83427,34.518895,193.35057,24.072913);
		p.cubicTo(186.70894,8.5517985,173.77734,0,156.99344,0);
		p.cubicTo(140.17756,0,127.25396,8.5517985,120.59634,24.072913);

		#Paint it four times, translated and inverted

		painter.drawPath(p);

		dx = 176. - 115.98477;

		painter.save();

		painter.translate(dx, 0);
		painter.drawPath(p);

		painter.translate(dx, 0);

		painter.scale(-1, -1);
		painter.translate(0, -138.81088);
		painter.translate(-115.98477 - 197.95 - dx, 0);
		painter.drawPath(p);

		painter.translate(dx, 0);
		painter.drawPath(p);

		painter.restore();


	def drawEmwebLogo(self, painter):
		painter.save()
		painter.setPen(Wt.WPen(Wt.NoPen))

		# The emweb logo can be drawn as 3 e's, and one combined m/w

		# Emweb
		painter.setBrush(Wt.WBrush(Wt.black));
		self.drawEmwebE(painter);

		# emwEb
		painter.save();
		painter.translate(397, 0);
		self.drawEmwebE(painter);

		# emweB
		painter.translate(210, 0);
		painter.scale(-1, 1);
		self.drawEmwebE(painter);

		painter.restore();

		# eMWeb
		painter.setBrush(Wt.WBrush(emweb));
		self.drawEmwebMW(painter);

		painter.restore();

class PaintExample(Wt.WContainerWidget):
	def __init__(self, root):
		Wt.WContainerWidget.__init__(self, root)
		Wt.WText("<h2>Paint example</h2>"
	    "<p>A simple example demonstrating cross-browser vector graphics."
	    "</p>"
	    "<p>The emweb logo below is painted using the Wt WPainter API, and "
	    "rendered to the browser using inline SVG, inline VML or the "
	    "HTML 5 &lt;canvas&gt; element."
	    "</p>",
	    self)

		emweb = Wt.WContainerWidget(self)
		emweb.setMargin(Wt.WLength.Auto, Wt.Left | Wt.Right)

		layout = Wt.WGridLayout()
		emweb.setLayout(layout, Wt.AlignCenter | Wt.AlignTop)

		scaleSlider = Wt.WSlider(Wt.Horizontal)
		scaleSlider.setMinimum(0)
		scaleSlider.setMaximum(20)
		scaleSlider.setValue(10)
		scaleSlider.setTickInterval(5)
		scaleSlider.setTickPosition(Wt.WSlider.TicksBothSides)
		scaleSlider.resize(300, 50)
		scaleSlider.valueChanged().connect(self.scaleShape)

		layout.addWidget(scaleSlider, 0, 1, Wt.AlignCenter | Wt.AlignMiddle)

		rotateSlider = Wt.WSlider(Wt.Vertical)
		rotateSlider.setMinimum(-30)
		rotateSlider.setMaximum(30)
		rotateSlider.setValue(0)
		rotateSlider.setTickInterval(10)
		rotateSlider.setTickPosition(Wt.WSlider.TicksBothSides)
		rotateSlider.resize(50, 400)
		rotateSlider.valueChanged().connect(self.rotateShape)

		layout.addWidget(rotateSlider, 1, 0, Wt.AlignCenter | Wt.AlignMiddle)

		self.shapes = ShapesWidget();
		self.shapes.setAngle(0.0);
		self.shapes.setRelativeSize(0.5);
		self.shapes.setPreferredMethod(Wt.WPaintedWidget.InlineSvgVml);

		layout.addWidget(self.shapes, 1, 1, Wt.AlignCenter | Wt.AlignMiddle);

	def rotateShape(self, v):
		self.shapes.setAngle(v / 2.0);
		prefmethod = Wt.WPaintedWidget.HtmlCanvas
		if v < 0: prefmethod =  Wt.WPaintedWidget.InlineSvgVml
		self.shapes.setPreferredMethod(prefmethod)

	def scaleShape(self, v):
		self.shapes.setRelativeSize(0.1 + 0.9 * (v/20.0));



class PaintApplication(Wt.WApplication):
	def __init__(self, env):
		Wt.WApplication.__init__(self, env)
		self.setTitle("Paint example");
		self.useStyleSheet(Wt.WLink("painting.css"));
		PaintExample(self.root());

def createApplication(env):
	return PaintApplication(env)


if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)


