#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#


from Wt import *
from ChartConfig import ChartConfig
import CsvUtil

def readCsvFile(fname, parent):
	model = WStandardItemModel(0, 0, parent);
    	try:
    		f = open(fname);
		CsvUtil.readFromCsv(f, model);
                for row in range(model.rowCount()):
                  for col in range(model.columnCount()):
                    model.item(row, col).setFlags(ItemIsSelectable | ItemIsEditable);
		return model
	except IOError, e:
      		error = WString.tr("error-missing-data");
      		error.arg(fname, UTF8);
      		WText(error, parent);
      		return None;

class TimeSeriesExample(WContainerWidget):
	def __init__(self, parent):
		WContainerWidget.__init__(self, parent)
  		WText(WString.tr("scatter plot"), self)


		model = readCsvFile("timeseries.csv", self);
		if not model: return

		for i in range(model.rowCount()):
			s = model.data(i, 0)
			d = WDate.fromString(WString(s), WString("dd/MM/yy"));
			model.setData(i, 0, d);

                # Show a view that allows editing of the model
                w = WContainerWidget(self);
                table = WTableView(w);
                table.setMargin(10, Top | Bottom);
                table.setMargin(WLength.Auto, Left | Right);

                table.setModel(model);
                table.setSortingEnabled(False);
                table.setColumnResizeEnabled(True);
                table.setSelectionMode(NoSelection);
                table.setAlternatingRowColors(True);
                table.setColumnAlignment(0, AlignCenter);
                table.setHeaderAlignment(0, AlignCenter);
                table.setRowHeight(22);

                if WApplication.instance().environment().ajax():
                  table.resize(800, 20 + 5 * 22);
                  table.setEditTriggers(WAbstractItemView.SingleClicked);
                else:
                  table.resize(800, 20 + 5 * 22 + 25);
                  table.setEditTriggers(WAbstractItemView.NoEditTriggers);

                delegate = WItemDelegate(self);
                delegate.setTextFormat("%.1f");
                table.setItemDelegate(delegate);
                table.setItemDelegateForColumn(0, WItemDelegate(self));

                table.setColumnWidth(0, 80);
                for i in range(1, model.columnCount()):
                  table.setColumnWidth(i, 90);

		#  
		#   Create the scatter plot.
		#   
		chart = Chart.WCartesianChart(self);
		chart.setModel(model);       # set the model
		chart.setXSeriesColumn(0)    # set the column that holds the X data
		chart.setLegendEnabled(True) # enable the legend

		chart.setType(Chart.ScatterPlot)            # set type to ScatterPlot
		chart.axis(Chart.XAxis).setScale(Chart.DateScale) # set scale of X axis to DateScale

		# Provide space for the X and Y axis and title. 
		chart.setPlotAreaPadding(100, Left);
		chart.setPlotAreaPadding(50, Top | Bottom);

		#  
		#  Add first two columns as line series
		#  
		for i in range(1, 3):
			s = Chart.WDataSeries(i, Chart.LineSeries)
			s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3))
			chart.addSeries(s);

  		chart.resize(WLength(800), WLength(400)) # WPaintedWidget must be given explicit size

  		chart.setMargin(WLength(10), Top | Bottom);           # add margin vertically
  		chart.setMargin(WLength.Auto, Left | Right); # center horizontally

		ChartConfig(chart, self)


class ScatterPlotExample(WContainerWidget):
	def __init__(self, parent):
		WContainerWidget.__init__(self, parent)
		WText(WString.tr("scatter plot 2"), self)

		model = WStandardItemModel(40, 2, self);
		model.setHeaderData(0, WString("X"))
		model.setHeaderData(1, WString("Y = sin(X)"))

		import math

		for i in range(40):
    			x = (float(i) - 20) / 4;

    			model.setData(i, 0, x);
			model.setData(i, 1, math.sin(x));
 
			#
			# Create the scatter plot.
			#

		self.chart = Chart.WCartesianChart(self)
		self.chart.setModel(model);        # set the model
		self.chart.setXSeriesColumn(0);    # set the column that holds the X data
		self.chart.setLegendEnabled(True); # enable the legend

		self.chart.setType(Chart.ScatterPlot);   # set type to ScatterPlot

		# Typically, for mathematical functions, you want the axes to cross
		# at the 0 mark:
		self.chart.axis(Chart.XAxis).setLocation(Chart.ZeroValue);
		self.chart.axis(Chart.YAxis).setLocation(Chart.ZeroValue);

		# Provide space for the X and Y axis and title. 
		self.chart.setPlotAreaPadding(100, Left);
		self.chart.setPlotAreaPadding(50, Top | Bottom);

		# Add the curves
		s = Chart.WDataSeries(1, Chart.CurveSeries);
		s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3))
		self.chart.addSeries(s);

		self.chart.resize(800, 300); # WPaintedWidget must be given explicit size

		self.chart.setMargin(10, Top | Bottom);            # add margin vertically
		self.chart.setMargin(WLength.Auto, Left | Right); # center horizontally

		config = ChartConfig(self.chart, self);
		config.setValueFill(Chart.ZeroValueFill);

class PieExample(WContainerWidget):
	def __init__(self, parent):
		WContainerWidget.__init__(self, parent)
		WText(WString.tr("pie chart"), self)

		model = readCsvFile("pie.csv", self);

		if not model: return

                for row in range(model.rowCount()):
                  for col in range(model.columnCount()):
                    model.item(row, col).setFlags(ItemIsSelectable | ItemIsEditable)

                w = WContainerWidget(self);
                table = WTableView(w);
                table.setMargin(10, Top | Bottom);
                table.setMargin(WLength.Auto, Left | Right);
                table.setSortingEnabled(True);
                table.setModel(model);
                table.setColumnWidth(1, 100);
                table.setRowHeight(22);

                if WApplication.instance().environment().ajax():
                  table.resize(150 + 100 + 14, 20 + model.rowCount() * 22);
                  table.setEditTriggers(WAbstractItemView.SingleClicked);
                else:
                  table.resize(150 + 100 + 14, WLenght.Auto);
                  table.setEditTriggers(WAbstractItemView.NoEditTrigger);

		#
		# Create the pie chart.
		#/
		chart = Chart.WPieChart(self);
		chart.setModel(model);       # set the model
		chart.setLabelsColumn(0);    # set the column that holds the labels
		chart.setDataColumn(1);      # set the column that holds the data

		# configure location and type of labels
		chart.setDisplayLabels(Chart.Outside | Chart.TextLabel | Chart.TextPercentage);

		# enable a 3D effect
		chart.setPerspectiveEnabled(True, 0.2);

		# explode the first item
		chart.setExplode(0, 0.3);

		chart.resize(800, 300); # WPaintedWidget must be given explicit size

		chart.setMargin(10, Top | Bottom);            # add margin vertically
		chart.setMargin(WLength.Auto, Left | Right); # center horizontally


class CategoryExample(WContainerWidget):
	def __init__(self, parent):
		WContainerWidget.__init__(self, parent)
		WText(WString.tr("category chart"), self)

		model = readCsvFile("category.csv", self);

		if not model: return

                w = WContainerWidget(self);
                table = WTableView(w);

                table.setMargin(10, Top | Bottom);
                table.setMargin(WLength.Auto, Left | Right);

                table.setModel(model);
                table.setSortingEnabled(True);
                table.setColumnResizeEnabled(True);
                table.setSelectionMode(NoSelection);
                table.setAlternatingRowColors(True);
                table.setColumnAlignment(0, AlignCenter);
                table.setHeaderAlignment(0, AlignCenter);
                table.setRowHeight(22);

                if WApplication.instance().environment().ajax():
                  table.resize(600, 20 + 5 * 22);
                  table.setEditTriggers(WAbstractItemView.SingleClicked);
                else:
                  table.resize(600, WLength.Auto);
                  table.setEditTriggers(WAbstractItemView.NoEditTrigger);

                delegate = WItemDelegate(self);
                delegate.setTextFormat("%.f");
                table.setItemDelegate(delegate);

                table.setColumnWidth(0, 80);
                for i in range (1, model.columnCount() - 1):
                  table.setColumnWidth(i, 120);

		#
		# Create the category chart.
		#
  		chart = Chart.WCartesianChart(self);
  		chart.setModel(model);        # set the model
  		chart.setXSeriesColumn(0);    # set the column that holds the categories
  		chart.setLegendEnabled(True); # enable the legend

  		# Provide space for the X and Y axis and title. 
  		chart.setPlotAreaPadding(100, Left);
  		chart.setPlotAreaPadding(50, Top | Bottom);


		#
		# Add all (but first) column as bar series
		#
		for i in range(1, model.columnCount()):
			s = Chart.WDataSeries(i, Chart.BarSeries)
    			s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
    			chart.addSeries(s);

		chart.resize(800, 400); # WPaintedWidget must be given explicit size

		chart.setMargin(10, Top | Bottom);            # add margin vertically
		chart.setMargin(WLength.Auto, Left | Right); # center horizontally

		ChartConfig(chart, self);

class ChartsExample(WContainerWidget):
	def __init__(self, root):
		WContainerWidget.__init__(self, root)

		WText(WString.tr("introduction"), self)

  		CategoryExample(self);
  		TimeSeriesExample(self)
		ScatterPlotExample(self)
		PieExample(self)

