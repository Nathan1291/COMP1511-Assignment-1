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

void create_enemies (struct tile map[MAP_ROWS][MAP_COLS], 
                     int row_start, 
                     int col_start, 
                     int num_enemies);

int valid_position(int lake_row, int lake_col, int lake_height, int lake_width);

void create_lake(struct tile map[MAP_ROWS][MAP_COLS]);

void create_path(struct tile map[MAP_ROWS][MAP_COLS], 
                 int positions[4], 
                 int path[MAP_ROWS * MAP_COLS][2],
                 int *path_length_ptr);

int within_bounds(int row, int col);

int valid_tower_position(struct tile map[MAP_ROWS][MAP_COLS], 
                         int tower_row, 
                         int tower_col);

int spawn_tower(struct tile map[MAP_ROWS][MAP_COLS], int starting_money);

int move_enemies(struct tile map[MAP_ROWS][MAP_COLS], 
                  int path[MAP_ROWS * MAP_COLS][2],
                  int path_length);

int has_tower(struct tile map[MAP_ROWS][MAP_COLS], int tower_row, int tower_col);

int is_fortified(struct tile map[MAP_ROWS][MAP_COLS], int tower_row, int tower_col);

int enough_money(struct tile map[MAP_ROWS][MAP_COLS], int tower_row, int tower_col, int *starting_money);

void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLS], int *starting_money);

int attack_basic_and_powers(struct tile map[MAP_ROWS][MAP_COLS], 
                         int curr_row, 
                         int curr_col,
                         int num_attack);

int attack_fortified(struct tile map[MAP_ROWS][MAP_COLS], 
                         int curr_row, 
                         int curr_col,
                         int num_attack);

void attack(struct tile map[MAP_ROWS][MAP_COLS], 
            int path[MAP_ROWS * MAP_COLS][2],
            int *path_length_ptr,
            int *starting_money);

void remove_towers(struct tile map[MAP_ROWS][MAP_COLS], int row, int col);


void reduce_offset(int *row_offset, int *col_offset, int row_spacing, int col_spacing);

void rain(struct tile map[MAP_ROWS][MAP_COLS]);

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialise_map(struct tile map[MAP_ROWS][MAP_COLS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLS], int lives, int money);
void print_tile(struct tile tile, int entity_print);

int main(int argc, char *argv[]) {
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

    // Stage 1.1 scanning in starting and ending points
    int row_start;
    int col_start;

    printf("Start Point: ");
    scanf("%d %d", &row_start, &col_start);

    int row_end;
    int col_end;

    printf("End Point: ");
    scanf("%d %d", &row_end, &col_end);

    // Stage 1.1 Loading in starting and ending points
    map[row_start][col_start].land = PATH_START;
    map[row_end][col_end].land = PATH_END;

    // Stage 1.1 Printing the map after loading in start/end points
    // Stage 1.1 as well as the starting lives and money
    print_map(map, starting_lives, starting_money);

    // Stage 1.2 Scan in the initial enemies
    int num_enemies;
    printf("Initial Enemies: ");
    scanf("%d", &num_enemies);

    // Setting original n_enemies to be 0 so that create_enemies is reusable
    map[row_start][col_start].n_enemies = 0;

    // Stage 1.2 Load in the initial enemies
    create_enemies(map, row_start, col_start, num_enemies);

    // Stage 1.2 Printing the map after loading in the enemies
    print_map(map, starting_lives, starting_money);

    // Stage 1.3 Creating the Lake
    create_lake(map);

    // Stage 1.3 Printing the map
    print_map(map, starting_lives, starting_money);

    // Stage 2.1 creating a path
    // Placing inputs into an array to suit the styling of the assignment (>80 char lines)
    int positions[4] = {row_start, col_start, row_end, col_end};

    // Initialising variables to store path values for ease of use
    // in further stages
    // path stores all path values
    int path[MAP_ROWS * MAP_COLS][2];
    int path_length = 0;
    create_path(map, positions, path,  &path_length);

    // Stage 2.1 Printing the map
    print_map(map, starting_lives, starting_money);

    // Stage 2.2 Command Loop
    char command;
    printf("Enter Command: ");

    // Checking if command loop is interrupted
    while (scanf(" %c", &command) != EOF) {
        // Stage 2.2 Spawning in enemies Command
        if (command == 'e') {
            scanf("%d", &num_enemies);
            create_enemies(map, row_start, col_start, num_enemies);
            print_map(map, starting_lives, starting_money);
        }

        // Stage 2.3 Spawning in towers
        if (command == 't') {
            starting_money = spawn_tower(map, starting_money);
            print_map(map, starting_lives, starting_money);
        }

        // Stage 3.1 Moving Enemies
        if (command == 'm') {
            int lives_lost = move_enemies(map, path, path_length);
            printf("%d enemies reached the end!\n", lives_lost);
            starting_lives -= lives_lost;
            print_map(map, starting_lives, starting_money);

            if (starting_lives <= 0) {
                printf("Oh no, you ran out of lives!");
                break;
            }
        }

        // Stage 3.2 Upgrade Tower
        if (command == 'u') {
            upgrade_tower(map, &starting_money);
            print_map(map, starting_lives, starting_money);
        }

        // Stage 3.2 Tower Attacks
        if (command == 'a') {
            attack(map, path,  &path_length, &starting_money);
            print_map(map, starting_lives, starting_money);
        }

        if (command == 'r') {
            rain(map);
            print_map(map, starting_lives, starting_money);
        }

        printf("Enter Command: ");
    }


    printf("\nGame Over!\n");
}





////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Stage 1.2
void create_enemies (struct tile map[MAP_ROWS][MAP_COLS], 
                     int row_start, 
                     int col_start, 
                     int num_enemies) {
    // Load in the n number of initial enemies on the starting tiles
    // Don't change starting point if the number of enemies isn't positive
    if (num_enemies > 0) {
        map[row_start][col_start].entity = ENEMY;
        map[row_start][col_start].n_enemies += num_enemies;
    }
}

// Stage 1.3
int valid_lake_position(int lake_row, int lake_col, int lake_height, int lake_width) {
    // Stage 1.3
    // This is a function used in the function create_lake
    // It checks if the lake is well within the bounds
    int max_row = lake_row + lake_height;
    int max_col = lake_col + lake_width;
    return lake_row < 0 || max_row > MAP_ROWS || lake_col < 0 || max_col > MAP_COLS;
}

// Stage 1.3
void create_lake(struct tile map[MAP_ROWS][MAP_COLS]) {
    // intialising variables
    int lake_row;
    int lake_col;
    int lake_height;
    int lake_width;

    // Scanning in the values for the lake
    printf("Enter Lake: ");
    scanf("%d %d %d %d", &lake_row, &lake_col, &lake_height, &lake_width);

    // Checking if the lake is out of bounds
    if (valid_lake_position(lake_row, lake_col, lake_height, lake_width)) {
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

// Stage 2.1
void create_path(struct tile map[MAP_ROWS][MAP_COLS], 
                 int positions[4], 
                 int path[MAP_ROWS * MAP_COLS][2],
                 int *path_length_ptr) {
    // positions = {row_start, col_start, row_end, col_end}
    // initialising variable for later use
    int path_length_value;

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
            
            // Storing the path values as we go for future use
            *path_length_ptr += 1;
            path_length_value = *path_length_ptr;
            path[path_length_value - 1][0] = curr_row;
            path[path_length_value - 1][1] = curr_col;

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

// Stage 2.3
int within_bounds(int row, int col) {
    // used in valid_tower_position to check if its within bounds
    return row < 0 || row > MAP_ROWS || col < 0 || col > MAP_COLS;
}

// Stage 2.3
int valid_tower_position(struct tile map[MAP_ROWS][MAP_COLS], 
                         int tower_row, 
                         int tower_col) {
    // Stage 2.3
    // variable which tracks if it is a valid tower position or not
    int valid_pos = 1;
    // Checking if the position is within the bounds
    if (within_bounds(tower_row, tower_col)) {
        valid_pos = 0;
    }
    // Checking if the land on the tile is grass
    else if (map[tower_row][tower_col].land != GRASS) {
        valid_pos = 0;
    }
    // Checking if the entity on the land isnt occupied
    else if (map[tower_row][tower_col].entity != EMPTY) {
        valid_pos = 0;
    }
    // Returns true if it passes all the tests
    return valid_pos;
}

// Stage 2.3, returns starting money
int spawn_tower(struct tile map[MAP_ROWS][MAP_COLS], int starting_money) {
    int tower_row;
    int tower_col;
    scanf("%d %d", &tower_row, &tower_col);
    // Check if there is enough money and if it is a valid position, then add it
    if (valid_tower_position(map, tower_row, tower_col) && starting_money > 200) {
        map[tower_row][tower_col].entity = BASIC_TOWER;
        starting_money -= 200;
        printf("Tower successfully created!\n");
    }
    // Message in case there isn't enough money or is an invalid position
    else {
        printf("Error: Tower creation unsuccessful. ");
        printf("Make sure you have at least $200 and that the ");
        printf("tower is placed on a grass block with no entity.\n");
    }
    return starting_money;
}

// Stage 3.1, returns the number of lives lost
int move_enemies(struct tile map[MAP_ROWS][MAP_COLS], 
                  int path[MAP_ROWS * MAP_COLS][2],
                  int path_length) {
    int num_move;
    scanf("%d", &num_move);

    // Storing the number of lives lost
    int num_lives_lost = 0;

    // Going through each path tile starting from the end
    for (int i = path_length-1; i >= 0; i--) {
        int curr_row = path[i][0];
        int curr_col = path[i][1];

        // Checking if there is an enemy on the tile
        if (map[curr_row][curr_col].entity == ENEMY) {
            // finding the number of enemy in the tile
            int num_enemies = map[curr_row][curr_col].n_enemies;

            // Checking if the move will make the enemy reach the end
            if (i + num_move >= path_length) {
                num_lives_lost += num_enemies;
                map[curr_row][curr_col].entity = EMPTY;
                map[curr_row][curr_col].n_enemies = 0;
            }

            // Moving the enemies if they do not reach the end
            else {
                map[path[i+num_move][0]][path[i+num_move][1]].entity = ENEMY;
                map[path[i+num_move][0]][path[i+num_move][1]].n_enemies = num_enemies;

                map[curr_row][curr_col].entity = EMPTY;
                map[curr_row][curr_col].n_enemies = 0;
            }
        }
    }  
    return num_lives_lost;
}

// Stage 3.2, checks if there is a tower in the given tile
int has_tower(struct tile map[MAP_ROWS][MAP_COLS], int tower_row, int tower_col) {
    int val = 0;
    if (map[tower_row][tower_col].entity == BASIC_TOWER) {
        val = 1;   
    }
    else if (map[tower_row][tower_col].entity == POWER_TOWER) {
        val = 1;   
    }
    else if (map[tower_row][tower_col].entity == FORTIFIED_TOWER) {
        val = 1;   
    }
    return val;
}

// Stage 3.2, Checks if there is a fortified tower in the given tile
int is_fortified(struct tile map[MAP_ROWS][MAP_COLS], int tower_row, int tower_col) {
    int val = 0;
    if (map[tower_row][tower_col].entity == FORTIFIED_TOWER) {
        val = 1;
    }
    return val;
}

// Stage 3.2, Checks if there is enough money for the upgrade
int enough_money(struct tile map[MAP_ROWS][MAP_COLS], 
                 int tower_row, 
                 int tower_col, 
                 int *starting_money) {
    int val = 1;
    int money = *starting_money;
    if (map[tower_row][tower_col].entity == BASIC_TOWER) {
        if (money < 300) {
            val = 0;
        }
    }
    else if (map[tower_row][tower_col].entity == POWER_TOWER) {
        if (money < 500) {
            val = 0;
        }
    }
    return val;
}

// Stage 3.2, upgrades the tower if the conditions are correct
// else it outputs error messages
void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLS], int *starting_money) {
    int tower_row;
    int tower_col;
    scanf("%d %d", &tower_row, &tower_col);

    if (within_bounds(tower_row, tower_col)) {
        printf("Error: Upgrade target is out-of-bounds.\n");
    }
    else if (!has_tower(map, tower_row, tower_col)) {
        printf("Error: Upgrade target contains no tower entity.\n");
    }
    else if (is_fortified(map, tower_row, tower_col)) {
        printf("Error: Tower cannot be upgraded further.\n");
    }
    else if (!enough_money(map, tower_row, tower_col, starting_money)) {
        printf("Error: Insufficient Funds.\n");
    }
    else {
        if (map[tower_row][tower_col].entity == BASIC_TOWER) {
            map[tower_row][tower_col].entity = POWER_TOWER;

            *starting_money -= 300;
        }
        else if (map[tower_row][tower_col].entity == POWER_TOWER) {
            map[tower_row][tower_col].entity = FORTIFIED_TOWER;

            *starting_money -= 500;
        }
        printf("Upgrade Successful!\n");
    }
}


// Stage 3.3, makes the towers attack, made due to nesting error
int tower_attack(struct tile map[MAP_ROWS][MAP_COLS], 
                 int row,
                 int col,
                 int num_attack,
                 int power) {
    int num_destroyed;
    // checks if the remaining enemies are larger than the attack power
    if (map[row][col].n_enemies > num_attack * power) {
        map[row][col].n_enemies -= num_attack * power;
        num_destroyed = num_attack * power;
    }
    else {
        num_destroyed = map[row][col].n_enemies;
        map[row][col].n_enemies = 0;
    }
    return num_destroyed;
}

// Stage 3.3, calculates the attack for basic and power towers
int attack_basic_and_powers(struct tile map[MAP_ROWS][MAP_COLS], 
                         int curr_row, 
                         int curr_col, 
                         int num_attack) {
    int num_destroyed = 0;                        
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (within_bounds(curr_row + i, curr_col + j)) {
                continue;
            }
            else if (map[curr_row+i][curr_col+j].entity == BASIC_TOWER) {
                num_destroyed += tower_attack(map, curr_row, curr_col, num_attack, 1);
            } 
            else if (map[curr_row+i][curr_col+j].entity == POWER_TOWER) {
                num_destroyed += tower_attack(map, curr_row, curr_col, num_attack, 2);

            }
            // makes sure to return tiles to normal after enemies have died
            if (map[curr_row][curr_col].n_enemies <= 0) {
                map[curr_row][curr_col].n_enemies = 0;
                map[curr_row][curr_col].entity = EMPTY;
            }
        }
    }
    return num_destroyed;
}

// Stage 3.3, calculates the attack for fortified towers
int attack_fortified(struct tile map[MAP_ROWS][MAP_COLS], 
                         int curr_row, 
                         int curr_col,
                         int num_attack) {
    int num_destroyed = 0;                        
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (within_bounds(curr_row + i, curr_col + j)) {
                continue;
            }
            else if (map[curr_row+i][curr_col+j].entity == FORTIFIED_TOWER) {
                num_destroyed += tower_attack(map, curr_row, curr_col, num_attack, 3);
            }
            // makes sure to return tiles to normal after enemies have died
            if (map[curr_row][curr_col].n_enemies <= 0) {
                map[curr_row][curr_col].n_enemies = 0;
                map[curr_row][curr_col].entity = EMPTY;
            }
        }
    }
    return num_destroyed;
}


// Stage 3.3, main attack function
void attack(struct tile map[MAP_ROWS][MAP_COLS], 
            int path[MAP_ROWS * MAP_COLS][2],
            int *path_length_ptr,
            int *starting_money) {
    // Check enemies for towers within radius 1 from enemies for basic and powers
    // Check enemies for towers within radius 2 from enemies for fortified

    int path_length = *path_length_ptr;
    int num_attack;
    scanf("%d", &num_attack);

    int destroyed = 0;
    for (int i = path_length-1; i >= 0; i--) {
        int curr_row = path[i][0];
        int curr_col = path[i][1];
        // Checking if there is an enemy on the tile
        if (map[curr_row][curr_col].entity == ENEMY) {
            // Checks the surrounding radius of 1 for basic and power tower

            destroyed += attack_basic_and_powers(map, curr_row, curr_col, num_attack);
            // Checks the surrounding raidius of 2 for fortified towers
            destroyed += attack_fortified(map, curr_row, curr_col, num_attack);
        }    
    }
    *starting_money += destroyed * 5;
    printf("%d enemies destroyed!\n", destroyed);
}

// Remove the tower from the tile if the rain lands on the tile
void remove_towers(struct tile map[MAP_ROWS][MAP_COLS], int row, int col) {
    if (map[row][col].entity == BASIC_TOWER) {
        map[row][col].entity = EMPTY;
    }
    if (map[row][col].entity == POWER_TOWER) {
        map[row][col].entity = EMPTY;
    }
}

// Clean the offset values so that they are easy to use
// turns negative and large values to values that are within the map
void reduce_offset(int *row_offset, int *col_offset, int row_spacing, int col_spacing) {
    int r_offset = *row_offset;
    int c_offset = * col_offset;

    r_offset %= row_spacing;
    c_offset %= col_spacing;

    if (r_offset < 0) {
        r_offset += row_spacing;
    }
    if (c_offset < 0) {
        c_offset += col_spacing;
    }

    *row_offset = r_offset;
    *col_offset = c_offset;
}

// Stage 3.4, upgrades the tower if the conditions are correct
void rain(struct tile map[MAP_ROWS][MAP_COLS]) {
    int row_spacing;
    int col_spacing;
    int row_offset;
    int col_offset;

    scanf("%d %d %d %d", &row_spacing, &col_spacing, &row_offset, &col_offset);

    reduce_offset(&row_offset, &col_offset, row_spacing, col_spacing);

    int curr_row = row_offset;
    int curr_col;

    // Double nested for loop to go through each tile where there will be rain
    // checks if it will spawn water, then despawn a tower if it does
    while (curr_row < MAP_ROWS) {
        curr_col = col_offset;
        while (curr_col < MAP_COLS) {
            if (map[curr_row][curr_col].land == GRASS) {
                map[curr_row][curr_col].land = WATER;
                remove_towers(map, curr_row, curr_col);
            }
            curr_col += col_spacing;
        }
        curr_row += row_spacing;
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