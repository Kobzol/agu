import pygame
import math
import numpy as np

from coord import Coord
from vertex import Vertex
from face import Face
from edge import Edge


def point_line_distance(edge, coord):
    edge_dir = edge.get_dir()
    line_p = edge.v1
    n = (edge_dir[1], -edge_dir[0])

    c = -edge_dir[0] * coord.x - edge_dir[1] * coord.y;
    d = -n[0] * line_p.coord.x - n[1] * line_p.coord.y;

    a = np.array([[n[0], n[1]], [-edge_dir[0], -edge_dir[1]]])
    b = np.array([-d, c])

    print('a:', a)
    print('b:', b)
    x = np.linalg.solve(a, b)

    other_point = Coord(x[0], x[1])

    return math.sqrt( (coord.x - other_point.x)**2 + (coord.y - other_point.y)**2 )


class PlanMap(object):

    def __init__(self):
        self.firstV = None
        self.lastV = None
        self.firstE = None
        self.lastE = None
        self.firstF = None
        self.lastF = None
        self.vCount = 0
        self.eCount = 0
        self.fCount = 0

        vertex = self.create_vertex()
        edge = self.create_edge()
        face1 = self.create_face()
        face2 = self.create_face()

        vertex.coord.x = 10
        vertex.coord.y = 10

        edge.v1 = vertex
        edge.v2 = vertex
        edge.fc1 = face1
        edge.fc2 = face2
        edge.prev1 = edge
        edge.succ1 = edge
        edge.prev2 = edge
        edge.succ2 = edge

        face1.edge = edge
        face2.edge = edge

    def create_vertex(self):
        vertex = Vertex()
        self.vCount += 1
        vertex.next = None
        vertex.num = self.vCount

        if self.firstV is None:
            self.firstV = vertex
        if self.lastV is not None:
            self.lastV.next = vertex
        self.lastV = vertex

        return vertex

    def create_edge(self):
        edge = Edge()
        self.eCount += 1
        edge.next = None
        edge.num = self.eCount

        if self.firstE is None:
            self.firstE = edge
        if self.lastE is not None:
            self.lastE.next = edge
        self.lastE = edge

        return edge

    def create_face(self):
        face = Face()
        self.fCount += 1
        face.num = self.fCount

        if self.firstF is None:
            self.firstF = face
        if self.lastF is not None:
            self.lastF.next = face
        self.lastF = face

        return face

    def split_edge(self, edge, coord):
        new_vertex = self.create_vertex()
        new_vertex.coord = coord
        new_edge = self.create_edge()

        new_edge.v1 = new_vertex
        new_edge.v2 = edge.v2;
        new_edge.fc1 = edge.fc1
        new_edge.fc2 = edge.fc2
        new_edge.prev1 = edge
        new_edge.succ1 = edge.succ1
        new_edge.prev2 = edge.prev2
        new_edge.succ2 = edge

        edge.succ1.set_prev(edge.fc1, new_edge)
        edge.prev2.set_succ(edge.fc2, new_edge)

        edge.v2 = new_vertex
        edge.succ1 = new_edge
        edge.prev2 = new_edge

        return new_edge


    def split_face(self, face, edge1, edge2):
        new_edge = self.create_edge()
        new_face = self.create_face()
        prev_edge1 = edge1.get_prev(face)
        prev_edge2 = edge2.get_prev(face)

        new_edge.v1 = edge1.get_start_vetex(face)
        new_edge.v2 = edge2.get_start_vertex(face)
        new_edge.fc1 = face
        new_edge.fc2 = new_face
        new_edge.prev1 = prev_edge1
        new_edge.succ1 = edge2
        new_edge.prev2 = prev_edge2
        new_edge.succ2 = edge1

        prev_edge1.set_succ(face, new_edge)
        prev_edge2.set_succ(face, new_edge)
        edge1.set_prev(face, new_edge)
        edge2.set_prev(face, new_edge)
        face.edge = edge2
        new_face.edge = new_edge
        start_edge = new_ege
        edge = edge1

        edge.actualize_face(face, new_face)
        edge = edge.get_succ(new_face)

        while edge is not start_edge:
            edge.actualize_face(face, new_face)
            edge = edge.get_succ(new_face)

        return new_face

    """
  public Face splitFace(Face face, Edge edge1, Edge edge2) {
    Edge newEdge, prevEdge1, prevEdge2, startEdge, edge;
    Face newFace;

    newEdge = createEdge();
    newFace = createFace();
    prevEdge1 = edge1.getPrev(face);
    prevEdge2 = edge2.getPrev(face);

    newEdge.v1 = edge1.getStartVertex(face);
    newEdge.v2 = edge2.getStartVertex(face);
    newEdge.fc1 = face;
    newEdge.fc2 = newFace;
    newEdge.prev1 = prevEdge1;
    newEdge.succ1 = edge2;
    newEdge.prev2 = prevEdge2;
    newEdge.succ2 = edge1;

    prevEdge1.setSucc(face, newEdge);
    prevEdge2.setSucc(face, newEdge);
    edge1.setPrev(face, newEdge);
    edge2.setPrev(face, newEdge);
    face.edge = edge2;
    newFace.edge = newEdge;
    startEdge = newEdge;
    edge = edge1;

    do {
      edge.actualizeFace(face, newFace);
      edge = edge.getSucc(newFace);
    } while (edge == startEdge);

    return newFace;
  }

/*  public Face cutFace2(Face face, Coord point, Coord dir) {
    Edge edge, startEdge, edge1, edge2;
    Coord edgeDir, Section;
    float t1, t2;
    boolean exist = false;

    Section[] sections = new Section[2];
    int sectionCount = 0;

    startEdge = face.edge;
    edge = startEdge;
    do {
      edge = edge.getSucc(face);
//       Vec_Diff( Edge^.Get_End_Vertex( Face )^.Coord,
//                 Edge^.Get_Start_Vertex( Face )^.Coord, Edge_Dir );
//       Intersect( Point, Dir, Edge^.Get_Start_Vertex( Face )^.Coord,
//                 Edge_Dir, Exist, T1, T2, Section );
      exist = exist && (t2 > 0) && (t2 <= 1.0);
      if (exist) {
        sectionCount++;
        sections[sectionCount].edge = edge;
        sections[sectionCount].coord = Section; //nechapu
      }
    } while ((edge == startEdge) || (sectionCount == 2));

    if (sectionCount == 2) {
      edge1 = splitEdge(sections[0].edge, sections[0].coord);
      edge2 = splitEdge(sections[1].edge, sections[1].coord);
//       Vec_Diff( Sections[2].Coord, Sections[1].Coord, Edge_Dir );
/*       if (Scal_Mul( Dir, Edge_Dir ) < 0) {
    return  splitFace( face, edge1, edge2 );
    } else {
    return splitFace( face, edge2, edge1 );
    }
     */
/*}
  }
*/  

    """

    def __str__(self):
        so = "===   Double Connected Edge List   ===\n"
        vertex = self.firstV

        while vertex is not None:
            so += str(vertex)
            so += '\n'
            vertex = vertex.next

        edge = self.firstE

        while edge is not None:
            so += str(edge)
            so += '\n'
            edge = edge.next

        face = self.firstF

        while face is not None:
            so += str(face)
            so += '\n'
            face = face.next

        return so
    """
  @Override
  public String toString() {
    Vertex vertex;
    Edge edge;
    Face face;
    StringBuilder sb = new StringBuilder("===   Double Connected Edge List   ===\n");
    vertex = firstV;

    while (vertex != null) {
      sb.append(vertex.toString() + "\n");
      vertex = vertex.next;
    }
    sb.append("\n");
    edge = firstE;

    while (edge != null) {
      sb.append(edge.toString() + "\n");
      edge = edge.next;
    }
    sb.append("\n");

    face = firstF;

    while (face != null) {
      sb.append(face.toString());
      face = face.next;
    }
    sb.append("\n");

    return sb.toString();
  }
    """

    def draw(self, surface):
        #pygame.draw.line(surface, (255, 255, 255), (0, 0), (100, 100), 5)

        edge = self.firstE
        while edge is not None:
            if edge == self.firstE:
                edge.draw(surface, first=True)
            elif edge == self.lastE:
                edge.draw(surface, last=True)
            else:
                edge.draw(surface)
            edge = edge.next

        vertex = self.firstV
        while vertex is not None:
            #if edge == self.firstE:
            #    edge.draw(surface, first=True)
            #elif edge == self.lastE:
            #    edge.draw(surface, last=True)
            #else:
            vertex.draw(surface)
            vertex = vertex.next

    def nearest_edge(self, coord):
        nearest_edge = None
        edge = self.firstE
        min_dist = 1.0e5

        while edge is not None:
            dist = point_line_distance(edge, coord)

            if dist < min_dist:
                min_dist = dist
                nearest_edge = edge
            edge = edge.next

        return nearest_edge

