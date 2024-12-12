#include <libinput.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct plot {
    char plant;
    uint32_t plot_id;
    struct {
        bool top;
        bool right;
        bool bottom;
        bool left;
    } fence;
} Plot;

typedef struct garden {
    Plot **plots;
    size_t w;
    size_t h;
} Garden;

typedef struct region {
    size_t area;
    size_t perimeter;
} Region;

static uint32_t garden_plot_id = 1;

static bool check_fence(Garden *garden, size_t dx, size_t dy, char plant)
{
    if (dx < 0 || dx >= garden->w || dy < 0 || dy >= garden->h) {
        return true;
    }

    Plot *plot = &garden->plots[dy][dx];
    if (plot->plant != plant) {
        return true;
    }

    return false;
}

static void plot_region(Region *region, Garden *garden, int x, int y, char plant)
{
    /* Off grid */
    if (x < 0 || x >= garden->w || y < 0 || y >= garden->h) {
        return;
    }

    Plot *plot = &garden->plots[y][x];


    /* Already visited? */
    if (plot->plot_id > 0) {
        return;
    }

    /* Is it the right plant? */
    if (garden->plots[y][x].plant != plant) {
        return;
    }

    /* Mark as visited */
    plot->plot_id = garden_plot_id;
    region->area++;

    if (check_fence(garden, x - 1, y, plant)) {
        plot->fence.left = true;
        region->perimeter++;
    }
    if (check_fence(garden, x + 1, y, plant)) {
        plot->fence.right = true;
        region->perimeter++;
    }
    if (check_fence(garden, x, y - 1, plant)) {
        plot->fence.top = true;
        region->perimeter++;
    }
    if (check_fence(garden, x, y + 1, plant)) {
        plot->fence.bottom = true;
        region->perimeter++;
    }

    plot_region(region, garden, x - 1, y, plant);
    plot_region(region, garden, x + 1, y, plant);
    plot_region(region, garden, x, y - 1, plant);
    plot_region(region, garden, x, y + 1, plant);
}

static uint32_t count_region_corners(Garden *garden, uint32_t plot_id)
{
    uint32_t corners = 0;
    for (size_t i = 0; i < garden->h; i++) {
        for (size_t j = 0; j < garden->w; j++) {
            Plot *plot = &garden->plots[i][j];
            if (plot->plot_id != plot_id) {
                continue;
            }

            /* Check the convex corners */
            if (plot->fence.top) {
                if (plot->fence.left) {
                    corners++;
                }
                if (plot->fence.right) {
                    corners++;
                }
            }
            if (plot->fence.bottom) {
                if (plot->fence.left) {
                    corners++;
                }
                if (plot->fence.right) {
                    corners++;
                }
            }

            /* Check the concave corners */
            if (!plot->fence.top) {
                if (!plot->fence.left) {
                    if (garden->plots[i - 1][j - 1].plot_id != plot_id) {
                        corners++;
                    }
                }
                if (!plot->fence.right) {
                    if (garden->plots[i - 1][j + 1].plot_id != plot_id) {
                        corners++;
                    }
                }
            }
            if (!plot->fence.bottom) {
                if (!plot->fence.left) {
                    if (garden->plots[i + 1][j - 1].plot_id != plot_id) {
                        corners++;
                    }
                }
                if (!plot->fence.right) {
                    if (garden->plots[i + 1][j + 1].plot_id != plot_id) {
                        corners++;
                    }
                }
            }
        }
    }
    return corners;
}

static void solve(Garden *garden)
{
    uint32_t total_cost = 0;
    uint32_t bulk_cost = 0;
    for (size_t i = 0; i < garden->h; i++) {
        for (size_t j = 0; j < garden->w; j++) {
            Plot *plot = &garden->plots[i][j];
            if (plot->plot_id > 0) {
                continue;
            }
            Region region = {0, 0};
            plot_region(&region, garden, j, i, plot->plant);
            total_cost += region.area * region.perimeter;
            bulk_cost += count_region_corners(garden, garden_plot_id) * region.area;
            garden_plot_id++;
        }
    }
    printf("Part 1: %u\n", total_cost);
    printf("Part 2: %u\n", bulk_cost);
}

int main(int argc, char **argv)
{
    Lines *lines;
    Garden garden;

    lines = lines_read_file("input.txt");

    garden.plots = malloc(sizeof(Plot *) * lines->size);
    for (int i = 0; i < lines->size; i++) {
        garden.plots[i] = calloc(lines->length, sizeof(Plot));
        for (int j = 0; j < lines->length; j++) {
            garden.plots[i][j].plant = lines->lines[i][j];
        }
    }
    garden.w = lines->length;
    garden.h = lines->size;
    lines_free(lines);

    solve(&garden);

    /* Free memory */
    for (int i = 0; i < garden.h; i++) {
        free(garden.plots[i]);
    }
    free(garden.plots);

    return 0;
}
