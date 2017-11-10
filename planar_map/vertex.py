import pygame

from coord import Coord

class Vertex(object):

    def __init__(self):
        self.coord = Coord()
        self.next = None
        self.num = None
  
    def draw(self, surface):
        if pygame.font:
            font = pygame.font.Font(None, 20)
            text = font.render("V= %d" % self.num, 1, (0, 255, 0))
            textpos = pygame.Rect((self.coord.x + 5, self.coord.y), (30, 30))
            surface.blit(text, textpos)

    def __str__(self):
        return "Vertex [{0}, {1}]".format(self.coord.x, self.coord.y)
