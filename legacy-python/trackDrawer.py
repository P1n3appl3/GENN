import sys
import pygame


def main():
    pygame.init()
    screenSize = screenWidth, screenHeight = 1200, 900
    screen = pygame.display.set_mode(screenSize)
    outside = []
    inside = []
    track = []
    state = 0
    print "Use LMB to draw the outside then inside then center line"
    print "Use RMB to delete points and any key to advance to the next part of the track"
    while state < 3:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    if state == 2:
                        track.append(event.pos)
                    elif state == 0:
                        outside.append(event.pos)
                    elif state == 1:
                        inside.append(event.pos)
                elif event.button == 3:
                    if state == 2:
                        del(track[-1])
                    elif state == 0:
                        del(outside[-1])
                    elif state == 1:
                        del(inside[-1])
            elif event.type == pygame.KEYDOWN:
                state += 1

        # drawing
        screen.fill((100, 100, 100))
        if len(track) > 1:
            pygame.draw.lines(screen, (255, 0, 0), state > 2, track)
        if len(outside) > 1:
            pygame.draw.lines(screen, (0, 255, 0), state > 0, outside)
        if len(inside) > 1:
            pygame.draw.lines(screen, (0, 255, 0), state > 1, inside)
        pygame.display.flip()
    #fileName = raw_input("Input track name: ")
    fileName = "track"
    f = open(fileName + ".txt", "w")
    f.write(str(outside).strip("[]"))
    f.write("\n")
    f.write(str(inside).strip("[]"))
    f.write("\n")
    f.write(str(track).strip("[]"))
    f.write("\n")
    f.close()
if __name__ == "__main__":
    main()
