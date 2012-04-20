#include "utils/object_holder.h"

#include "geometry/directed_rectangle_object.h"
#include "geometry/rectangle_object.h"

#include <fstream>
#include <stdexcept>
#include <string>

namespace utils {

geometry::RectangleObject* RectangleObjectFactory(
    const std::string& serialized) {
  geometry::RectangleObject* res;
  if (geometry::DirectedRectangleObject::IsSerializedDirectedRectangleObject(
      serialized)) {
    res = new geometry::DirectedRectangleObject();
  } else {
    res = new geometry::RectangleObject();
  }
  res->Parse(serialized);
  return res;
}

void ObjectHolder::DumpToFile(const std::string& file_path) const {
  std::ofstream out(file_path.c_str());
  if (!out) {
    throw std::runtime_error("Could not open the file to serialize into.");
    return;
  }

  out << roadSegments_.size() << std::endl;
  for (unsigned index = 0; index < roadSegments_.size(); ++index) {
    out << roadSegments_[index]->Serialize() << std::endl;
  }

  out << parkingLots_.size() << std::endl;
  for (unsigned index = 0; index < parkingLots_.size(); ++index) {
    out << parkingLots_[index]->Serialize() << std::endl;
  }

  out << obstacles_.size() << std::endl;
  for (unsigned index = 0; index < obstacles_.size(); ++index) {
    out << obstacles_[index]->Serialize() << std::endl;
  }
}

void ObjectHolder::ParseFromFile(const std::string& file_path) {
  std::ifstream in(file_path.c_str());
  if (!in) {
    throw std::runtime_error("Could not open file to parse.");
    return;
  }

  DeleteObjects();
  std::string serialized;

  unsigned road_segments_size;
  in >>  road_segments_size;
  getline(in, serialized);
  for (unsigned index = 0; index < road_segments_size; ++index) {
    getline(in, serialized);
    roadSegments_.push_back(RectangleObjectFactory(serialized));
  }

  unsigned parking_lots_size;
  in >>  parking_lots_size;
  getline(in, serialized);
  for (unsigned index = 0; index < parking_lots_size; ++index) {
    getline(in, serialized);
    parkingLots_.push_back(RectangleObjectFactory(serialized));
  }

  unsigned obstacles_size;
  in >>  obstacles_size;
  getline(in, serialized);
  for (unsigned index = 0; index < obstacles_size; ++index) {
    getline(in, serialized);
    obstacles_.push_back(RectangleObjectFactory(serialized));
  }
}

}  // namespace utils
