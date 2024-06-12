import typing
import sys
import argparse
import logging
import numpy
from pesto.loaders import JsonLoader, CsvLoader
from pesto.plotters import FunctionalPlotter
from pesto.crafters import BenchmarkReportCrafter

def SlowTrend(Xs):
  return numpy.power(Xs, 3)

def FastTrend(Xs):
  return numpy.power(Xs, 2) * numpy.log(Xs)

def do_benchmark(config: argparse.Namespace) -> None:
  data = JsonLoader.run(path=config.benchmarkpath)
  reports = BenchmarkReportCrafter.run(input=data)
  x='N'
  y='elapsed'
  FunctionalPlotter.run(input={f"benchmark-{x}-{y}": reports},
                        crafter=None,
                        x=x, y=y,
                        scale=config.scale,
                        ranges=None,
                        trends=[{
                          'target': 'dcct::FFTWActuator',
                          'label': 'N^2 LogN',
                          'func': FastTrend
                        },{
                          'target': 'dcct::PocketFFTActuator',
                          'label': 'N^2 LogN',
                          'func': FastTrend
                        }
                        #,{
                        #  'target': 'dcct::FastActuator',
                        #  'label': 'N^3',
                        #  'func': SlowTrend
                        #}
                        ],
                        outdir=config.plotpath)

def do_compression(config: argparse.Namespace) -> None:
  reports = {
    'benchmark-compression': {
      'fftw': CsvLoader.run(path="benchmark.fftw.log"),
      'pocketfft': CsvLoader.run(path="benchmark.pocketfft.log")
    }
  }
  x='N'
  y='Time'
  FunctionalPlotter.run(input=reports,
                        crafter=None,
                        x=x, y=y,
                        scale=config.scale,
                        outdir=config.plotpath)

if __name__ == "__main__":
  action_map: dict[str, typing.Callable[[argparse.Namespace], None]] = {
    'benchmark': do_benchmark,
    'compression': do_compression
  }

  cli = argparse.ArgumentParser()
  cli.add_argument('verb', type=str, choices=action_map.keys(), help='action to perform')
  cli.add_argument('-b', '--benchmarkpath', type=str, default='./report.json', help='dass benchmark path')
  cli.add_argument('-p', '--plotpath', type=str, default='./plots/logs', help='dass plot path')
  cli.add_argument('-x', '--xmetric', type=str, default='N', choices=['TotalElapsed', 'Iteration'], help='which metric to plot on X axis')
  cli.add_argument('-y', '--ymetric', type=str, default='elapsed', choices=['elapsed'], help='which metric to plot on Y ayis')
  cli.add_argument('-s', '--scale', type=str, default='linlin', choices=['linlin', 'linlog', 'loglin', 'loglog'], help='scale of Xs/Ys in the plots')
  cli.add_argument('-r', '--ranges', type=float, nargs='*', help='optionally applies ranges to the plots limiting the output and fixing some axes')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  config = cli.parse_args(sys.argv[1:])

  if config.verbose:
    logging.getLogger().setLevel(logging.INFO)

  action_map[config.verb](config)
