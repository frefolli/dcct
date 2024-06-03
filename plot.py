"""
Plotting Unit
"""
import os
import typing
import numpy
import matplotlib.pyplot as plt
import matplotlib.axes as axes
import json

def load_report(path: str) -> dict:
  data = {}
  with open(path, mode="rb") as file:
    _data = json.load(file)
    for key in _data:
      data[key] = {}
      for cell in _data[key]:
        for label in cell:
          if label not in data[key]:
            data[key][label] = []
          data[key][label].append(cell[label])
  return data

def get_xy(report: dict, actuator: str) -> tuple|None:
  data = report.get(actuator)
  if data is not None:
    return numpy.array(data['N']), numpy.array(data['elapsed'])
  return None, None

def plot_by_mode(mode: str, *args, **kargs):
  match mode:
    case 'LinLog':
      plt.semilogy(*args, **kargs)
    case 'LinLin':
      plt.plot(*args, **kargs)
    case _:
      raise ValueError('unknown mode \'%s\'' % mode)

def time_addons(report: dict, mode: str = 'LinLog'):
  naive_Xs, naive_Ys = get_xy(report, 'dcct::FastActuator')
  if naive_Xs is not None:
    Xs = naive_Xs.astype(numpy.float64)
    Ys = numpy.power(Xs, 3)
    Ys = Ys / Ys[-1]
    Ys = Ys * naive_Ys[-1]
    plot_by_mode(mode, Xs, Ys, label='N^3')
  
  fftw_Xs, fftw_Ys = get_xy(report, 'dcct::FFTWActuator')
  if fftw_Xs is not None:
    Xs = fftw_Xs.astype(numpy.float64)
    Ys = numpy.power(Xs, 2) * numpy.log(Xs)
    Ys = Ys / Ys[-1]
    Ys = Ys * fftw_Ys[-1]
    plot_by_mode(mode, Xs, Ys, label='N^2 LognN')
  
  pocketfft_Xs, pocketfft_Ys = get_xy(report, 'dcct::PocketFFTActuator')
  if pocketfft_Xs is not None:
    Xs = pocketfft_Xs.astype(numpy.float64)
    Ys = numpy.power(Xs, 2) * numpy.log(Xs)
    Ys = Ys / Ys[-1]
    Ys = Ys * pocketfft_Ys[-1]
    plot_by_mode(mode, Xs, Ys, label='N^2 LognN')

def plot_report(report: dict, mode: str = 'LinLog'):
  path = 'benchmark-%s.png' % mode
  fig = plt.figure(figsize=(15, 15))
  for actuator in report:
    Xs, Ys = get_xy(report, actuator)
    plot_by_mode(mode, Xs, Ys, label=actuator, marker='o')
  time_addons(report, mode=mode)
  plt.xlabel('N')
  plt.ylabel('elapsed')
  plt.legend(fancybox=True, shadow=True)
  plt.title('Benchmark C++ (%s)' % mode)
  plt.savefig(path)
  plt.close(fig)

if __name__ == "__main__":
  report = load_report('report.json')
  plot_report(report, mode='LinLog')
  plot_report(report, mode='LinLin')
