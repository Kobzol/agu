import pygame

class Edge(object):

    def __init__(self):
        self.next = None
        self.prev1 = None
        self.succ1 = None
        self.prev2 = None
        self.succ2 = None
        self.fc1 = None
        self.fc2 = None
        self.v1 = None
        self.v2 = None
        self.num = None

    def get_prev(self, face):
        if face is self.fc1:
            return self.prev1
        elif face is self.fc2:
            return self.prev2
        else:
            return None

    def get_succ(self, face):
        if face is self.fc1:
            return self.succ1
        elif face is self.fc2:
            return self.succ2
        else:
            return None

    def get_start_vertex(self, face):
        if face is self.fc1:
            return self.v1
        elif face is self.fc2:
            return self.v2
        else:
            return None

    def get_end_vertex(self, face):
        if face is self.fc1:
            return self.v2
        elif face is self.fc2:
            return self.v1
        else:
            return None

    #def get_dir(self, face, dir):
    #    print('NEJEDE')
    #    #//Vec_Diff( Get_End_Vertex( Face )^.Coord, Get_Start_Vertex( Face )^.Coord, Dir );
    #    return (self.v2.x - self.v1.x, self.v2.y - self.v1.y)

    def get_dir(self):
        print('NEJEDE')
        return (self.v2.coord.x - self.v1.coord.x, self.v2.coord.y - self.v1.coord.y)

    def is_left_face(self, face):
        return face is self.fc1

    def is_right_face(self, face):
        return face is self.fc2

    def get_oposite_face(self, face):
        if face is self.fc1:
            return self.fc2
        elif face is self.fc2:
            return self.fc2
        else:
            return None

    def set_prev(self, face, prev):
        if face is self.fc1:
            self.prev1 = prev
        elif face is self.fc2:
            self.prev2 = prev

    def set_succ(self, face, succ):
        if face is self.fc1:
            self.succ1 = succ
        elif face is self.fc2:
            self.succ2 = succ

    def set_start_vertex(self, face, vertex):
        if face is self.fc1:
            self.v1 = vertex
        elif face is self.fc2:
            self.v2 = vertex

    def set_end_vertex(self, face, vertex):
        if face is self.fc1:
            self.v2 = vertex
        elif face is self.fc2:
            self.v1 = vertex

    def actualize_face(self, face, new_face):
        if face is self.fc1:
            self.fc1 = new_face
        elif face is self.fc2:
            self.fc2 = new_face

    def midpoint(self):
        x1 = self.v1.coord.x
        y1 = self.v1.coord.y
        x2 = self.v2.coord.x
        y2 = self.v2.coord.y

        return ((( x2 - x1 ) / 2), (( y2 - y1 ) / 2))

    def draw(self, surface, **kw):
        x1 = self.v1.coord.x
        y1 = self.v1.coord.y
        x2 = self.v2.coord.x
        y2 = self.v2.coord.y

        if 'last' in kw:
            if kw['last']:
                pygame.draw.line(surface, (255, 50, 50), (x1, y1), (x2, y2), 3)
        if 'first' in kw:
            if kw['first']:
                pygame.draw.line(surface, (50, 255, 50), (x1, y1), (x2, y2), 3)
        else:
            pygame.draw.line(surface, (255, 255, 255), (x1, y1), (x2, y2), 1)

        if pygame.font:
            midpoint = self.midpoint()
            font = pygame.font.Font(None, 24)
            text = font.render("E= %d" % self.num, 1, (255, 0, 0))
            textpos = pygame.Rect((x1 + midpoint[0], y1 + midpoint[1]), (30, 30))
            surface.blit(text, textpos)


    def __str__(self):
        so = "Edge [ {0} ]:\n" + "\tV1= {1} V2= {2}\n" + "\tF1= {3} F2= {3}\n" + "\tP1= {4} S1= {5}\n" + "\tP2= {6} S2= {7}\n"
        so = so.format(self.num, self.v1.num, self.v2.num, self.fc1.num, self.fc2.num, self.prev1.num, self.succ1.num, self.prev2.num, self.succ2.num)
        
        return so

