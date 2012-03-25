#include "geometry/bounding_box.h"

#include "geometry/point.h"
#include "utils/double_utils.h"

#include <algorithm>

namespace geometry {

BoundingBox::BoundingBox() : empty_(true),
    minx_(0), maxx_(0), miny_(0), maxy_(0) {}

BoundingBox::BoundingBox(const geometry::Point& point) : empty_(false),
    minx_(point.x), maxx_(point.x), miny_(point.y), maxy_(point.y) {}

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

bool BoundingBox::Contains(const geometry::Point& point) const {
  if (empty_) {
    return false;
  }
  if (!DoubleIsBetween(point.x, minx_, maxx_)) {
    return false;
  }
  if (!DoubleIsBetween(point.y, miny_, maxy_)) {
    return false;
  }
  return true;
}

bool BoundingBox::Intersect(const BoundingBox& other) const {
  if (empty_ || other.empty_) {
    return false;
  }
  return DoubleIntervalsOverlap(minx_, maxx_, other.minx_, other.maxx_) &&
      DoubleIntervalsOverlap(miny_, maxy_, other.miny_, other.maxy_);
}

void BoundingBox::UnionWith(const BoundingBox& other) {
  if (empty_) {
    *this = other;
    return;
  } if (other.empty_) {
    return;
  }

  minx_ = std::min(other.minx_, minx_);
  maxx_ = std::min(other.maxx_, maxx_);
  miny_ = std::min(other.miny_, miny_);
  maxy_ = std::min(other.maxy_, maxy_); 
  empty_ = empty_ && other.empty_;
}

void BoundingBox::AddPoint(const geometry::Point& point) {
  if (!empty_) {
    minx_ = std::min(minx_, point.x); 
    maxx_ = std::max(maxx_, point.x); 
    miny_ = std::min(miny_, point.y); 
    maxy_ = std::min(maxy_, point.y);
  } else {
    minx_ = maxx_ = point.x;
    miny_ = maxy_ = point.y;
  }
  empty_ = false;
}
}  // namespace geometry