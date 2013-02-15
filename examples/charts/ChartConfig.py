#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#


from Wt import *
from PanelList import PanelList

markertype = [Chart.NoMarker, Chart.SquareMarker, Chart.CircleMarker, Chart.CrossMarker, Chart.XCrossMarker, Chart.TriangleMarker]


def q(cond, trueval, falseval):
	if cond: return trueval
	return falseval

def addHeader(t, value):
	t.elementAt(0, t.columnCount()).addWidget(WText(value));

def addEntry(model, value): 
	model.insertRows(model.rowCount(), 1)
	model.setData(model.rowCount()-1, 0, value)

def seriesIndexOf(chart, modelColumn):
	for i in range(len(chart.series())):
		if chart.series()[i].modelColumn() == modelColumn:
			return i;
	return -1;

def getDouble(edit):
	try:
		return float(edit.text().toUTF8())
	except:
		return None

def getInt(edit):
	try:
		return int(edit.text().toUTF8())
	except:
		return None

class SeriesControl:
	pass

class AxisControl:
	pass


class ChartConfig(WContainerWidget):
	def __init__(self, chart, parent):
		WContainerWidget.__init__(self, parent)
		self.chart = chart
		self.fill = Chart.MinimumValueFill
		self.seriesControls = []
		self.axisControls = []

		chart.initLayout()

		list = PanelList(self)


		sizeValidator = WIntValidator(200, 2000, self)
		sizeValidator.setMandatory(True);

		anyNumberValidator = WDoubleValidator(self);
		anyNumberValidator.setMandatory(True);

		angleValidator = WDoubleValidator(-90, 90, self);
		angleValidator.setMandatory(True);

		#------ Chart Properties -----

		orientation = WStandardItemModel(0, 1, self);
		addEntry(orientation, "Vertical")
		addEntry(orientation, "Horizontal")

		chartConfig = WTable();
		chartConfig.setMargin(WLength.Auto, Left | Right);

		row = 0;
		chartConfig.elementAt(row, 0).addWidget(WText(WString("Title:")));
		self.titleEdit = WLineEdit(chartConfig.elementAt(row, 1));
		self.connectSignals(self.titleEdit);
		++row;

		chartConfig.elementAt(row, 0).addWidget(WText(WString("Width:")));
		self.chartWidthEdit = WLineEdit(chartConfig.elementAt(row, 1));
		self.chartWidthEdit.setText(str(self.chart.width().value()));
		self.chartWidthEdit.setValidator(sizeValidator);
		self.chartWidthEdit.setMaxLength(4);
		self.connectSignals(self.chartWidthEdit);
		++row;

		chartConfig.elementAt(row, 0).addWidget(WText(WString("Height:")));
		self.chartHeightEdit = WLineEdit(chartConfig.elementAt(row, 1));
		self.chartHeightEdit.setText(str(self.chart.height().value()));
		self.chartHeightEdit.setValidator(sizeValidator);
		self.chartHeightEdit.setMaxLength(4);
		self.connectSignals(self.chartHeightEdit);
		++row;

		chartConfig.elementAt(row, 0).addWidget(WText(WString("Orientation:")));
		self.chartOrientationEdit = WComboBox(chartConfig.elementAt(row, 1));
		self.chartOrientationEdit.setModel(orientation);
		self.connectSignals(self.chartOrientationEdit);
		++row;

		for i in range(chartConfig.rowCount()):
			chartConfig.elementAt(i, 0).setStyleClass("tdhead");
			chartConfig.elementAt(i, 1).setStyleClass("tddata");

		p = list.addWidget("Chart properties", chartConfig);
		p.setMargin(WLength.Auto, Left | Right);
		p.resize(750, WLength.Auto);
		p.setMargin(20, Top | Bottom);

		if self.chart.isLegendEnabled():
			self.chart.setPlotAreaPadding(200, Right);

		# ---- Series properties ----

		types = WStandardItemModel(0, 1, self);
		addEntry(types, "Points")
		addEntry(types, "Line")
		addEntry(types, "Curve")
		addEntry(types, "Bar")
		addEntry(types, "Line Area")
		addEntry(types, "Curve Area")
		addEntry(types, "Stacked Bar")
		addEntry(types, "Stacked Line Area")
		addEntry(types, "Stacked Curve Area")

		markers = WStandardItemModel(0, 1, self)
		addEntry(markers, "None")
		addEntry(markers, "Square")
		addEntry(markers, "Circle")
		addEntry(markers, "Cross")
		addEntry(markers, "X cross")
		addEntry(markers, "Triangle")

		axes = WStandardItemModel(0, 1, self)
		addEntry(axes, "1st Y axis")
		addEntry(axes, "2nd Y axis")

		labels = WStandardItemModel(0, 1, self)
		addEntry(labels, "None")
		addEntry(labels, "X")
		addEntry(labels, "Y")
		addEntry(labels, "X: Y")

		seriesConfig = WTable()
		seriesConfig.setMargin(WLength.Auto, Left | Right)

		addHeader(seriesConfig, "Name")
		addHeader(seriesConfig, "Enabled")
		addHeader(seriesConfig, "Type")
		addHeader(seriesConfig, "Marker")
		addHeader(seriesConfig, "Y axis")
		addHeader(seriesConfig, "Legend")
		addHeader(seriesConfig, "Shadow")
		addHeader(seriesConfig, "Value labels")

		seriesConfig.rowAt(0).setStyleClass("trhead")


		for j in range(1, chart.model().columnCount()):
			sc = SeriesControl()

			WText(chart.model().headerData(j), seriesConfig.elementAt(j, 0));

			sc.enabledEdit = WCheckBox(seriesConfig.elementAt(j, 1));
			self.connectSignals(sc.enabledEdit);

			sc.typeEdit = WComboBox(seriesConfig.elementAt(j, 2));
			sc.typeEdit.setModel(types);
			self.connectSignals(sc.typeEdit);

			sc.markerEdit = WComboBox(seriesConfig.elementAt(j, 3));
			sc.markerEdit.setModel(markers);
			self.connectSignals(sc.markerEdit);

			sc.axisEdit = WComboBox(seriesConfig.elementAt(j, 4));
			sc.axisEdit.setModel(axes);
			self.connectSignals(sc.axisEdit);

			sc.legendEdit = WCheckBox(seriesConfig.elementAt(j, 5));
			self.connectSignals(sc.legendEdit);

			sc.shadowEdit = WCheckBox(seriesConfig.elementAt(j, 6));
			self.connectSignals(sc.shadowEdit);

			sc.labelsEdit = WComboBox(seriesConfig.elementAt(j, 7));
			sc.labelsEdit.setModel(labels);
			self.connectSignals(sc.labelsEdit);

			si = seriesIndexOf(chart, j);

			if (si != -1) :
				sc.enabledEdit.setChecked();
				s = self.chart.series(j);
				if s.type() == Chart.PointSeries:
					sc.typeEdit.setCurrentIndex(0);
				elif s.type() == Chart.LineSeries:
					sc.typeEdit.setCurrentIndex(q(s.fillRange() != Chart.NoFill ,
						(7 if s.isStacked() else 4) , 1))
				elif s.type() == Chart.CurveSeries:
					sc.typeEdit.setCurrentIndex(q(s.fillRange() != Chart.NoFill ,
						(8 if s.isStacked() else 5) , 2))
				elif s.type() == Chart.BarSeries:
					sc.typeEdit.setCurrentIndex(6 if s.isStacked() else 3);

				sc.markerEdit.setCurrentIndex((int(s.marker())))
				sc.legendEdit.setChecked(s.isLegendEnabled());
				sc.shadowEdit.setChecked(s.shadow() != WShadow());

			self.seriesControls.append(sc);

			seriesConfig.rowAt(j).setStyleClass("trdata")

		p = list.addWidget("Series properties", seriesConfig);
		p.expand();
		p.setMargin(WLength.Auto, Left | Right);
		p.resize(750, WLength.Auto);
		p.setMargin(20, Top | Bottom);

		# ---- Axis properties ----

		yScales = WStandardItemModel(0, 1, self);
		addEntry(yScales, "Linear scale");
		addEntry(yScales, "Log scale");

		xScales = WStandardItemModel(0, 1, self);
		addEntry(xScales, "Categories");
		addEntry(xScales, "Linear scale");
		addEntry(xScales, "Log scale");
		addEntry(xScales, "Date scale");

		axisConfig = WTable();
		axisConfig.setMargin(WLength.Auto, Left | Right);

		addHeader(axisConfig, "Axis");
		addHeader(axisConfig, "Visible");
		addHeader(axisConfig, "Scale");
		addHeader(axisConfig, "Automatic");
		addHeader(axisConfig, "Minimum");
		addHeader(axisConfig, "Maximum");
		addHeader(axisConfig, "Gridlines");
		addHeader(axisConfig, "Label angle");

		axisConfig.rowAt(0).setStyleClass("trhead");

		for i in range(3):
			axisName = [ "X axis", "1st Y axis", "2nd Y axis" ];
			axis_enum = [Chart.XAxis, Chart.Y1Axis, Chart.Y2Axis][i]
			j = i + 1;

			axis = self.chart.axis(axis_enum);
			sc = AxisControl()

			WText(WString(axisName[i], UTF8), axisConfig.elementAt(j, 0));

			sc.visibleEdit = WCheckBox(axisConfig.elementAt(j, 1));
			sc.visibleEdit.setChecked(axis.isVisible());
			self.connectSignals(sc.visibleEdit);

			sc.scaleEdit = WComboBox(axisConfig.elementAt(j, 2));
			if axis.scale() == Chart.CategoryScale:
				sc.scaleEdit.addItem("Category scale");
			else:
				if axis.id() == Chart.XAxis:
					sc.scaleEdit.setModel(xScales);
					sc.scaleEdit.setCurrentIndex(int(axis.scale()));
				else:
					sc.scaleEdit.setModel(yScales);
					sc.scaleEdit.setCurrentIndex(int(axis.scale()) - 1);
			self.connectSignals(sc.scaleEdit);

			autoValues = axis.autoLimits() == (Chart.MinimumValue | Chart.MaximumValue);
	
			sc.minimumEdit = WLineEdit(axisConfig.elementAt(j, 4));
			sc.minimumEdit.setText(str(axis.minimum()));
			sc.minimumEdit.setValidator(anyNumberValidator);
			sc.minimumEdit.setEnabled(not autoValues);
			self.connectSignals(sc.minimumEdit);
	
			sc.maximumEdit = WLineEdit(axisConfig.elementAt(j, 5));
			sc.maximumEdit.setText(str(axis.maximum()));
			sc.maximumEdit.setValidator(anyNumberValidator);
			sc.maximumEdit.setEnabled(not autoValues);
			self.connectSignals(sc.maximumEdit);
	
			sc.autoEdit = WCheckBox(axisConfig.elementAt(j, 3));
			sc.autoEdit.setChecked(autoValues);
			self.connectSignals(sc.autoEdit);
			sc.autoEdit.checked().connect(sc.maximumEdit.disable);
			sc.autoEdit.unChecked().connect(sc.maximumEdit.enable);
			sc.autoEdit.checked().connect(sc.minimumEdit.disable);
			sc.autoEdit.unChecked().connect(sc.minimumEdit.enable);
	
			sc.gridLinesEdit = WCheckBox(axisConfig.elementAt(j, 6));
			self.connectSignals(sc.gridLinesEdit);
	
			sc.labelAngleEdit = WLineEdit(axisConfig.elementAt(j, 7));
			sc.labelAngleEdit.setText("0");
			sc.labelAngleEdit.setValidator(angleValidator);
			self.connectSignals(sc.labelAngleEdit);
	
			axisConfig.rowAt(j).setStyleClass("trdata");
	
			self.axisControls.append(sc);
	
		p = list.addWidget("Axis properties", axisConfig);
		p.setMargin(WLength.Auto, Left | Right);
		p.resize(750, WLength.Auto);
		p.setMargin(20, Top | Bottom);


		# 
		#   If we do not have JavaScript, then add a button to reflect changes to
		#   the chart.
		#   
		if not WApplication.instance().environment().javaScript():
			b = WPushButton(self);
			b.setText("Update chart");
			b.setInline(False); # so we can add margin to center horizontally
			b.setMargin(WLength.Auto, Left | Right);
			b.clicked().connect(self.update);

	def setValueFill(self, fill):
		self.fill = fill


	def update(self):
		haveLegend = False;
		series = []

		for i in range(1, self.chart.model().columnCount()):
			sc = self.seriesControls[i-1]

			if sc.enabledEdit.isChecked():
      				s = Chart.WDataSeries(i)

				if sc.typeEdit.currentIndex() == 0:
					s.setType(Chart.PointSeries);
					if sc.markerEdit.currentIndex() == 0:
						sc.markerEdit.setCurrentIndex(1);
				elif sc.typeEdit.currentIndex() == 1:
					s.setType(Chart.LineSeries);
				elif sc.typeEdit.currentIndex() == 2:
					s.setType(Chart.CurveSeries);
				elif sc.typeEdit.currentIndex() == 3:
					s.setType(Chart.BarSeries);
				elif sc.typeEdit.currentIndex() == 4:
					s.setType(Chart.LineSeries);
					s.setFillRange(self.fill);
				elif sc.typeEdit.currentIndex() == 5:
					s.setType(Chart.CurveSeries);
					s.setFillRange(self.fill);
				elif sc.typeEdit.currentIndex() == 6:
					s.setType(Chart.BarSeries);
					s.setStacked(True);
				elif sc.typeEdit.currentIndex() == 7:
					s.setType(Chart.LineSeries);
					s.setFillRange(self.fill);
					s.setStacked(True);
				elif sc.typeEdit.currentIndex() == 8:
					s.setType(Chart.CurveSeries);
					s.setFillRange(self.fill);
					s.setStacked(True);	

		      		s.setMarker(markertype[sc.markerEdit.currentIndex()]);

				if (sc.axisEdit.currentIndex() == 1): 
					s.bindToAxis(Chart.Y2Axis);

				if sc.legendEdit.isChecked():
					s.setLegendEnabled(True);
					haveLegend = True;
				else:
					s.setLegendEnabled(False);

				if (sc.shadowEdit.isChecked()):
					s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
				else:
					s.setShadow(WShadow());

				if sc.labelsEdit.currentIndex() == 1:
					s.setLabelsEnabled(Chart.XAxis);
				if sc.labelsEdit.currentIndex() == 2:
					s.setLabelsEnabled(Chart.YAxis);
				if sc.labelsEdit.currentIndex() == 3:
					s.setLabelsEnabled(Chart.XAxis);	
					s.setLabelsEnabled(Chart.YAxis);

				series.append(s);

		self.chart.setSeries(series);

		for i in range(3):
			sc = self.axisControls[i]
			axis_enum = [Chart.XAxis, Chart.Y1Axis, Chart.Y2Axis][i]
			axis = self.chart.axis(axis_enum);

			axis.setVisible(sc.visibleEdit.isChecked())

			if sc.scaleEdit.count() != 1:
				k = sc.scaleEdit.currentIndex();
				if axis.id() != Chart.XAxis: k += 1
				else: 
					if k == 0: self.chart.setType(Chart.CategoryChart)
					else: self.chart.setType(Chart.ScatterPlot);

				if k == 1:
					axis.setScale(Chart.LinearScale)
				elif k == 2:
					axis.setScale(Chart.LogScale);
				elif k == 3:
					axis.setScale(Chart.DateScale)

			if sc.autoEdit.isChecked():
				axis.setAutoLimits(Chart.MinimumValue | Chart.MaximumValue);
			else:
				if self.validate(sc.minimumEdit) and self.validate(sc.maximumEdit):

					min = getDouble(sc.minimumEdit)
					max = getDouble(sc.maximumEdit);

					if axis.scale() == Chart.LogScale:
						if min <= 0:
							min = 0.0001;

					axis.setRange(min, max);

			if self.validate(sc.labelAngleEdit):
				angle = getDouble(sc.labelAngleEdit)
      				axis.setLabelAngle(angle);

			axis.setGridLinesEnabled(sc.gridLinesEdit.isChecked())

		self.chart.setTitle(self.titleEdit.text());

		if self.validate(self.chartWidthEdit) and self.validate(self.chartHeightEdit):
			width = getInt(self.chartWidthEdit)
			height = getInt(self.chartHeightEdit)
			self.chart.resize(width, height)

		if self.chartOrientationEdit.currentIndex() == 0: 
			self.chart.setOrientation(Vertical)
		elif self.chartOrientationEdit.currentIndex() == 1: 
			self.chart.setOrientation(Horizontal)

		self.chart.setLegendEnabled(haveLegend);
		self.chart.setPlotAreaPadding(200 if haveLegend else 40, Right);


	def connectSignals(self, w):
		w.changed().connect(self.update)
		if isinstance(w, WLineEdit):
			w.enterPressed().connect(self.update);

	def validate(self, w):
		valid = w.validate() == WValidator.Valid;

		if WApplication.instance().environment().javaScript():
    			w.setStyleClass("" if valid else "Wt-invalid")
			w.setToolTip("" if valid else "Invalid value")

		return valid
