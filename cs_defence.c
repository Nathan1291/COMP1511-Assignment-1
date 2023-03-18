// Tower Defense
//
// This program was written by [Nathan Lue] (z5477755)
// on [16/03/23]
//
// TODO: Creates as simulation of a tower defense game

#include <stdio.h>

#define MAP_ROWS 6
#define MAP_COLS 12

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum land_type {
    GRASS,
    WATER,
    PATH_START,
    PATH_END,
    PATH_UP,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity {
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER,
};

struct tile {
    enum land_type land;
    enum entity entity;

    int n_enemies;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your function prototypes here

void create_enemies (struct tile map[MAP_ROWS][MAP_COLS], int row_start, int col_start);

int valid_position(int lake_row, int lake_col, int lake_height, int lake_width);

void create_lake();

void create_path();

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialise_map(struct tile map[MAP_ROWS][MAP_COLS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLS], int lives, int money);
void print_tile(struct tile tile, int entity_print);

int main(void) {
    // This `map` variable is a 2D array of `struct tile`s.
    // It is `MAP_ROWS` x `MAP_COLS` in size (which is 6x12 for this
    // assignment!)
    struct tile map[MAP_ROWS][MAP_COLS];

    // This will initialise all tiles in the map to have GRASS land and EMPTY
    // entity values.
    initialise_map(map);
    
    // Stage 1.1 - Scan in starting lives and money
    int starting_lives;
    int starting_money;

    printf("Starting Lives: ");
    scanf(" %d", &starting_lives);

    printf("Starting Money($): ");
    scanf(" %d", &starting_money);

    // Stage 1.1 scan and load in starting and ending points
    int row_start;
    int col_start;

    printf("Start Point: ");
    scanf("%d %d", &row_start, &col_start);

    int row_end;
    int col_end;

    printf("End Point: ");
    scanf("%d %d", &row_end, &col_end);

    map[row_start][col_start].land = PATH_START;
    map[row_end][col_end].land = PATH_END;

    // Stage 1.1 Printing the map after loading in start/end points
    // Stage 1.1 as well as the starting lives and money
    print_map(map, starting_lives, starting_money);

    // Stage 1.2 Scan and load in the initial enemies
    create_enemies(map, row_start, col_start);

    // Stage 1.2 Printing the map after loading in the enemies
    print_map(map, starting_lives, starting_money);

    // Stage 1.3 Creating the Lake
    create_lake(map);

    // Stage 1.3 Printing the map
    print_map(map, starting_lives, starting_money);

    // Stage 2.1 creating a path
    // Placing inputs into an array to suit the styling of the assignment (>80 char lines)
    int positions[4] = {row_start, col_start, row_end, col_end};
    create_path(map, positions);

    // Stage 2.1 Printing the map
    print_map(map, starting_lives, starting_money);


}





////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void create_enemies (struct tile map[MAP_ROWS][MAP_COLS], int row_start, int col_start) {
    // Stage 1.2
    // Scan in the initial enemies
    int num_enemies;
    printf("Initial Enemies: ");
    scanf("%d", &num_enemies);

    // Load in the n number of initial enemies on the starting tiles
    // Don't change starting point if the number of enemies isn't positive
    if (num_enemies > 0) {
        map[row_start][col_start].entity = ENEMY;
        map[row_start][col_start].n_enemies = num_enemies;
    }
}

int valid_position(int lake_row, int lake_col, int lake_height, int lake_width) {
    // Stage 1.3
    // This is a function used in the function create_lake
    // It checks if the lake is well within the bounds
    int max_row = lake_row + lake_height;
    int max_col = lake_col + lake_width;
    return lake_row < 0 || max_row > MAP_ROWS || lake_col < 0 || max_col > MAP_COLS;
}


void create_lake(struct tile map[MAP_ROWS][MAP_COLS]) {
    // Stage 1.3
    // intialising variables
    int lake_row;
    int lake_col;
    int lake_height;
    int lake_width;

    // Scanning in the values for the lake
    printf("Enter Lake: ");
    scanf("%d %d %d %d", &lake_row, &lake_col, &lake_height, &lake_width);

    // Checking if the lake is out of bounds
    if (valid_position(lake_row, lake_col, lake_height, lake_width)) {
        printf("Error: Lake out of bounds, ignoring...\n");
    }

    // Changing the values to water if the lake is inside the boundaries
    else {
        for (int i = 0; i < lake_height; i++) {
            for (int j = 0; j < lake_width; j++) {
                map[lake_row+i][lake_col+j].land = WATER;
            }
        }
    }
}

void create_path(struct tile map[MAP_ROWS][MAP_COLS], int positions[4]) {
    // Stage 2.1
    // positions = {row_start, col_start, row_end, col_end}

    // Printing message in preparation to take input
    printf("Enter Path: ");

    // keeping a tacker of the current row
    int curr_row = positions[0];
    int curr_col = positions[1];

    // Renaming variables for better readability
    int row_end = positions[2];
    int col_end = positions[3];
    // Creates a variable to store the command
    char path_command;

    // Checks if the current position is the same as the end square, terminates if it is
    int pos_tracker = 1;

    while (pos_tracker) {
        // Checking if the current position is within the boundaries of the field
        if (curr_row == row_end && curr_col == col_end) {
            pos_tracker = 0;
        }
        else {
            // Changing the tile land value depending on the inptu
            scanf(" %c", &path_command);
            if (path_command == 'r') {
                map[curr_row][curr_col].land = PATH_RIGHT;
                curr_col++;
            }
            else if (path_command == 'l') {
                map[curr_row][curr_col].land = PATH_LEFT;
                curr_col--;
            }
            else if (path_command == 'd') {
                map[curr_row][curr_col].land = PATH_DOWN;
                curr_row++;
            }
            else {
                map[curr_row][curr_col].land = PATH_UP;
                curr_row--;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Initialises map tiles to contain GRASS land and EMPTY entity.
 * 
 * Parameters:
 *     map - The map to initialise.
 * Returns:
 *     Nothing.
 */
void initialise_map(struct tile map[MAP_ROWS][MAP_COLS]) {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            map[row][col].land = GRASS;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        }
    }
}

/**
 * Prints all map tiles based on their value, with a header displaying lives
 * and money.
 * 
 * Parameters:
 *     map   - The map to print tiles from.
 *     lives - The number of lives to print with the map.
 *     money - The amount of money to print with the map.
 * Returns:
 *     Nothing.
 */
void print_map(struct tile map[MAP_ROWS][MAP_COLS], int lives, int money) {
    printf("\nLives: %d Money: $%d\n", lives, money);
    for (int row = 0; row < MAP_ROWS * 2; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            print_tile(map[row / 2][col], row % 2);
        }
        printf("\n");
    }
}

/**
 * Prints either the land or entity component of a single tile, based on
 * the `land_print` parameter;
 * 
 * Parameters:
 *     tile         - The tile to print the land/entity from
 *     land_print - Whether to print the land part of the tile or the entity
 *         part of the tile. If this value is 0, it prints the land, otherwise
 *         it prints the entity.
 * Returns:
 *     Nothing.
 */
void print_tile(struct tile tile, int land_print) {
    if (land_print) {
        if (tile.land == GRASS) {
            printf(" . ");
        } else if (tile.land == WATER) {
            printf(" ~ ");
        } else if (tile.land == PATH_START) {
            printf(" S ");
        } else if (tile.land == PATH_END) {
            printf(" E ");
        } else if (tile.land == PATH_UP) {
            printf(" ^ ");
        } else if (tile.land == PATH_RIGHT) {
            printf(" > ");
        } else if (tile.land == PATH_DOWN) {
            printf(" v ");
        } else if (tile.land == PATH_LEFT) {
            printf(" < ");
        } else if (tile.land == TELEPORTER) {
            printf("( )");
        } else {
            printf(" ? ");
        }
    } else {
        if (tile.entity == EMPTY) {
            printf("   ");
        } else if (tile.entity == ENEMY) {
            printf("%03d", tile.n_enemies);
        } else if (tile.entity == BASIC_TOWER) {
            printf("[B]");
        } else if (tile.entity == POWER_TOWER) {
            printf("[P]");
        } else if (tile.entity == FORTIFIED_TOWER) {
            printf("[F]");
        } else {
            printf(" ? ");
        }
    }
}
