#include "geometry/bounding_box.h"

#include "utils/double_utils.h"

namespace geometry {

BoundingBox::BoundingBox() : empty_(true),
    minx_(0), maxx_(0), miny_(0), maxy_(0) {}

BoundingBox::BoundingBox(double minx, double maxx, double miny, double maxy)
    : empty_(false), minx_(minx), maxx_(maxx), miny_(miny), maxy_(maxy) {}
  
bool BoundingBox::IsEmpty() const {
  return empty_;
}

double BoundingBox::GetMinX() const {
  return minx_;
}

double BoundingBox::GetMaxX() const {
  return maxx_;
}

double BoundingBox::GetMinY() const {
  return miny_;
}

double BoundingBox::GetMaxY() const {
  return maxy_;
}

BoundingBox BoundingBox::GetExpanded(double value) {
  return BoundingBox(minx_ - value, maxx_ + value,
                     miny_ - value, maxy_ + value);
}

bool BoundingBox::Intersect(const BoundingBox& other) const {
  return DoubleIntervalsOverlap(minx_, maxx_, other.minx_, other.maxx_) &&
      DoubleIntervalsOverlap(miny_, maxy_, other.miny_, other.maxy_);
}

}  // namespace geometry