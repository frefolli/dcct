"""
Plotting Unit
"""
import matplotlib.pyplot
import pesto.generics
import pesto.logs
import logging
import typing
import numpy
import os

class BasicPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.data: dict
    self.label: str
    self.x: str
    self.y: str
    self.scale: str
    Xs = self.data[self.x]
    Ys = self.data[self.y]
    self.label = "%s (min: %s, max: %s)" % (self.label, round(min(Ys), 3), round(max(Ys), 3))
    match self.scale:
      case 'linlin':
        matplotlib.pyplot.plot(Xs, Ys, marker='o', label=self.label)
      case 'linlog':
        matplotlib.pyplot.semilogy(Xs, Ys, marker='o', label=self.label)
      case 'loglin':
        matplotlib.pyplot.semilogx(Xs, Ys, marker='o', label=self.label)
      case 'loglog':
        matplotlib.pyplot.loglog(Xs, Ys, marker='o', label=self.label)

class ConstantYPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.Xs: list
    self.y: float
    self.scale: str
    self.label = str(self.y)
    Ys = numpy.array([self.y for _ in self.Xs])
    match self.scale:
      case 'linlin':
        matplotlib.pyplot.plot(self.Xs, Ys, marker='o', label=self.label)
      case 'linlog':
        matplotlib.pyplot.semilogy(self.Xs, Ys, marker='o', label=self.label)
      case 'loglin':
        matplotlib.pyplot.semilogx(self.Xs, Ys, marker='o', label=self.label)
      case 'loglog':
        matplotlib.pyplot.loglog(self.Xs, Ys, marker='o', label=self.label)

class TheoricTrendPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    """"""
    self.data: dict
    self.target: str
    self.x: str
    self.y: str
    self.label: str
    self.func: typing.Any
    self.scale: str

    input_Xs, input_Ys = self.data[self.target][self.x], self.data[self.target][self.y]
    Xs = numpy.array(input_Xs, dtype=numpy.float64)
    Ys = self.func(Xs)
    Ys = Ys / Ys[-1]
    Ys = Ys * input_Ys[-1]
    BasicPlotter.run(data={self.x: Xs, self.y: Ys}, label=self.label, x=self.x, y=self.y, scale=self.scale)

class AggregatePlotter(pesto.generics.ServiceObject[None]):
  """
  Simple Aggregate plots. If ranges are passed, it cuts plots to display only Ys = [min(ranges), max(ranges)]. (The ranges do need to be sorted.)
  """
  def exec(self) -> None:
    self.data: dict
    self.title: str
    self.x: str
    self.y: str
    self.scale: str
    self.ranges: list|None
    self.trends: list|None
    self.outdir: str

    filepath = os.path.join(self.outdir, '%s.png' % self.title)
    logging.info("Generating %s" % filepath)
    fig = matplotlib.pyplot.figure(figsize=(15, 15))
    matplotlib.pyplot.title(self.title)
    for label in self.data:
      if self.ranges is not None:
        indexes = numpy.where(numpy.logical_and(self.data[label][self.y] >= self.ranges[0], self.data[label][self.y] <= self.ranges[-1]))
        self.data[label][self.y] = self.data[label][self.y][indexes]
        self.data[label][self.x] = self.data[label][self.x][indexes]
      BasicPlotter.run(data=self.data[label], label=label, x=self.x, y=self.y, scale=self.scale)

    if self.ranges is not None:
      for y in self.ranges[1:-1]:
        Xs = matplotlib.pyplot.xlim()
        ConstantYPlotter.run(Xs=Xs, y=y, scale=self.scale)

    if self.trends is not None:
      for trend in self.trends:
        TheoricTrendPlotter.run(data=self.data,
                                x=self.x,
                                y=self.y,
                                target=trend['target'],
                                label=trend['label'],
                                func=trend['func'],
                                scale=self.scale)

    matplotlib.pyplot.xlabel(self.x)
    matplotlib.pyplot.ylabel(self.y)
    matplotlib.pyplot.legend()
    matplotlib.pyplot.savefig(filepath)
    matplotlib.pyplot.close(fig)
    logging.info("Done with %s" % filepath)

class FunctionalPlotter(pesto.generics.ServiceObject[None]):
  """
  Functionally crafts and plots data. If ranges are passed, it cuts plots to display only Ys = [min(ranges), max(ranges)]. (The ranges don't need to be sorted.)
  """
  def exec(self) -> None:
    self.input: typing.Any
    self.crafter: pesto.generics.ServiceObject[dict]|None
    self.x: str
    self.y: str
    self.scale: str
    self.ranges: list|None
    self.trends: list|None
    self.outdir: str

    try:
      self.ranges = sorted(self.ranges)
    except Exception:
      self.ranges = None

    try:
      self.trends = self.trends
    except Exception:
      self.trends = None

    os.makedirs(self.outdir, exist_ok=True)
    data: dict
    
    if self.crafter is not None:
      data = self.crafter.run(input=self.input)
    else:
      data = self.input

    for group in data:
      AggregatePlotter.run(data=data[group], title=group,
                           x=self.x, y=self.y,
                           scale=self.scale, ranges=self.ranges,
                           trends=self.trends, outdir=self.outdir)
