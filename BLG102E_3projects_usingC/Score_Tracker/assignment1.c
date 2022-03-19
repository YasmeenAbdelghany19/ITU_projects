#include <stdio.h>   //printf, scanf
#include <stdlib.h>  //EXIT_SUCCESS, EXIT_FAILURE
#include <stdbool.h> //true, false

int main()
{
    int points_player1 = 0;
    int points_player2 = 0;
    int score_player1 = 0;
    int score_player2 = 0;
    int point = 0;
    bool deuce = true;

    printf("0-0\n");

    //prompts the user to enter 1 or 2 until at least three points are scored by each player
    do
    {
        printf("Point: ");
        scanf("%d", &point);

        //checks if the point is scored by 1 (player1) or 2 (player2), increments their points
        switch (point)
        {
        case 1:
            points_player1++;

            //checks if both players have scored equal points
            if (points_player1 == 3 && points_player2 == 3)
            {
                printf("DEUCE\n");
                deuce = true;
                break;
            }
            //checks if player1 has at least 4 points and 2 points more than player 2, he wins
            else if (points_player1 >= 4 && points_player1 >= points_player2 + 2)
            {
                printf("GAME: PLAYER 1\n");
                return EXIT_SUCCESS;
            }
            //prints the scores of player 1 and player 2
            else
            {
                //calculates scores of points zero, one, two and three as 0, 15, 30, 40
                score_player1 = points_player1 < 3 ? points_player1 * 15 : 40;

                printf("%d-%d\n", score_player1, score_player2);
                deuce = false;
            }
            break;
        case 2:
            points_player2++;

            //checks if both players have scored equal points 3-3
            if (points_player1 == 3 && points_player2 == 3)
            {
                printf("DEUCE\n");
                deuce = true;
                break;
            }
            //checks if player2 has at least 4 points and 2 points more than player 1, he wins
            else if (points_player2 >= 4 && points_player2 >= points_player1 + 2)
            {
                printf("GAME: PLAYER 2\n");
                return EXIT_SUCCESS;
            }
            //prints the score of player 1 and player 2
            else
            {
                //calculates scores of points zero, one, two and three as 0, 15, 30, 40
                score_player2 = points_player2 < 3 ? points_player2 * 15 : 40;

                printf("%d-%d\n", score_player1, score_player2);
                deuce = false;
            }
            break;
        }
    } while (deuce == false);

    /**
    * if the game status is a deuce -> 3 points are scored by each player
    * prompts the user for input until one player wins
    */
    while (deuce)
    {
        printf("Point: ");
        scanf("%d", &point);

        //checks if player 1 or player 2 is one point ahead 
        switch (point)
        {
        case 1:
            printf("ADVANTAGE: PLAYER 1\n");
            printf("Point: ");
            scanf("%d", &point);

            //checks if player 1 scores again, he wins
            if (point == 1)
            {
                printf("GAME: PLAYER 1\n");
                return EXIT_SUCCESS;
            }
            //checks if instead player 2 scores, game status is a deuce
            else if (point == 2)
            {
                printf("DEUCE\n");
                deuce = true;
            }
            break;
        case 2:
            printf("ADVANTAGE: PLAYER 2\n");
            printf("Point: ");
            scanf("%d", &point);

            //checks if player 2 scores again, he wins
            if (point == 2)
            {
                printf("GAME: PLAYER 2\n");
                return EXIT_SUCCESS;
            }
            //checks if instead player 1 scores, game status is a deuce
            else if (point == 1)
            {
                printf("DEUCE\n");
                deuce = true;
            }
            break;
        }
    }
    return EXIT_SUCCESS;
}