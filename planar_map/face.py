class Face(object):

    def __init__(self):
        self.next = None
        self.edge = None
        self.num  = None

    def draw(self, g):
        p = []
        e = self.edge

        while e is not None:
            p.append([e.v1.coord.x, e.v1.coord.y])
            e = e.next

        """
        Polygon p = new Polygon();
    
        Edge e = edge;
        while (e != null) {
          p.addPoint(e.v1.coord.x, e.v1.coord.y);
          e = e.next;
        }
        g.fillPolygon(p);
        """

        pygame.draw.polygon(surface, (128, 128, 128), [(10, 10), (10, 100), (100, 100)], 5)

    def __str__(self):
        return "Face [ {0} ]: First Edge: {0}".format(self.num, self.num)
