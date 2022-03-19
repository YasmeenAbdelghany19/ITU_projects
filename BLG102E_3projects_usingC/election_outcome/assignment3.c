#include <stdio.h>   //printf, scanf, fopen, fclose, fscanf, fgets, feof
#include <stdlib.h>  //EXIT_SUCCESS, EXIT_FAILURE
#include <stdbool.h> //true, false

#define MAX_LINE 80
#define N_CANDIDATES 4

//the following data structure represents a candidate
typedef struct candidate
{
    char letter;   // 'A', 'B', 'C', 'D'
    long int votes;     //total votes of a candidate
    float percent; //candidate's percentage of total votes
} CANDIDATE;

// Reads the vote data of each candidate from the opened file (district by district)
void read_file(FILE *, CANDIDATE *, int, char *);

// Calculate the total number of votes and percentages for each candidate.
void calculate(int **, CANDIDATE *, int);

// Decide which candidate wins or which two candidates will compete in the second round
void decide_winner(CANDIDATE *);

int main(int argc, char *argv[])
{
    //Exit the program if the number of command line arguments are not 2
    if (argc != 2)
    {
        printf("%s program requires two arguments\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *votes_file = argv[1];        //votes_file is the name of the file in the 2nd command line argument
    CANDIDATE candidate[N_CANDIDATES]; //struct array for the 4 candidates
    int total_lines = 0;
    FILE *vfPtr; //vfPtr = voters.txt file pointer

    //Exit the program if the file is not open
    if ((vfPtr = fopen(votes_file, "r")) == NULL)
    {
        printf("%s could not be opened", votes_file);
        exit(EXIT_FAILURE);
    }
    else
    {
        while (true)
        {
            char district[MAX_LINE];
            //read every district in the file
            fgets(district, sizeof(district), vfPtr);

            if (feof(vfPtr))
            {
                fclose(vfPtr);
                break;
            }
            total_lines++;
        }
        //read votes data from every district and call the calculation function
        read_file(vfPtr, candidate, total_lines, votes_file);
        //prints the winner or those who will compete in the 2nd round based on previous calculations onto the output screen
        decide_winner(candidate);
    }

    return EXIT_SUCCESS;
}

/**
 *  Takes the file pointer name and reads vote data of each district   
 *  
 *  @param vfPtr the name of the file pointer taken from the 2nd command line argument
 *  @param candidate struct array of the 4 candidates
 *  @return no return - after reading each district (line) from the file,
 *                      assign each of the 4 integers to the first field(int votes) of each candidate's struct
 *                      and call the calculation fucntion.
 */
void read_file(FILE *vfPtr, CANDIDATE *candidate, int lines, char *votes_file)
{
    char district[MAX_LINE];

    //DYNAMIC ALLOCATION OF 2D ARRAY

    int **file_array = NULL;
    file_array = (int **)malloc(sizeof(int *) * lines);

    for (int i = 0; i < lines; i++)
    {
        file_array[i] = (int *)malloc(sizeof(int) * N_CANDIDATES);
        for (int j = 0; j < N_CANDIDATES; j++)
        {
            file_array[i][j] = 0;
        }
    }

    //exit the program if there is no memory allocated for this array
    if (file_array == NULL)
    {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    if ((vfPtr = fopen(votes_file, "r")) == NULL)
    {
        printf("%s could not be opened", votes_file);
        exit(EXIT_FAILURE);
    }
    else
    {
        int i = 0;
        while (true)
        {
            //read every district in the file
            fgets(district, sizeof(district), vfPtr);

            if (feof(vfPtr))
            {
                //end_of_file = true;
                fclose(vfPtr);
                calculate(file_array, candidate, lines);
                break;
            }

            sscanf(district, "%d %d %d %d",
                   &file_array[i][0],
                   &file_array[i][1],
                   &file_array[i][2],
                   &file_array[i][3]);

            i++;
        }
    }
}

/**
 *  Calculate the total number of votes of each candidated, all the number of votes, and the percentages for each candidate .
 *  
 *  @param file_array array of size 4 that represent the total value of the votes in each column
 *  @param candidate struct array of the 4 candidates
 *  @return no return - Calculates the total votes of each candidated by enumerating the votes data taken from the previous function.
 *                      Calculates the total votes in the file and the candidates' percentage of total votes.
 */
void calculate(int **file_array, CANDIDATE *candidate, int lines)
{
    int total_votes = 0;

    for (int i = 0; i < N_CANDIDATES; i++)
    {
        candidate[i].votes = 0;
    }

    //EACH CANDIDATES' TOTAL VOTES
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < N_CANDIDATES; j++)
        {
            candidate[j].votes += file_array[i][j];
        }
    }

    //FREEING MEMORY OF 2D ARRAY
    for (int i = 0; i < lines; i++)
    {
        free(file_array[i]);
    }
    free(file_array);

    //TOTAL VOTES CALCULATIONS
    for (int i = 0; i < N_CANDIDATES; i++)
    {
        total_votes += candidate[i].votes;
    }
    
    //PERCENTAGES CALCULATIONS
    for (int i = 0; i < N_CANDIDATES; i++)
    {
        candidate[i].percent = (float)candidate[i].votes / total_votes * 100;
    }
}

/**
 *   Decide which candidate wins or which two candidates will compete in the second round
 *  
 *  @param candidate struct array of the 4 candidates
 *  @return no return - prints the winner or the 2 candidates that  will compete in the second round onto the output screen
 */
void decide_winner(CANDIDATE *candidate)
{
    char winner;
    char letter = 'A';
    double highest = 0;
    double second_highest = 0;
    bool found_winner = false;

    for (int i = 0; i < N_CANDIDATES; i++)
    {
        candidate[i].letter = letter++;
    }

    for (int i = 0; i < N_CANDIDATES; i++)
    {
        if (candidate[i].percent > 50)
        {
            winner = candidate[i].letter; //the winner is the candidates who got more than 50% of the total votes
            printf("Winner: %c\n", winner);
            found_winner = true;
        }
    }

    /*  If no candidate has more than 50% of the votes, there will be a second round
        between the two candidates who have received the highest number of votes.   */
    if (found_winner == false)
    {
        char first_in_round2;  //is the candidate with the highest number of votes
        char second_in_round2; //is the candidate with the second highest number of votes

        //finds the first candidate
        for (int i = 0; i < N_CANDIDATES; i++)
        {
            if (candidate[i].percent > highest)
            {
                highest = candidate[i].percent;
                first_in_round2 = candidate[i].letter;
            }
        }

        //finds the second candidate
        for (int i = 0; i < N_CANDIDATES; i++)
        {
            if (candidate[i].letter == first_in_round2)
            {
                continue; //skip the candidate who has the highest votes out of all
            }
            else if (candidate[i].percent > second_highest)
            {
                second_highest = candidate[i].percent;
                second_in_round2 = candidate[i].letter;
            }
        }

        printf("Second round: %c and %c\n", first_in_round2, second_in_round2);
    }
}
