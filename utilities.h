typedef int bool;
#define true 1
#define false 0

void start_timer();
int check_timer(double seconds);
int get_index(int current_dir);
int get_sign(int val);
void print_matrix(int matrix[N_FLOORS][3]);
int set_direction(int current_floor, int destination);