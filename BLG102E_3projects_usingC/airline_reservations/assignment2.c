#include <stdio.h>   //printf, scanf
#include <stdlib.h>  //EXIT_SUCCESS
#include <stdbool.h> //true, false
#include <math.h>    //pow, abs

#define MAX_ROWS 99
#define MAX_COLUMNS 20
#define MAX_AISLES 5
#define MIN_SCORE -1000000000

//Prints seat layout of the plane with the updated reservation
void print_layout(int, int, char (*)[MAX_COLUMNS], int, int[]);

//Sets all empty-seats scores to 0
void set_scores_to0(int, int, int, double (*)[MAX_COLUMNS]);

//Finds empty seat to determine its score
void find_empty_seat(int, int, int, char (*)[MAX_COLUMNS], double (*)[MAX_COLUMNS]);

//Calculates empty seats scores
void seat_scores(int, int, int, int, int, double (*)[MAX_COLUMNS], char (*)[MAX_COLUMNS]);

//Finds and reserves farthest empty seat score
void reserve_one_seat(int, int, int, double (*)[MAX_COLUMNS], char (*)[MAX_COLUMNS]);

//Calculates consecutive n empty seats' scores for family reservations
void family_reservations(int, int, int, char (*)[MAX_COLUMNS], int, int *, double (*)[MAX_COLUMNS], int[]);

//Finds farthest n empty seats scores
void largest_block(int, int, int, double (*)[MAX_COLUMNS], double (*)[MAX_COLUMNS], char (*)[MAX_COLUMNS], int);

//Reserves n empty seats
void reserve_n_seats(int, int, char (*)[MAX_COLUMNS], int, int);

//changes seats sign from reserved '+' to taken 'X'
void reserved_seats_layout(int, int, int, char (*)[MAX_COLUMNS]);

int main()
{
    int rows = 0;
    int columns = 0; //number of seats per row
    int aisle_count = 0;
    int aisle[MAX_AISLES] = {0};
    int passengers = 0;
    int max_reservations = 0;
    int nth_reservation = 0;
    char empty_seat = '-';
    char seat[MAX_ROWS][MAX_COLUMNS] = {{0}};
    double scores[MAX_ROWS][MAX_COLUMNS] = {{0}};

    printf("Number of rows: ");
    scanf("%d", &rows);
    printf("Number of seats per row: ");
    scanf("%d", &columns);
    printf("Number of aisles: ");
    scanf("%d", &aisle_count);

    max_reservations = rows * columns;

    //determines where each aisle will be in an array
    for (int i = 0; i < aisle_count; i++)
    {
        printf("Add aisle %d after seat: ", i + 1);
        scanf("%d", &aisle[i]);
    }

    printf("\n");

    //set all seats sign to empty seat '-'
    for (int i = 1; i < rows + 1; i++)
    {
        for (int j = 1; j < columns + aisle_count + 1; j++)
        {
            seat[i][j] = empty_seat;
        }
    }

    //print seats layout with the empty seats
    print_layout(rows, columns, seat, aisle_count, aisle);

    /**
     *  Prompts the user for a number of people and exits on the input of -1
     */
    do
    {
        printf("The number of people in the reservation\n (0: print current reservations, -1: exit): ");
        scanf("%d", &passengers);

        //counts the updated number of reservations
        if (passengers <= columns && nth_reservation <= max_reservations && passengers != -1)
        {
            nth_reservation += passengers;
        }

        //prints current seats layout
        if (passengers == 0)
        {
            printf("\n");
            print_layout(rows, columns, seat, aisle_count, aisle);
        }

        /**
         *  warns the user that there are no available seats for the given no. of passengers in the reservation
         *  if the no. of new reservations is greater than no. of seats per row
         *  or if all seats are taken
         */
        else if ((passengers > columns || nth_reservation > max_reservations) && passengers != -1)
        {
            printf("\nNo available seats for the requested reservation!\n");
            printf("\n");

            //Decrease the total number of reseervations if there are empty seats available
            if (abs(passengers - nth_reservation) < max_reservations && passengers <= columns)
            {
                nth_reservation -= passengers;
            }
            continue;
        }
        else if (passengers >= 1)
        {
            //single passenger reservation
            if (passengers == 1)
            {
                printf("Reserved seat: ");
                //empty seat scores are all 0 at first
                set_scores_to0(rows, columns, aisle_count, scores);
                //find every empty seat and calculate its score
                find_empty_seat(rows, columns, aisle_count, seat, scores);
                //reserve a seat with the largest score -> farthest seat from all taken seats
                reserve_one_seat(rows, columns, aisle_count, scores, seat);
                //print layout
                print_layout(rows, columns, seat, aisle_count, aisle);
                //change all reserved_seats to unempty_seats
                reserved_seats_layout(rows, columns, aisle_count, seat);
            }
            //family reservations
            else if (passengers > 1)
            {
                //empty seat scores are all 0 at first
                set_scores_to0(rows, columns, aisle_count, scores);
                //find every empty seat and calculate its score
                find_empty_seat(rows, columns, aisle_count, seat, scores);
                //find consecutive block of n seats, calculate its total score, if available reserve the block
                family_reservations(rows, columns, aisle_count, seat, passengers, &nth_reservation, scores, aisle);
                //change all reserved_seats to unempty_seats
                reserved_seats_layout(rows, columns, aisle_count, seat);
            }
        }

    } while (passengers != -1);

    return EXIT_SUCCESS;
}

/**
 *  Prints a 2D array that represents aircraft layout
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @param path_num is the number of aisles in the aircraft layout.
 *  @param path array indicates after which seat an aisle will be.
 *  @return no return- print letters on the seats of row 0, numbers on column 0, 
 *          | | for aisles, - for empty seats, X for reserved seats, and + for newly reserved seats.
 */
void print_layout(int rows, int cols, char (*seat)[MAX_COLUMNS], int path_num, int path[])
{
    char seat_letter = 'A';
    int seat_number = 1;
    char passage = '|';

    for (int i = 0; i < rows + 1; i++)
    {
        int k = 0;
        for (int j = 0; j < cols + path_num + 1; j++)
        {
            //seat of (0,0) has space characters
            if (i == 0 && j == 0)
            {
                printf("    ");
            }
            //prints an aisle "| |" after each seat given by the user
            else if (i >= 0 && j == path[k] + k + 1)
            {
                seat[i][j] = passage;
                printf("%c\t%c", passage, passage);
                k++;
            }
            //prints seat letters (A, B,..) in the columns of first row
            else if (i == 0 && j > 0 && j != path[j] + j + 1)
            {
                seat[i][j] = seat_letter++;
                printf(" %c ", seat[i][j]);
            }
            //prints seat numbers (1, 2,..) in the rows of first columns
            else if (j == 0 && i > 0) //
            {
                seat[i][j] = seat_number++;
                printf("%-4d", seat[i][j]);
            }
            //prints the sign of each seat in the aircraft -> empty, taken, newly reserved
            else
            {
                printf(" %c ", seat[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 *  Resets all seat scores to 0
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param scores array has the score of each seat.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @return no return- changes the score value of each seat
 */
void set_scores_to0(int rows, int cols, int paths, double (*scores)[MAX_COLUMNS])
{
    for (int i = 1; i < rows + 1; i++)
    {
        for (int j = 1; j < cols + paths + 1; j++)
        {
            scores[i][j] = 0;
        }
    }
}

/**
 *  Look for every empty seat -> '-'
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @param scores array has the score of each seat.
 *  @return no return- searches for an empty seat then goes to another function.
 */
void find_empty_seat(int rows, int cols, int paths, char (*seat)[MAX_COLUMNS], double (*scores)[MAX_COLUMNS])
{
    char empty_seat = '-';

    for (int i1 = 1; i1 < rows + 1; i1++)
    {
        for (int j1 = 1; j1 < cols + paths + 1; j1++)
        {
            if (seat[i1][j1] == empty_seat)
            {
                //when an empty seat is found, calculate the distance between it and every other taken seat
                seat_scores(i1, j1, rows, cols, paths, scores, seat);
            }
        }
    }
}

/**
 *  Calculates the score of the founded empty seat based on its distance from all taken seats
 *  
 *  @param i1 is the number that indicates the row number of the founded empty seat.
 *  @param j1 is the number that indicates the column number of the founded empty seat.
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param scores array has the score of each seat.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @return no return- computes the distances and adjusted distances and adds it to the score of the empty seat.
 */
void seat_scores(int i1, int j1, int rows, int cols, int paths, double (*scores)[MAX_COLUMNS], char (*seat)[MAX_COLUMNS])
{
    double dist = 0;
    double adjusted_dist = 0;
    char taken_seat = 'X';

    for (int i2 = 1; i2 < rows + 1; i2++)
    {
        for (int j2 = 1; j2 < cols + paths + 1; j2++)
        {
            if (seat[i2][j2] == taken_seat)
            {
                dist = (double)abs(i1 - i2) + (double)abs(j1 - j2) / 2; //distance formula
                adjusted_dist = 100 - (double)pow(100, 1 / dist);       //adjusted distance formula
                scores[i1][j1] += adjusted_dist;                        //sum of all adjuctsed distances is added to the score of the empty seat
            }
        }
    }
}

/**
 *  Reserves a seat with maximum score by representing it as '+' in the layout
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param scores array has the score of each seat.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @return no return- prints the seat letter and number of the empty seat with the maximum score.
 */
void reserve_one_seat(int rows, int cols, int paths, double (*scores)[MAX_COLUMNS], char (*seat)[MAX_COLUMNS])
{
    double maximum_score = MIN_SCORE;
    int largest_i = 0;
    int largest_j = 0;
    char reserved_seat = '+';

    for (int i = 1; i < rows + 1; i++)
    {
        for (int j = 1; j < cols + paths + 1; j++)
        {
            //updates the value of 'maximum_score'
            if (scores[i][j] > maximum_score && seat[i][j] == '-')
            {
                maximum_score = scores[i][j];
                largest_i = i;
                largest_j = j;
            }
            //if the two or more seats have the same score choose the one in the lower row
            else if (scores[i][j] == maximum_score && seat[i][j] == '-')
            {
                if (i < largest_i)
                {
                    largest_i = i;
                    largest_j = j;
                }
                //if both seats are in the same row choose the oe with in lower column
                else if (i == largest_i)
                {
                    if (j < largest_j)
                    {
                        largest_i = i;
                        largest_j = j;
                    }
                }
            }
        }
    }
    seat[largest_i][largest_j] = reserved_seat; //change the sign of the seat with max score into '+'
    printf("%d%c\n\n", seat[largest_i][0], seat[0][largest_j]);
}

/**
 *  Looks for consecutive block of n seats and calculates its total score while keeping a count of the number of aisles in that block
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @param passengers is the number of people the user has inputted for the new reservation.
 *  @param nth_res is the total number of reservations that has been done.
 *  @param scores array has the score of each seat.
 *  @param aisle array indicates after which seat an aisle will be.
 *  @return no return- if no consecutive block of n seats is found prints no availabe seats,
 *          but if a block is found, calls two other functions .
 */
void family_reservations(int rows, int cols, int paths, char (*seat)[MAX_COLUMNS], int passengers, int *nth_res, double (*scores)[MAX_COLUMNS], int aisle[])
{
    double block_jth_score[MAX_ROWS][MAX_COLUMNS] = {{0}}; //has total score of each block, block's index start with the first seat in the block.
    double aisles_in_block[MAX_ROWS][MAX_COLUMNS] = {{0}}; //has the number of aisles in each block of n seats.
    double seat_block_score = 0;
    int family_number = 0; //number of members in the reservations
    int space = 0;         //number of aisles in each block of n seats
    int seat_x = 0;        //count of empty seats in a row
    int nth_seat = 0;      //nth number of seat in the block
    char passage = '|';
    char empty_seat = '-';
    bool available_block = false;

    for (int i = 1; i < rows + 1; i++)
    {
        seat_x = 0;
        for (int j = 1; j < cols + paths + 1; j++)
        {
            nth_seat = 1;

            if (seat[i][j] == passage)
            {
                continue;
            }
            /**
             *  finds every empty seat and increments the number of empty seats in the ith row to find consecutive n seats
             *  if an empty seat is followed by a taken seat seat_x is reseted to 0
            */
            else if (seat[i][j] == empty_seat)
            {
                seat_x++;

                /**
                 * when the number of empty seats in the ith row is greater than or equal to the number of passengers in the reservations
                 * the block of n seats is found 
                 * computes the score of that block -> sum of the scores of every seat in that block
                */
                if (seat_x >= passengers)
                {
                    available_block = true;
                    seat_block_score = 0;
                    family_number = j;
                    space = 0;

                    while (nth_seat <= passengers)
                    {
                        if (seat[i][family_number] != passage)
                        {
                            seat_block_score += scores[i][family_number];
                            nth_seat++;
                            family_number--;
                        }
                        else
                        {
                            space++;
                            family_number--;
                        }
                    }

                    //each seat in the max-score-block has the max score and count of aisles in the block assigned to it
                    while (family_number < j)
                    {
                        block_jth_score[i][family_number + 1] = seat_block_score;
                        aisles_in_block[i][family_number + 1] = space;
                        family_number++;
                    }
                }
                else
                {
                    block_jth_score[i][j] = MIN_SCORE; //if the seat (i,j) is not in a block, it gets an irrelevant(very small) score
                }
            }
            else
            {
                seat_x = 0;
                block_jth_score[i][j] = MIN_SCORE; //if the seat (i,j) is not in a block, it gets an irrelevant(very small) score
            }
        }
    }
    if (available_block == false)
    {
        printf("\nNo available seats for the requested reservation!\n");
        printf("\n");
        *nth_res -= passengers; //decrement the total number of reservations if no consecutive block of n seats is found
    }
    else
    {
        //find the block with maximum score and reserve n seats
        largest_block(rows, cols, paths, block_jth_score, aisles_in_block, seat, passengers);
        //prints the current aircraft layout
        print_layout(rows, cols, seat, paths, aisle);
    }
}

/**
 *  Determines the block with the maximum score
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param block_jth_score array has total score of each block, block's index start with the first seat in the block.
 *  @param aisles_in_block array has the number of aisles in each block of n seats.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @param passengers is the number of people the user has inputted for the new reservation.
 *  @return no return- finds the i and j index of the first seat in the block with the maximum score and calls another function.
 */
void largest_block(int rows, int cols, int paths, double (*block_jth_score)[MAX_COLUMNS], double (*aisles_in_block)[MAX_COLUMNS], char (*seat)[MAX_COLUMNS], int passengers)
{
    double maximum_score = MIN_SCORE;
    int largest_i = 0;
    int largest_j = 0;
    int largest_block_aisle_number = -1;

    for (int i = 1; i < rows + 1; i++)
    {
        for (int j = 1; j < cols + paths + 1; j++)
        {
            //updates the value of 'maximum_score' and  to find the maximum score
            if (block_jth_score[i][j] > maximum_score && seat[i][j] == '-')
            {
                maximum_score = block_jth_score[i][j];
                largest_i = i;
                largest_j = j;
                largest_block_aisle_number = aisles_in_block[i][j];
            }
            //when two blocks have the same max score, choose the one with minimum number of aisles
            else if (block_jth_score[i][j] == maximum_score && seat[i][j] == '-')
            {
                if (aisles_in_block[i][j] < largest_block_aisle_number)
                {
                    largest_block_aisle_number = aisles_in_block[i][j];
                    largest_i = i;
                    largest_j = j;
                }
                //if both blocks have the same max score and aisles choose the one in the lower row
                else if (aisles_in_block[i][j] == largest_block_aisle_number)
                {
                    if (i < largest_i)
                    {
                        largest_i = i;
                        largest_j = j;
                    }
                    //if both blocks are in the same row, choose the one in the lower column
                    else if (i == largest_i)
                    {
                        if (j < largest_j)
                        {
                            largest_i = i;
                            largest_j = j;
                        }
                    }
                }
            }
        }
    }

    reserve_n_seats(passengers, largest_block_aisle_number, seat, largest_i, largest_j);
}

/**
 *  Reserves a block of n seats by changing each seat's sign into '+'
 *  
 *  @param passengers is the number of people the user has inputted for the new reservation.
 *  @param aisle_count is the number of aisles in the block that is to be reserved.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @param largest_i is the row number of first seat in the blcok with the max score.
 *  @param largest_j is the column number of first seat in the blcok with the max score.
 *  @return no return- prints the letters and numbers of all seats fn the block with the maximum score.
 */
void reserve_n_seats(int passengers, int aisle_count, char (*seat)[MAX_COLUMNS], int largest_i, int largest_j)
{
    char reserved_seat = '+';
    char passage = '|';
    int n = 0;

    printf("Reserved seat: ");

    for (int j = largest_j; j < largest_j + passengers + aisle_count; j++)
    {
        if (seat[largest_i][j] == passage)
        {
            continue;
        }
        else if (n < passengers)
        {
            seat[largest_i][j] = reserved_seat; //changes the sign of the seats in the block into '+'
            n++;
            printf("%d%c ", seat[largest_i][0], seat[0][j]);
        }
    }
    printf("\n\n");
}

/**
 *  Changes the sign of the newly reserved seat(s) from '+' (just reserved) to 'X' (taken)
 *  
 *  @param rows is the number of rows in the aircraft.
 *  @param cols is the number of seats per row in the aircraft.
 *  @param paths is the number of aisles in the aircraft  layout.
 *  @param seat array has the sign of each seat in the aircraft -> empty, taken, newly reserved.
 *  @return no return- changes the sign of the newly reserved seat(s) from '+' to 'X'.
 */
void reserved_seats_layout(int rows, int cols, int paths, char (*seat)[MAX_COLUMNS])
{
    char taken_seat = 'X';
    char reserved_seat = '+';

    for (int i = 0; i < rows + 1; i++)
    {
        for (int j = 0; j < cols + paths + 1; j++)
        {
            if (seat[i][j] == reserved_seat)
            {
                seat[i][j] = taken_seat;
            }
        }
    }
}
