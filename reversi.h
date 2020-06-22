#ifndef REVERSI_HPP
#define REVERSI_HPP

#include <iostream>
#include <vector>

struct Square
{

    enum SquareValue
    {
        FREE = 0,
        WHITE,
        BLACK
    };

    SquareValue value_;

    Square(SquareValue value = FREE) : value_(value) {}

    Square &operator=(SquareValue value);

    bool operator==(SquareValue value) const;
    bool operator!=(SquareValue value) const;

    void flip();
};

std::ostream &operator<<(std::ostream &out, const Square &square);

Square::SquareValue opposite_color(Square::SquareValue value);

class Board
{
public:
    Board(size_t s);
    Board();
    Board(const Board &rhs);

    Board &operator=(const Board &rhs);

    ~Board();

    size_t dimension() const
    {
        return dimension_;
    }

    Square &operator()(char row, size_t column);
    Square const &operator()(char row, size_t column) const;

    bool is_legal_and_opposite_color(char row, size_t column, Square::SquareValue turn) const;
    bool is_legal_and_same_color(char row, size_t column, Square::SquareValue turn) const;

    std::ostream &print(std::ostream &out) const;

    friend std::ostream &operator<<(std::ostream &out, const Board &board);

    char indexToRow(size_t row)
    {
        return 'a' + row;
    }

private:
    size_t dimension_;
    Square **squares_;

    size_t row_to_index(char row) const
    {
        return (size_t)(row - 'a');
    }

    bool is_valid_location(char row, size_t column) const;
};

struct Checkpoint
{
    Board board_;
    Square::SquareValue turn_;

    Checkpoint(const Board &b, Square::SquareValue t);
};

class Reversi
{
public:
    Reversi(size_t size);

    void play();

private:
    void prompt() const;

    void win_loss_tie_message(size_t white_count, size_t black_count);

    bool is_game_over() const;

    bool is_legal_choice(char row, size_t column, Square::SquareValue turn) const;

    void save_checkpoint();

    void undo();

    void countColors(int &blackCount, int &whiteCount);

private:
    Board board_;

    Square::SquareValue turn_;

    std::vector<Checkpoint> history_;

    int historySize;
};

#endif
