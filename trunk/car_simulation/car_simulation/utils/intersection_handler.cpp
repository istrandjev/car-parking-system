#include "utils/intersection_handler.h"

#include "geometry/boundary_line.h"
#include "geometry/bounding_box.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "geometry/segment.h"
#include "geometry/straight_boundary_line.h"
#include "geometry/vector.h"
#include "utils/boundary_line_holder.h"
#include "utils/double_utils.h"
#include "utils/object_holder.h"

#include <algorithm>
#include <vector>

namespace utils {

static const double GAP_TOLERANCE = 0.6;

IntersectionHandler::IntersectionHandler(double minx, double maxx,
    double miny, double maxy, BoundaryLinesHolder* boundary_lines_holder)
        : grid_(minx, maxx, miny, maxy), 
          boundaryLinesHolder_(boundary_lines_holder) {}


void IntersectionHandler::Init(const ObjectHolder& object_holder) {
  const std::vector<geometry::RectangleObject*>& road_segments =
      object_holder.GetRoadSegments();

  for (unsigned index = 0; index < road_segments.size(); ++index) {
    grid_.AddRectangleObject(road_segments[index]);
  }

  const std::vector<geometry::RectangleObject*>& parking_lots =
      object_holder.GetParkingLots();
  
  for (unsigned index = 0; index < parking_lots.size(); ++index) {
    grid_.AddRectangleObject(parking_lots[index]);
  }

  const std::vector<geometry::RectangleObject*>& obstacles =
      object_holder.GetObstacles();
  
  for (unsigned index = 0; index < obstacles.size(); ++index) {
    grid_.AddRectangleObject(obstacles[index]);
  }

  for (unsigned index = 0; index < obstacles.size(); ++index) {
    geometry::Polygon bounds = obstacles[index]->GetBounds();
    for (unsigned side_index = 0; side_index < bounds.NumberOfSides();
        ++side_index) {
      grid_.AddBoundaryLine(boundaryLinesHolder_->
          AddStraightBoundaryLine(bounds.GetSide(side_index), false));
    }
  }

  for (unsigned index = 0; index < road_segments.size(); ++index) {
    AddBoundaryLinesForObject(road_segments[index]);
  }
  for (unsigned index = 0; index < parking_lots.size(); ++index) {
    AddBoundaryLinesForObject(parking_lots[index]);
  }

  RemoveSmallBoundaryLines();
}

std::vector<const geometry::BoundaryLine*> IntersectionHandler::
    GetBoundaryLines(const geometry::BoundingBox& bounding_box) const {
  return grid_.GetBoundaryLines(bounding_box);
}

std::vector<const geometry::BoundaryLine*> IntersectionHandler::
    GetBoundaryLines() const {
  return grid_.GetBoundaryLines();
}

bool Intersect(const geometry::Polygon& polygon,
               const geometry::Segment& segment,
               std::pair<double, double>* intersection) {
  double min_fraction, max_fraction;
  min_fraction = 1.0;
  max_fraction = 0.0;
  if (polygon.ContainsPoint(segment.A())) {
    min_fraction = 0.0;
  }
  if (polygon.ContainsPoint(segment.B())) {
    max_fraction = 1.0;
  }

  for (unsigned index = 0; index < polygon.NumberOfSides(); ++index) {
    double fraction;
    if (segment.Intersect(polygon.GetSide(index), &fraction)) {
      min_fraction = std::min(min_fraction, fraction);
      max_fraction = std::max(max_fraction, fraction);
    }
  }

  if (DoubleIsGreater(max_fraction, min_fraction)) {
    intersection->first = min_fraction;
    intersection->second = max_fraction;
    return true;
  }
  return false;
}

void IntersectionHandler::AddBoundaryLinesForObject(
    const geometry::RectangleObject* object){
  const geometry::Polygon bounds = object->GetBounds();

  for (unsigned side_index = 0; side_index < bounds.NumberOfSides();
      ++side_index) {
    geometry::Segment segment = bounds.GetSide(side_index);

    std::vector<const geometry::RectangleObject*> objects =
      grid_.GetRectangleObjects(segment.GetBoundingBox());
    
    std::vector<std::pair<double, double> > overlapped_intervals;
    for (unsigned index = 0; index < objects.size(); ++index) {
      if (objects[index] == object) {
        continue;
      }
      std::pair<double, double> interval;
      geometry::Polygon obj_bounds = objects[index]->GetBounds();
      if (Intersect(obj_bounds, segment, &interval)) {
        overlapped_intervals.push_back(interval);
      }
      
      obj_bounds.Translate(bounds.GetSideNormal(side_index).Unit() * GAP_TOLERANCE);
      if (Intersect(obj_bounds, segment, &interval)) {
        overlapped_intervals.push_back(interval);
      }
    }
    sort(overlapped_intervals.begin(), overlapped_intervals.end());
    double current_fraction = 0.0;
    for (unsigned index = 0; index < overlapped_intervals.size(); ++index) {
      if (DoubleIsGreater(overlapped_intervals[index].first,
          current_fraction)) {
        geometry::Segment sub_segment(segment.SubSegment(current_fraction,
            overlapped_intervals[index].first));

        grid_.AddBoundaryLine(boundaryLinesHolder_->
            AddStraightBoundaryLine(sub_segment, false));
      }

      current_fraction = std::max(current_fraction,
          overlapped_intervals[index].second);
    }

    if (DoubleIsGreater(1.0, current_fraction)) {
        geometry::Segment sub_segment(segment.SubSegment(current_fraction, 1.0));
        grid_.AddBoundaryLine(boundaryLinesHolder_->
            AddStraightBoundaryLine(sub_segment, false));
    }
  } 
}

void IntersectionHandler::RemoveSmallBoundaryLines() {
  std::vector<const geometry::BoundaryLine*> boundary_lines =
      grid_.GetBoundaryLines();
  for (unsigned index = 0; index < boundary_lines.size(); ++index) {
    if (DoubleIsGreater(boundary_lines[index]->GetLength(), GAP_TOLERANCE)) {
      continue;
    }
    geometry::BoundingBox bounding_box = 
        boundary_lines[index]->GetBoundingBox();
    std::vector<const geometry::BoundaryLine*> neighbouring_lines = 
        grid_.GetBoundaryLines(bounding_box.GetExpanded(GAP_TOLERANCE));

    bool has_long_neighbour = false;
    for (unsigned neighbour_index = 0; neighbour_index <
        neighbouring_lines.size(); ++neighbour_index) {
      const geometry::BoundaryLine* neighbour =
          neighbouring_lines[neighbour_index];
      if (DoubleIsGreater(neighbour->GetLength(),GAP_TOLERANCE) && 
          neighbour->GetBoundingBox().Intersect(bounding_box)) {
        has_long_neighbour = true;
        break;    
      }   
    }

    if (!has_long_neighbour) {
      grid_.RemoveBoundaryLine(boundary_lines[index]);
    }
  }
}

}  // namespace utils