#include "board.h"

Board::Board(){
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            data[row][col] = Empty;
        }
    }
    nextPlayer_to_move = Player1;
}

Board::Board(const string &fen)
{
    int row = 0;
    int col = 0;
    for (int i = 0; i < fen.length(); i++)
    {
        if (fen[i] == '/')
        {
            row++;
            col = 0;

        }
        else if (fen[i] == 'x')
        {
            data[row][col] = Player1;
            col++;
        }
        else
        {
            int count = fen[i]-'0';
            for (int j = 0; j < count; j++)
            {
                data[row][col] = Empty;
                col++;
            }
        }
    }
    if (toMove() == 1)
    {
        nextPlayer_to_move = Player1;
    }
    else{
        nextPlayer_to_move = Player2;
    }
}


void Board::printBoard_as_FENstring(ostream &os) const
{
    for (int row = 0; row < NUM_ROWS; row++)
    {
        int count = 0;
        for (int col = 0; col < NUM_COLS - 1; col++)
        {
            if (data[row][col] == Player1)
            {
                os<<"x";
            }
            else if (data[row][col] == Player2)
            {
                os<<"o";
            }
            else count++;

            if (data[row][col+1] != Empty)
            {
                if (count!=0) os<<count;

                count = 0;
            }
        }

        if (data[row][6] == Player1)
        {
            os<<"x";
        }
        else if (data[row][6] == Player2)
        {
            os<<"o";
        }
        else{
            count++;
            os<<count;
        }
        os<<"/";
    }
    if (nextPlayer_to_move == Player1)
    {
        os<<" x";
    }
    else os<<" o";

    os<<endl;
}



PieceType Board::atLocation(int row, int col)
{
    return data[row][col];
}

void Board::prettyPrintBoard(ostream &os) const{
    os<<endl;
    for (int row = NUM_ROWS -1; row>=0; row--)
    {
        os<<"    +---+---+---+---+---+---+---+"<<endl<<"    ";
        for (int col = 0; col < NUM_COLS; col++)
        {
            os << " | ";
            if (data[row][col] == Player1) os<<PLAYER1_TOKEN;
            else if (data[row][col] == Player2) os << PLAYER2_TOKEN;
            else os<<EMPTY_TOKEN;
        }
        os<<" |"<<endl;
    }
    os<<"    +---+---+---+---+---+---+---+"<<endl<<"   col   1  2   3   4   5   6   7"<<endl;
    return;
}

int Board::toMove() const{
    int count_x = 0;
    int count_o = 0;
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            if (data[row][col] == Player1)
            {
                count_x++;

            }
            else if (data[row][col] == Player2) count_o++;
        }
    }
    if (count_x <= count_o) return 1;
    else return 2;
}

Result Board::makeMove(int col)
{
    int row = getFirstFreeRow(col);
    if (!inBounds(row, col)) return IllegalMove;
    data[row][col] = updateToMove();
    if (isWin(row, col))
    {
        return Win;

    }
    else if (isBoardFull()) return Draw;
    return NoResult;
}

int Board::getFirstFreeRow(int col) const{
    for (int row = 0; row < NUM_ROWS; row++)
    {
        if (data[row][col] == Empty) return row;
    }
    return NUM_ROWS;
}

PieceType Board::updateToMove()
{
    if (nextPlayer_to_move == Player1)
    {
        nextPlayer_to_move = Player2;
        return Player1;
    }
    else
    {
        nextPlayer_to_move = Player1;
        return Player2;
    }
}

bool Board::isBoardFull() const{
    bool is_full = true;
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            if (data[row][col] == Empty) is_full = false;
        }
    }
    return is_full;
}

bool Board::inBounds(int row, int col) const{
    if (row < NUM_ROWS && col < NUM_COLS && row >= 0 && col >=0) return true;

    return false;
}

int Board::piecesInDirection(int row, int col, int dRow, int dCol) const{
    PieceType sample = data[row][col];
    if (sample == Empty) return 0;

    if (dRow == 1 && dCol == 0)
    {
        int count = 0;
        while(inBounds(++row, col) && data[row][col] == sample) count++;

        return count;
    }

    if (dRow == -1 && dCol == 0)
    {
        int count = 0;
        while(inBounds(--row, col) && data[row][col] == sample) count++;

        return count;
    }
    if (dRow == 0 && dCol == 1)
    {
        int count =0;
        while(inBounds(row, ++col) && data[row][col] == sample) count++;

        return count;
    }
    if (dRow == 0 && dCol == -1)
    {
        int count = 0;
        while(inBounds(row, --col) && data[row][col] == sample) count++;

        return count;
    }
    if (dRow == 1 && dCol == 1)
    {
        int count = 0;
        while(inBounds(++row, ++col) && data[row][col] == sample)
        {
            count++;
        }
        return count;
    }
    if (dRow == -1 && dCol == -1)
    {
        int count = 0;
        while(inBounds(--row, --col) && data[row][col] == sample) count++;

        return count;
    }

    if (dRow == -1 && dCol == 1)
    {
        int count = 0;
        while(inBounds(--row, ++col) && data[row][col] == sample)
        {
            count++;
        }
        return count;
    }
    if (dRow == 1 && dCol == -1)
    {
        int count = 0;
        while(inBounds(++row, --col) && data[row][col] == sample) count++;

        return count;
    }

    return 0;



}

bool Board::isWin(int row, int col) const{
    return (piecesInDirection(row, col, 0, 1) + piecesInDirection(row, col, 0, -1))>2||
            (piecesInDirection(row, col, 1, 0) + piecesInDirection(row, col, -1, 0))>2||
            (piecesInDirection(row, col, 1, 1) + piecesInDirection(row, col, -1, -1))>2||
            (piecesInDirection(row, col, 1, -1) + piecesInDirection(row, col, -1, 1))>2;
}
