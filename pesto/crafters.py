"""
Crafting Unit
"""
import pesto.generics
import pesto.logs
import numpy

class DictOfListsToDictOfArrays(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.payload: dict
    return {k:numpy.array(self.payload[k]) for k in self.payload}

class BenchmarkReportCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.input: dict
    reports: dict = {}
    for solver in self.input:
      subreport = {}
      for key in self.input[solver][0].keys():
        subreport[key] = [record[key] for record in self.input[solver]]
      reports[solver] = subreport
    return reports
