#include "geometry/bounding_box.h"

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

}  // namespace geometry