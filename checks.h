#include "elev.h"
#include "utilities.h"

bool is_at_order(int matrix[N_FLOORS][3], int current_floor);
bool is_at_destiantion(int matrix[N_FLOORS][3], int current_floor);
bool is_at_intermediate(int matrix[N_FLOORS][3], int current_floor, int current_dir);
bool check_orders(int matrix[N_FLOORS][3]);
