import pygame
import numpy as np

WIDTH, HEIGHT = 800, 800
ROWS, COLS = 80, 80
CELL_SIZE = WIDTH // COLS
grid = np.zeros((ROWS, COLS))

pygame.init()
screen = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()
running = True
gaming = False

def update_grid(grid):
    new_grid = grid.copy()
    for row in range(ROWS):
        for col in range(COLS):
            live_neighbors = np.sum(grid[row-1:row+2, col-1:col+2]) - grid[row, col]
            if grid[row, col] == 1:
                if live_neighbors < 2 or live_neighbors > 3:
                    new_grid[row, col] = 0
            else:
                if live_neighbors == 3:
                    new_grid[row, col] = 1
    return new_grid

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                gaming = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            row, col = y // CELL_SIZE, x // CELL_SIZE
            if (grid[row, col] == 1):
                grid[row, col] = 0
            else:
                grid[row, col] = 1

    if gaming:
        grid = update_grid(grid)

    screen.fill((0, 0, 0))
    for row in range(ROWS):
        for col in range(COLS):
            color = (255, 255, 255) if grid[row, col] == 1 else (0, 0, 0)
            pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))
    pygame.display.flip()
    clock.tick(10)

pygame.quit()
