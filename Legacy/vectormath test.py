import sys
import pygame
import math


class Vector():
    x = 0
    y = 0

    def __init__(self, pos=(0, 0)):
        self.x = float(pos[0])
        self.y = float(pos[1])

    def magnitude(self):
        return math.sqrt(self.x**2 + self.y**2)

    def norm(self):
        m = self.magnitude
        return Vector((self.x / m, self.y / m))

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    def rotate(self, angle):
        return Vector((self.x * math.cos(angle) - self.y * math.sin(angle), self.x * math.sin(angle) + self.y * math.cos(angle)))

    def __add__(self, other):
        return Vector((self.x + other.x, self.y + other.y))

    def __sub__(self, other):
        return Vector((self.x - other.x, self.y - other.y))

    def __mul__(self, other):
        return Vector((self.x * other.x, self.y * other.y))

    def __div__(self, other):
        return Vector((self.x / other.x, self.y / other.y))

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def toTup(self):
        return (self.x, self.y)

    def toTupInt(self):
        return (int(self.x), int(self.y))


def main():
    pygame.init()
    screenSize = screenWidth, screenHeight = 640, 480
    screen = pygame.display.set_mode(screenSize)

    a = Vector((100, 300))
    b = Vector((578, 400))
    red = (255, 0, 0)
    w = 5
    h = 25

    while 1:
        screen.fill((100, 100, 100))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
        mouse = Vector((float(pygame.mouse.get_pos()[0]), float(
            pygame.mouse.get_pos()[1])))
        ab = b - a
        ac = mouse - a
        d = ab.dot(ac) / ab.dot(ab)
        final = a + ab * Vector((d, d))

        p = Vector((0, mouse.y))

        # drawing
        pygame.draw.line(screen, red, a.toTupInt(), b.toTupInt())
        pygame.draw.line(screen, red, final.toTupInt(), mouse.toTupInt())
        pygame.draw.rect(screen, (255, 255, 255), (p.x, p.y, w, h))
        pygame.display.flip()

if __name__ == "__main__":
    main()
