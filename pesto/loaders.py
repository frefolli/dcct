"""
Service Loaders
"""
import pesto.generics
import pesto.logs
import yaml
import pandas
import json

class YmlLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    with open(self.path) as _in:
      return yaml.load(_in, Loader=yaml.Loader)

class JsonLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    with open(self.path) as _in:
      return json.load(_in)

class DataFrameLoader(pesto.generics.ServiceObject[pandas.DataFrame]):
  def exec(self) -> pandas.DataFrame:
    self.path: str
    return pandas.read_csv(self.path)

class CsvLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    df = DataFrameLoader.run(path=self.path)
    return {k:list(df[k]) for k in df}
