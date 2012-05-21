#include "geometry/polygon.h"

#include "geometry/bounding_box.h"
#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/segment.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace geometry {

Polygon::Polygon(const std::vector<Point>& points) : points_(points) {
  Normalize();
}

void Polygon::Normalize() {
  double total_area = 0.0;
  for (unsigned index = 0; index < NumberOfVertices(); ++index) {
    total_area += GeometryUtils::GetOrientedArea(GetPoint(0), GetPoint(index),
      GetPointCyclic(index + 1));
  }

  if (DoubleIsGreater(0.0, total_area)) {
    std::reverse(points_.begin(), points_.end());
  }
}

void Polygon::AddPointDropDuplicates(const Point& point) {
  if (points_.empty()) {
    points_.push_back(point);
  } else {
    const Point& last = points_.back();
    if (!DoubleEquals(last.x, point.x) || !DoubleEquals(last.y, point.y)) {
      points_.push_back(point);
    }
  }
}

unsigned Polygon::NumberOfVertices() const {
  return points_.size();
}

unsigned Polygon::NumberOfSides() const {
  return points_.size();
}

const Point& Polygon::GetPoint(unsigned index) const {
  return points_[index];
}

const Point& Polygon::GetPointCyclic(int index) const {
  index = index % static_cast<int>(points_.size());
  if (index <  0) {
    index += static_cast<int>(points_.size());
  }
  return points_[index];
}

Segment Polygon::GetSide(unsigned side_index) const {
  if (side_index + 1 != NumberOfSides()) {
    return Segment(GetPoint(side_index), GetPoint(side_index + 1));
  } else {
    return Segment(GetPoint(side_index), GetPoint(0));
  }
}

Vector Polygon::GetSideNormal(unsigned side_index) const {
  const geometry::Point& A = GetPoint(side_index);
  const geometry::Point& B = GetPointCyclic(side_index + 1);
  const geometry::Point& C = GetPointCyclic(side_index + 2);

  Vector side_vector(A, B);
  if (geometry::GeometryUtils::InNegativeSemiPlane(C, A, B)) {
    return Vector(side_vector.y, -side_vector.x);
  } else {
    return Vector(-side_vector.y, side_vector.x);  
  }
}

BoundingBox Polygon::GetBoundingBox() const {
  geometry::BoundingBox bounding_box;
  for (unsigned i = 0; i < points_.size(); ++i) {
    bounding_box.AddPoint(points_[i]);
  }
  return bounding_box;
}

bool Polygon::ContainsPoint(const Point& point) const {
  int intersections = 0;

  // Check if the point lies on the boundary.
  for (unsigned index = 0; index < points_.size(); ++index) {
    if (GeometryUtils::LiesOnLine(GetPoint(index), GetPointCyclic(index + 1),
                                  point)) {
      geometry::BoundingBox bb;
      bb.AddPoint(GetPoint(index));
      bb.AddPoint(GetPointCyclic(index + 1));
      return bb.Contains(point);
    }
  }

  for (unsigned index = 0; index < points_.size(); ++index) {
    const Point& current = GetPoint(index);
    const Point& next = GetPointCyclic(index + 1);

    int sign1 = DoubleSign(point.x - current.x);
    if (sign1 == 0) {
      sign1 = -1;
    }

    int sign2 = DoubleSign(point.x - next.x);
    if (sign2 == 0) {
      sign2 = -1;
    }

    if (sign1 != sign2) {
      double y =
          (current.y * (next.x - point.x) + next.y * (point.x - current.x)) /
              (next.x - current.x);
      if (DoubleIsGreater(y, point.y)) {
        intersections++;
      }
    }
  }

  return intersections % 2 == 1;
}

void Polygon::Translate(const Vector& translation_vector) {
  for (unsigned index = 0; index < points_.size(); ++index) {
    points_[index] += translation_vector;
  }
}

void Polygon::Reset() {
  points_.clear();
}

std::istream& operator>>(std::istream& in, Polygon& polygon) {
  int number_of_points;
  in >> number_of_points;
  std::string temp;

  polygon.points_.resize(number_of_points);

  getline(in, temp);
  for (int index = 0; index < number_of_points; ++index) {
    getline(in, temp);
    for (unsigned i = 0; i < temp.size(); ++i) {
      if (temp[i] == ',') {
        temp[i] = ' ';
        break;
      }
    }
    std::istringstream in(temp);
    in >> polygon.points_[index].x >> polygon.points_[index].y;
  }
  polygon.Normalize();

  return in;
}

std::ostream& operator<<(std::ostream& out, const Polygon& polygon) {
  out << polygon.points_.size() << std::endl;
  for (unsigned index = 0; index < polygon.points_.size(); ++index) {
    out << polygon.points_[index].x << ", " << polygon.points_[index].y
      << std::endl;
  }

  return out;
}

}  // namespace geometry
