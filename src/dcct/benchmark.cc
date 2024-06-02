#include <dcct/benchmark.hh>
#include <dcct/logging.hh>
#include <json/json.h>
#include <filesystem>
#include <fstream>
#include <iostream>

bool dcct::LoadReport(dcct::Report& report, const std::string& filepath) {
  if (!std::filesystem::exists(filepath)) {
    return false;
  }
  report.filepath = filepath;

  std::ifstream in;
  in.open(filepath);
  Json::Value data;
  Json::Reader text_reader;
  if (!(text_reader.parse(in, data))) {
    dcct::LogWarning("syntax error while reading " + filepath);
    return false;
  }
  in.close();

  if (!data.isObject()) {
    dcct::LogWarning("expected an object inside " + filepath);
    return false;
  }

  report.data.clear();
  for (auto subtable = data.begin(); subtable != data.end(); ++subtable) {
    std::string key = subtable.key().asString();
    report.data[key] = {};

    if (!subtable->isArray()) {
      dcct::LogWarning("expected an array as " + subtable->toStyledString() + " children of report inside " + filepath);
      return false;
    }

    for (auto cell = subtable->begin(); cell != subtable->end(); ++cell) {
      if (!cell->isObject()) {
        dcct::LogWarning("expected objects inside array " + subtable->toStyledString() + " child of report inside " + filepath);
        return false;
      }

      report.data[key].push_back(dcct::Report::Cell {
        .N = cell->operator[]("N").asUInt(),
        .M = cell->operator[]("M").asUInt(),
        .elapsed = cell->operator[]("elapsed").asFloat(),
      });
    }
  }

  return true;
}

bool dcct::DumpReport(const dcct::Report& report) {
  Json::Value dump = Json::objectValue;
  for (std::pair<std::string, std::vector<dcct::Report::Cell>> subtable : report.data) {
    Json::Value records = Json::arrayValue;
    for (dcct::Report::Cell cell : subtable.second) {
      Json::Value record = Json::objectValue;
      record["N"] = cell.N;
      record["M"] = cell.M;
      record["elapsed"] = cell.elapsed;
      records.append(record);
    }
    dump[subtable.first] = records;
  }

  Json::StreamWriterBuilder stream_writer_builder;
  Json::StreamWriter* json_writer = stream_writer_builder.newStreamWriter();
  std::ofstream out;
  out.open(report.filepath);
  json_writer->write(dump, &out);
  out.close();
  delete json_writer;

  return true;
}