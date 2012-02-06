#include "polygon.h"

#include "double_utils.h"
#include "geometry_utils.h"
#include "point.h"

#include <iostream>
#include <sstream>
#include <string>

namespace geometry {

Polygon::Polygon(const std::vector<Point>& points) : points_(points){
  Normalize();
}

void Polygon::Normalize() {
  double total_area = 0.0;
  for (unsigned index = 0; index < NumberOfVertices(); ++index) {
    total_area += GeometryUtils::GetOrientedArea(GetPoint(0), GetPoint(index), 
      GetPointCyclic(index + 1));
  }

  if (DoubleIsGreater(0.0, total_area)) {
    reverse(points_.begin(), points_.end());
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

inline const Point& Polygon::GetPoint(unsigned index) const {
  return points_[index];
}

inline const Point& Polygon::GetPointCyclic(unsigned index) const {
  return points_[index % points_.size()];
}

bool Polygon::ContainsPoint(const Point& point) const {
  int intersections = 0;
  
  // Check if the point lies on the boundary.
  for (unsigned index = 0; index < points_.size(); ++index) {
    if (GeometryUtils::LiesOnLine(GetPoint(index), GetPointCyclic(index + 1), point)) {
      return true;
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
      double y = (current.y * (next.x - point.x) + next.y * (point.x - current.x)) 
          / (next.x - current.x);
      if (DoubleIsGreater(y, point.y)) {
        intersections++;
      }
    }
  }

  return intersections % 2 == 1;
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
