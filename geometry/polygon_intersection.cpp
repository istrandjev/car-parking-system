#include "geometry/polygon.h"

#include "utils/double_utils.h"
#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/segment.h"

#include <map>
#include <vector>

namespace geometry {

// Used for nodes in the polygon list created for a given polygon.
enum ExitEnter {
  // The point stored in the node is side-intersection point and the segment
  // after that point *does not* belong to the intersection of the two polygons.
  EXIT = 0,

  // The point stored in the node is side-intersection point and the segment
  // after that point belongs to the intersection of the two polygons.
  ENTER = 1,

  // The point stored in the node is not side-intersection point. The only such
  // points in the list should be vertices of the initial polygon.
  NEITHER
};

// Base class for polygon list nodes. A Polygon is represented in a list 
// consisting of such nodes. The points stored in the nodes represent all the 
// vertices of the polygon and all the intersection points with the sides of the 
// other polygon. The points in the list are ordered as would be met
// when traversing the polygon boundary in counter-clockwise direction. 
class PolygonListNode {
 public: 
   PolygonListNode() : next(NULL), prev(NULL), type(NEITHER), neighbour(NULL), 
     fraction(0.0), used(false), nextInOuput(NULL) {}

   virtual ~PolygonListNode(){};

  // @return - if the point stored in the node is vertex of the polygon.
  virtual bool IsVertex() const {
    return false;
  }
 
public:
  Point vertex;
  PolygonListNode* next;
  PolygonListNode* prev;
  ExitEnter type;
  PolygonListNode* neighbour;
  double fraction;
  bool used;

  // This pointer is used to find articulation points when creating output
  // polygons and is NULL except of when computing the output.
  PolygonListNode* nextInOuput;
};

// A node that stores a polygon vertex. In addition to a regular PolygonListNode
// it stores which is the next vertex of the given polygon and also stores 
// references to all intersection points lying on the edge starting from this
// vertex.
struct PolygonVertexNode : public PolygonListNode {
 public:
  virtual bool IsVertex() const {
    return true;
  }
 
  // @return - the next vertex when traversing the given polygon in 
  // counter-clockwise direction.
  PolygonVertexNode* NextVertex() const {
    return nextVertex_;
  }

 public:
  // Stores all the intersection points on the edge starting from the given 
  // vertex. This is used to faster find where does a new intersection point go
  // into the PolygonList.
  std::map<double, PolygonListNode*> intersections;
  
  // Next vertex when traversing the polygon in CCW direction.
  PolygonVertexNode* nextVertex_;
};

// Represents all the vertices of a given polygon and also stores all the 
// intersection points with the sides of the other polygon. The list places the
// points in the same order as they would be visited if traversing all the
// points in CCW direction. The list is cyclic and double-linked.
class PolygonList {
 public:
  PolygonList(const Polygon& polygon);
  ~PolygonList();

  // @return - the first vertex of the polygon.
  PolygonVertexNode* GetFront();

  // Inserts a new intersection point on the edge starting from "polygon_vertex"
  // in such a way that the order of the intersection points on that edge is 
  // the same as when traversing them along the edge. Returns a pointer to the 
  // inserted intersection point. Also handles cases when the point dividing the
  // edge in fraction has already been added.
  // @param intersection - the intersection point to be inserted.
  // @param fraction - the fraction in which intersection divides the edge.
  // @param polygon_vertex - the vertex from which starts the edge on which
  //     "intersection" lies.
  static PolygonListNode* AddIntersectionPointAfter(const Point& intersection, 
      double fraction, PolygonVertexNode* polygon_vertex);

private:
  PolygonVertexNode* front_;
};

PolygonList::PolygonList(const Polygon& polygon) {
  // Allocate all nodes at the same time.
  int vertex_count = polygon.NumberOfVertices();
  PolygonVertexNode* nodes = new PolygonVertexNode[vertex_count];
  
  for (int index = 0; index < vertex_count; ++index) {
    nodes[index].vertex = polygon.GetPoint(index);
    
    if (index + 1 < vertex_count) {
      nodes[index].nextVertex_ = nodes + index + 1;
    } else {
      nodes[index].nextVertex_ = nodes;
    }

    if (index > 0) {
      nodes[index].prev = nodes + index - 1;
    } else {
      nodes[index].prev = nodes + vertex_count - 1;
    }

    nodes[index].next = nodes[index].nextVertex_;
    nodes[index].intersections.insert(std::make_pair(1.0, nodes[index].next));
    nodes[index].type = NEITHER;
  }

  front_ = nodes;
}

PolygonVertexNode* PolygonList::GetFront() {
  return front_;
}

// static
PolygonListNode* PolygonList::AddIntersectionPointAfter(
    const Point& intersection, double fraction,
    PolygonVertexNode* polygon_vertex) {
  if (DoubleEquals(fraction, 0.0)) {
    return polygon_vertex;
  }

  if (DoubleEquals(fraction, 1.0)) {
    return polygon_vertex->NextVertex();
  }

  PolygonListNode* current = polygon_vertex->next;
  
  std::map<double, PolygonListNode*>::iterator iter = 
    polygon_vertex->intersections.lower_bound(fraction - epsylon);

  if (iter == polygon_vertex->intersections.end()) {
    current = polygon_vertex->intersections.rend()->second;
  } else {
    current = iter->second;
  }

  // Add the new node.
  PolygonListNode* new_node = new PolygonListNode;
  new_node->next = current;
  new_node->prev = current->prev;
  new_node->fraction = fraction;
  new_node->vertex = intersection;

  // Fix the links of the neighboring nodes in the list.
  current->prev->next = new_node;
  current->prev = new_node;

  polygon_vertex->intersections.insert(std::make_pair(fraction, new_node));
  return new_node;
}

PolygonList::~PolygonList() {
  PolygonListNode* current = front_->next;
  
  while (current != front_) {
    PolygonListNode* temp = current;
    current = current->next;
    if (temp->IsVertex() == false) {
      delete temp;
    }
  }

  delete [] front_;
}

// Determines if the ray with origin in "vertex" and end point in "ray_end"
// is contained within the oriented angle <(prev_vertex, vertex, next_vertex)
// @return ENTER if the ray is contained within the oriented angle and EXIT
//     otherwise.
ExitEnter GetIntersectionType(const Point& prev_vertex, const Point& vertex, 
    const Point& next_vertex, const Point& ray_end) {
  if (GeometryUtils::InNegativeSemiPlane(vertex, next_vertex, prev_vertex)) {
    if (!GeometryUtils::InNegativeSemiPlane(vertex, next_vertex, ray_end) || 
        !GeometryUtils::InNegativeSemiPlane(prev_vertex, vertex, ray_end)) {
      return ENTER;
    } else {
      return EXIT;
    }
  } else {
    if (!GeometryUtils::InNegativeSemiPlane(vertex, next_vertex, ray_end) && 
      !GeometryUtils::InNegativeSemiPlane(prev_vertex, vertex, ray_end)) {
        return ENTER;
    } else {
      return EXIT;
    }
  }
}

// Determines the type of the intersection(EXIT/ENTER). Intersection node is
// the PolygonListNode that represents the intersection point in one of the 
// polygons while "vertex" is the beginning of the edge with which the 
// intersection happens in the other polygon. "fraction" is the fraction in 
// which the intersection point divides the edge of the other polygon.
// @vertex - a PolygonListNode that stores the beginning of the edge in the 
//     other polygon with which the intersection occurs.
// @fraction - the fraction in which the intersection divides the edge of the
//     other polygon.
// @intersection_node - node representing the intersection in the polygon where
//     we want to determine it's type. Output parameter. Can not be NULL.
void DetermineIntersectionType(const PolygonListNode* vertex, double fraction,
    PolygonListNode* intersection_node) {
  const Point& next_point = intersection_node->next->vertex;

  if (DoubleEquals(fraction, 0.0)) {
    intersection_node->type = GetIntersectionType(vertex->prev->vertex,
      vertex->vertex, vertex->next->vertex, next_point);
  } else if (DoubleEquals(fraction, 1.0)) {
    intersection_node->type = GetIntersectionType(vertex->vertex,
      vertex->next->vertex, vertex->next->next->vertex, next_point);
  } else {
    if (!GeometryUtils::InNegativeSemiPlane(vertex->vertex, vertex->next->vertex, 
                                            next_point)) {
      intersection_node->type = ENTER;
    } else {
      intersection_node->type = EXIT;
    }
  }
}

bool Intersect(const Segment& segment1, const Segment& segment2,
  double* fraction1, double* fraction2, Point* intersection_point) {
  double vector1x = segment1.B_.x - segment1.A_.x;
  double vector1y = segment1.B_.y - segment1.A_.y;

  double vector2x = segment2.B_.x - segment2.A_.x;
  double vector2y = segment2.B_.y - segment2.A_.y;

  double determinant = vector2x * vector1y - vector2y * vector1x;
  if (DoubleIsZero(determinant)) {
    return false;
  }

  double x_diff = segment1.A_.x - segment2.A_.x;
  double y_diff = segment1.A_.y - segment2.A_.y;

  *fraction1 = (vector2y * x_diff - vector2x * y_diff) / determinant;
  if (DoubleIsGreater(0.0, *fraction1) || DoubleIsGreater(*fraction1, 1.0)) {
    return false;
  }

  *fraction2 = (vector1y * x_diff - vector1x * y_diff) / determinant;
  if (DoubleIsGreater(0.0, *fraction2) || DoubleIsGreater(*fraction2, 1.0)) {
    return false;
  }

  intersection_point->x = segment1.A_.x * (1.0 - *fraction1) + 
      segment1.B_.x * (*fraction1);
  intersection_point->y = segment1.A_.y * (1.0 - *fraction1) + 
      segment1.B_.y * (*fraction1);

  return true;
}

// Compute all the intersection points of a side of one of the polygons with
// a side of the other. This is done by iterating over all pair of sides. The
// points are inserted in the correct order.
void ComputeAndInsertIntersectionPoints(PolygonList* list1, 
                                        PolygonList* list2) {
  PolygonVertexNode* current_vertex1 = list1->GetFront();
  PolygonVertexNode* next_vertex1 = current_vertex1->NextVertex();
  do {
    PolygonVertexNode* current_vertex2 = list2->GetFront();
    PolygonVertexNode* next_vertex2 = current_vertex2->NextVertex();

    do {
      Segment segment1(current_vertex1->vertex, next_vertex1->vertex);
      Segment segment2(current_vertex2->vertex, next_vertex2->vertex);
      double fraction1, fraction2;
      Point intersection;

      // Insert intersection point if there is such.
      if (Intersect(segment1, segment2, &fraction1, &fraction2, 
        &intersection)) {
          PolygonListNode* inserted1 = PolygonList::AddIntersectionPointAfter(
            intersection, fraction1, current_vertex1);
          PolygonListNode* inserted2 = PolygonList::AddIntersectionPointAfter(
            intersection, fraction2, current_vertex2);

          DetermineIntersectionType(current_vertex2, fraction2, inserted1);
          DetermineIntersectionType(current_vertex1, fraction1, inserted2);

          inserted1->neighbour = inserted2;
          inserted2->neighbour = inserted1;
      }

      current_vertex2 = next_vertex2;
      next_vertex2 = current_vertex2->NextVertex();
    } while (current_vertex2 != list2->GetFront());
    current_vertex1 = next_vertex1;
    next_vertex1 = current_vertex1->NextVertex();
  } while (current_vertex1 != list1->GetFront());
}

// Adds the loop, starting from node following the nextInOuput links, to the 
// back of "polygons". Also resets the nextInOutput pointers to NULL.
// @param - beginning node of the loop. Next node in the loop is stored in 
//     node->nextInOutput and so on. Can not be NULL.
// @param polygons - output parameter. The new polygon will be appended to its
//     end. Can not be NULL.
void AppendLoopFromNode(PolygonListNode* node, std::vector<Polygon>* polygons) {
  PolygonListNode* loop_vertex = node;
  polygons->push_back(Polygon());
  while (loop_vertex != NULL) {
    polygons->back().AddPointDropDuplicates(loop_vertex->vertex);
    PolygonListNode* temp = loop_vertex;
    loop_vertex = loop_vertex->nextInOuput;
    temp->nextInOuput = NULL;
  }
}

// Computes the polygons that comprise the intersection of the two polygons
// represented in list1 and list2. All the intersection points of sides of 
// either of the polygons with the other should already be present in these
// lists.
// @param list1 - stores the vertexes and intersection points along the sides
//     of the first polygon as would be traversed in CCW direction.
// @param list2 - stores the vertexes and intersection points along the sides
//     of the first polygon as would be traversed in CCW direction.
// @param intersection - output parameter that stores the polygons comprising
//     the intersection. Will get cleared if not empty on function call.
void ComputePolygonsComprisingIntersection(
    PolygonList* list1, PolygonList* list2, 
    std::vector<Polygon>* intersections) {
  intersections->clear();

  PolygonListNode* node = list1->GetFront();
  
  do {
    if (node->used == false && node->type == ENTER) {
      node->used = true;
      
      PolygonListNode* previous_node = node;
      
      bool in_list1 = true;

      PolygonListNode* current_node = node->next;
      while (current_node != node && current_node != node->neighbour) {
        // Check if this vertex is already added in the once that have not yet
        // been assign to an intersection polygon. Always use the pointer in 
        // list1 for the comparison.
        PolygonListNode* node_in_list1;
        if (in_list1 || current_node->type == NEITHER) {
          node_in_list1 = current_node;
        } else {
          node_in_list1 = current_node->neighbour;
        }

        // If this vertex has already been visited, detach the whole loop
        // and add it as one of the intersection polygons.
        if (node_in_list1->nextInOuput != NULL) {
          AppendLoopFromNode(node_in_list1, intersections);
        } else {
          // If the node was already present in the list formed by nextInOuput
          // we need not change the nextInOuput pointer of the previous node.
          previous_node->nextInOuput = node_in_list1;
        }

        previous_node = node_in_list1;

        if (current_node->type == NEITHER) {
          current_node = current_node->next;
          continue;
        }

        current_node->used = true;
        current_node->neighbour->used = true;

        if (current_node->type == EXIT) {
          current_node = current_node->neighbour->next;
          in_list1 = !in_list1;
        } else {
          current_node = current_node->next;
        }
      }

      // Add the loop passing starting from "node" as an intersection polygon.
      AppendLoopFromNode(node, intersections);
    }

    node = node->next;
  } while (node != list1->GetFront());
}

void Intersect(const Polygon& poly1, const Polygon& poly2, 
    std::vector<Polygon>* intersections) {
  PolygonList list1(poly1);
  PolygonList list2(poly2);

  ComputeAndInsertIntersectionPoints(&list1, &list2);
  ComputePolygonsComprisingIntersection(&list1, &list2, intersections);
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
    if (intersection != NULL) {
      intersection->first = min_fraction;
      intersection->second = max_fraction;
    }
    return true;
  }
  return false;
}

}  // namespace geometry
