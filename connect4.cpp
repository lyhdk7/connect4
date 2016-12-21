#include "board.h"
//#include "graphics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;



pair<int, int> maxi(int board[NUM_ROWS][NUM_COLS], int depth, int player);

pair<int, int> minii(int board[NUM_ROWS][NUM_COLS], int depth, int player);




bool isFull(int myBoard[NUM_ROWS][NUM_COLS])
{
    for(int i=0; i<7; i++)
        if(myBoard[6-1][i]==0)
            return false;

    return true; 
}


int calculatePosition(int myBoard[NUM_ROWS][NUM_COLS], int Row, int Col, int a, int b)
{
    int humanPoint = 0;
    int bobPoint = 0;
    int total = 0;
    for (int i = 0; i < 4; i++)
    {
        if (myBoard[Row][Col] == 1) humanPoint++;
        else if (myBoard[Row][Col] == -1) bobPoint++;

        Row += a;
        Col += b;
    }

    if (humanPoint == 4) return 100000;
    else if (bobPoint == 4) return -100000;
    else return -bobPoint;


}


int board_score(int myBoard[NUM_ROWS][NUM_COLS])
{
    int total = 0;
    for (int i = 0; i < NUM_ROWS - 3; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            int cur = calculatePosition(myBoard, i, j, 1, 0);
            if (cur == 100000 || cur == -100000) return cur;
            total += cur;
        }
    }

    for (int i = 0; i < NUM_ROWS ; i++)
    {
        for (int j = 0; j < NUM_COLS-3; j++)
        {
            int cur = calculatePosition(myBoard, i, j, 0, 1);
            if (cur == 100000 || cur == -100000) return cur;
            total += cur;
        }
    }
    for (int i = 0; i < NUM_ROWS - 3; i++)
    {
        for (int j = 0; j < NUM_COLS-3; j++)
        {
            int cur = calculatePosition(myBoard, i, j, 1, 1);
            if (cur == 100000 || cur == -100000) return cur;
            total += cur;
        }
    }
    for (int i = 3; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS-3; j++)
        {
            int cur = calculatePosition(myBoard, i, j, -1, 1);
            if (cur == 100000 || cur == -100000) return cur;
            total += cur;
        }
    }
    return total;
}




pair<int, int> maxi(int board[NUM_ROWS][NUM_COLS], int depth, int player)
{
    /*
    int e = endgame(board, player);
    if (e == 1) return make_pair(0, 100000);
    else if (e == -1) return make_pair(0, -100000);
    //else if (e == 3) return make_pair(0, 0);
    */
    if (depth == 0 ||board_score(board) == 100000 || board_score(board) == -100000 ||isFull(board))
    {
        return make_pair(0, board_score(board));
    }

    


    pair<int, int> max = make_pair(-1, -99999);

    // For all possible moves
    for (int col = 0; col < NUM_COLS; col++) {
        if (board[NUM_ROWS-1][col] == 0)
        {
            for(int i = 0; i < NUM_ROWS; i++) {
                if(board[i][col] == 0) {
                board[i][col] = player;
                break;
                }
            }

            pair<int, int> next_move = minii(board, depth-1, -player);

            if (max.first == -1 || next_move.second > max.second)
            {
                max.first = col;
                max.second = next_move.second;
            }
            for (int i=NUM_ROWS-1; i>=0; i--){
                if (board[i][col] != 0) {
                    board[i][col] = 0;
                    break;
                }
            }

        }

        
    }

    return max;


}


pair<int, int> minii(int board[NUM_ROWS][NUM_COLS], int depth, int player)
{

    /*
    int e = endgame(board, player);
    if (e == 1) return make_pair(0, 100000);
    else if (e == -1) return make_pair(0, -100000);
    //else if (e == 3) return make_pair(0, 0);
    
    if (depth == 0 )
    {
        return make_pair(0, board_score(board));
    }
*/
    /*
    int e = endgame(board, player);
    if (e == 1) return make_pair(0, 100000);
    else if (e == -1) return make_pair(0, -100000);
    //else if (e == 3) return make_pair(0, 0);
    */
    if (depth == 0 ||board_score(board) == 100000 || board_score(board) == -100000 ||isFull(board))
    {
        return make_pair(0, board_score(board));
    }
    


    pair<int, int> min = make_pair(-1, 99999);

    for (int col = 0; col < NUM_COLS; col++)
    {
        if (board[NUM_ROWS - 1][col] == 0)
        {
            for(int i = 0; i < NUM_ROWS; i++) {
                if(board[i][col] == 0) {
                board[i][col] = player;
                break;
                }
            }
            pair<int, int> next_move = maxi(board, depth-1, -player);
            if (min.first == -1 || next_move.second < min.second)
            {
                min.first = col;
                min.second = next_move.second;
            }
            for (int i=NUM_ROWS-1; i>=0; i--){
                if (board[i][col] != 0) {
                    board[i][col] = 0;
                    break;
                }
            }
        }
    }
    return min;
}
























void terminalPlay()
{
    int key_num;
    bool running = true;
    Board game_board = Board();
    game_board.prettyPrintBoard(cout);
    while(running)
    {
        Result result;
        int myBoard[NUM_ROWS][NUM_COLS];
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLS; j++)
                {
                    if (game_board.atLocation(i, j) == Empty) myBoard[i][j] = 0;
                    else if (game_board.atLocation(i, j) == Player1) myBoard[i][j] = 1;
                    else myBoard[i][j] = -1;
                }
            }



        if (game_board.toMove() == 1)
        {

            cin >> key_num;
            result = game_board.makeMove(key_num - 1);
            if (result == Win)
            {
                cout<<"You Win"<<endl;
                running = false;

            }
            else if (result == Draw)
            {
                cout<<"Draw"<<endl;
                running = false;
            }
            else if (result == NoResult){}

            game_board.prettyPrintBoard(cout);

        //cout<<"    my score:"<<board_score(myBoard)<<endl;
        //cout<<"    calcu"<<calculatePosition(myBoard, 0, 0, 1, 0) << " "<<calculatePosition(myBoard, 0, 6, 1, 0)<<endl;
        }
        else
        {
            

            

            
            int col = minii(myBoard, 5, -1).first;
            result = game_board.makeMove(col);
            if (result == Win)
            {
                cout<<"robot Win"<<endl;
                running = false;

            }
            else if (result == Draw)
            {
                cout<<"Draw"<<endl;
                running = false;
            }
            else if (result == NoResult){}

            game_board.prettyPrintBoard(cout);


        }
    }
}

int main(int argc, char *argv[])
{
    int choice;
     terminalPlay();
    
    return 0;
}
