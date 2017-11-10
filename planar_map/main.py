import os
import sys
import pygame
from pygame.locals import *

from plan_map import PlanMap
from coord import Coord

if not pygame.font: print('Warning, fonts disabled')
if not pygame.mixer: print('Warning, sound disabled')


class PlanarMapMain(object):
    
    def __init__(self, width=440,height=480):
        pygame.init()
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((self.width, self.height))
        self.plan_map = PlanMap()
        self.first_vertex = True
        self.nearest_edge = None

    def draw(self):
        self.screen.blit(self.background, (0, 0))     
        #self.raptor_sprites.draw(self.screen)
        #self.enemy_sprites.draw(self.screen)
        #if self.bullet_sprites != None:
            #    self.bullet_sprites.draw(self.screen)

        self.plan_map.draw(self.screen)

        pygame.display.flip()

    def handle_keys(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: 
                sys.exit()
            elif event.type == KEYDOWN:
                pass
            elif event.type == KEYUP:
                pass
            elif event.type == MOUSEBUTTONDOWN:
                print(self.plan_map)
                print('BUTTON:', event.button)
                x, y = event.pos
                if event.button == 1:
                    if self.first_vertex:
                        self.plan_map.firstV.x = x
                        self.plan_map.firstV.y = y
                        self.first_vertex = not self.first_vertex
                    elif self.nearest_edge is None:
                        self.plan_map.split_edge(self.plan_map.lastE, Coord(x, y))
                    else:
                        self.plan_map.split_edge(self.nearest_edge, Coord(x, y))
                elif event.button == 3:
                    nearest_edge = self.plan_map.nearest_edge(Coord(x, y))
                    print('Nearest edge:', nearest_edge)

                #print event.pos
                #print(self.plan_map)

    def main_loop(self):
        pygame.key.set_repeat(500, 30)
        
        self.background = pygame.Surface(self.screen.get_size())
        self.background = self.background.convert()
        self.background.fill((0,0,0))

        while 1:
            self.handle_keys()
            self.draw()

if __name__ == "__main__":
    main_window = PlanarMapMain()
    main_window.main_loop()
