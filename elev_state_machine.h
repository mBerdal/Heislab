
typedef enum {
    IDLE,                      //elevator awaits orders/destination
    EXCECUTE,                  //excecute order/go to destination
    IN_TRANSIT,                //elevator is inbetween orders/destination
    AT_DESTINATION,            //elevator has reached its destination
    AT_ORDER,                  //elevator has reached an order
    INITIALIZE,                //initializing elevator variables and moves it to defined position
    RUN_TESTS                  //run tests to see if elevator has reached an order/destination
}STATUS;


struct Elev{
    STATUS status;
    int current_dir;
    int current_floor;
    int orders[N_FLOORS][3];
    bool stopped_at_intermediate;
};

//orders[N_FLOORS][3] contains following information:
//orders[n][0] = 1 if panel button up has been pushed at floor n. Otherswise 0
//orders[n][1] = 1 if panel button down has been pushed at floor n. Otherswise 0
//orders[n][2] = m if elevatorbutton m has been pushed when elevator is at floor n. Otherwise -1

struct Elev state_machine(struct Elev elevator);

