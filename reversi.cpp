#include <vector>
#include <stdexcept>

#include "reversi.h"

using namespace std;


Square& Square::operator=(SquareValue value){
	this -> value_ = value;
    return *this;
}

bool Square::operator==(SquareValue value) const {
	if(value_ == value){
		return true;
	}
	else{
		return false;
	}
}

bool Square::operator!=(SquareValue value) const{
	if(value_ != value){
		return true;
	}
	else{
		return false;
	}
}

std::ostream& operator<<(std::ostream& out, const Square& square){
	if(square.value_ == Square::FREE){
		out << '-';
	}
	else if(square.value_ == Square::WHITE){
		out << 'W';
	}
	else{
		out << 'B';
	}
	return out;
}

void Square::flip()
{
    switch (value_)
    {
    case WHITE:
        value_ = BLACK;
        break;
    case BLACK:
        value_ = WHITE;
        break;
    default:
        break;
    }
}

Square::SquareValue opposite_color(Square::SquareValue value)
{
    switch (value)
    {
    case Square::WHITE:
        return Square::BLACK;
    case Square::BLACK:
        return Square::WHITE;
    default:
        throw invalid_argument("Illegal square value");
    }
}



Board::Board(size_t s){
	dimension_ = s;
    //allocate 2d board array
	squares_ = new Square*[dimension_];
	for(size_t i = 0; i < dimension_; i++){
		squares_[i] = new Square[dimension_];
	}
}

Board::Board(){
    //set default size to 4
    dimension_ = 4;
    //allocate 2d board array
    squares_ = new Square*[dimension_];
    for(size_t i = 0; i < dimension_; i++){
        squares_[i] = new Square[dimension_];
    }
}

Board::Board(const Board& rhs){
    dimension_ = rhs.dimension_;
    squares_ = new Square*[dimension_];
    //allocate 2d board array
    for(size_t i = 0; i < dimension_; i++){
        squares_[i] = new Square[dimension_];
    }
    //copy each element from rhs to this array
    for(size_t i = 0; i < dimension_; i++){
        for(size_t j = 0; j < dimension_; j++){
            squares_[i][j] = rhs.squares_[i][j];
        }
    }
}

Board::~Board(){
    //clear memory allocated for board
    for(size_t i = 0; i < dimension_; i++){
        delete [] squares_[i];
    }
    delete [] squares_;
}

Board& Board::operator=(const Board& rhs){
    //check for self assignment
    if(this == &rhs){
        return *this;
    }
    //delete and reallocate board if it already exists
    if(squares_){
        for(size_t i = 0; i < dimension_; i++){
            delete [] squares_[i];
        }
        delete [] squares_;

        dimension_ = rhs.dimension_;
        squares_ = new Square*[dimension_];
        for(size_t i = 0; i < dimension_; i++){
            squares_[i] = new Square[dimension_];
        }
    }
    //copy each element from rhs to this array
    for(size_t i = 0; i < dimension_; i++){
        for(size_t j = 0; j < dimension_; j++){
            squares_[i][j] = rhs.squares_[i][j];
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Board& board){
	return board.print(out);
}

Square& Board::operator()(char row, size_t column)
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

Square const& Board::operator()(char row, size_t column) const
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

bool Board::is_legal_and_opposite_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] != Square::FREE && squares_[row_index][column - 1] != turn;
    }
    return false;
}

bool Board::is_legal_and_same_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] == turn;
    }
    return false;
}

bool Board::is_valid_location(char row, size_t column) const
{
    size_t row_index = row_to_index(row);
    return row_index < dimension_ && column - 1 < dimension_;
}


Checkpoint::Checkpoint(const Board& b, Square::SquareValue turn) :
    board_(b),
    turn_(turn)
{

}

ostream& Board::print(ostream& out) const
{
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        if (i < 10)
        {
            out << " ";
        }
        else
        {
            out << (i / 10);
        }
    }
    out << endl;
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        out << (i % 10);
    }
    out << endl;
    for (size_t i = 0; i < dimension_; i++)
    {
        out << (char)('a' + i) << ':';
        for (size_t k = 0; k < dimension_; k++)
        {
            out << squares_[i][k];
        }
        out << endl;
    }
    return out;
}

Reversi::Reversi(size_t size){
    //check for invalid size
	if(size % 2 != 0){
		cout << "Invalid size";
		return;
	}
	else if(size < 4 || size > 26){
		cout << "Invalid size";
		return;
	}
    //construct new board
    Board board(size);
    board_ = board;
    //set middle four squares to starting state
	board_(board_.indexToRow(size / 2 - 1), size / 2) = Square::BLACK;
	board_(board_.indexToRow(size / 2), size / 2) = Square::WHITE;
	board_(board_.indexToRow(size / 2 - 1), size / 2 + 1) = Square::WHITE;
	board_(board_.indexToRow(size / 2), size / 2 + 1) = Square::BLACK;
	turn_ = Square::BLACK;
}

void Reversi::play(){
	int numBlack; int numWhite;
	while(!is_game_over()){
		prompt();
        //get user input
		char entry;
		cin >> entry;
		if(entry == 'p'){
            //get row and column and clear stream if needed
			char row;
			int col;
			cin >> row;
			cin >> col;
            cin.clear();
            cin.ignore(256, '\n');
			if(is_legal_choice(row, col, turn_)){
				const size_t direction_count = 8;
    			const int direction_row[] =    {-1, -1,  0, +1, +1, +1,  0, -1};
    			const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1};
                //vectors to store paths if flips needed
    			vector<char> rowPath;
    			vector<int> colPath;

				for (size_t d = 0; d < direction_count; d++)
    			{
        			//which square is being checked
        			char cursor_row = row + direction_row[d];
        			size_t cursor_column = col + direction_column[d];
                    
        			//traverse board while on opposite color
        			bool found_opposite = false;
        			while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn_))
        			{
            			found_opposite = true;
            			rowPath.push_back(cursor_row);
            			colPath.push_back(cursor_column);
            			cursor_row += direction_row[d];
            			cursor_column += direction_column[d];
        			}

        			//check if following square is same color
        			bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn_);
                    //flip pieces stored in path vectors if needed
        			if(found_opposite && found_same){
                        board_(row, col) = turn_;
        				for(size_t i = 0; i < rowPath.size(); i++){
        					board_(rowPath[i], colPath[i]).flip();
        				}
                        
        			}
                    //clear path vectors
        			rowPath.clear();
        			colPath.clear();
                    
    			}
                turn_ = opposite_color(turn_);
			}
		}
        //save checkpoint and clear stream
		else if(entry == 'c'){
			save_checkpoint();
            cin.clear();
            cin.ignore(256, '\n');
		}
        //undo to last checkpoint and clear stream
		else if(entry == 'u'){
			undo();
            cin.clear();
            cin.ignore(256, '\n');
		}
        //quit game and display winner
		else{
			countColors(numBlack, numWhite);
			win_loss_tie_message(numWhite, numBlack);
            cin.clear();
            cin.ignore(256, '\n');
			return;
		}

	}
	//count colors and display winner
	countColors(numBlack, numWhite);
	win_loss_tie_message(numWhite, numBlack);
	

}

void Reversi::save_checkpoint(){
    //add checkpoint to history vector
    Board cBoard(board_);
	Checkpoint temp(cBoard, turn_);
	history_.push_back(temp);
}


void Reversi::countColors(int& blackCount, int& whiteCount){
    //traverse board and count black and white squares
	whiteCount = 0; blackCount = 0;
	for(size_t i = 0; i < board_.dimension(); i++){
		for(size_t j = 1; j <= board_.dimension(); j++){
			if(board_('a' + i, j) == Square::BLACK){
				blackCount++;
			}
			else if(board_('a' + i, j) == Square::WHITE){
				whiteCount++;
			}
		}
	}
}

void Reversi::undo(){
    //check if undo is available
	if(history_.size() < 1){
		return;
	}
    //revert to last checkpoint
	board_ = history_.back().board_;
	turn_ = history_.back().turn_;
    history_.pop_back();
}

void Reversi::prompt() const
{
    cout << board_ << endl;
    cout << (turn_ == Square::BLACK ? "B" : "W");
    cout << " - Enter 'p r/c', 'q', 'c', 'u':" << endl;
}

void Reversi::win_loss_tie_message(size_t white_count, size_t black_count)
{
    cout << board_ << endl;
    if (white_count > black_count)
    {
        cout << "W wins" << endl;
    }
    else if (white_count < black_count)
    {
        cout << "B wins" << endl;
    }
    else
    {
        cout << "Tie" << endl;
    }
    cout << "W=" << white_count << "/B=" << black_count << endl;
}

bool Reversi::is_legal_choice(char row, size_t column, Square::SquareValue turn) const
{
    // Vectors for each cardinal direction
    const size_t direction_count = 8;
    const int direction_row[] =    {-1, -1,  0, +1, +1, +1,  0, -1};
    const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1};

    // Make sure location is free
    if (board_(row, column) != Square::FREE)
    {
        return false;
    }

    // Now check in each directions
    for (size_t d = 0; d < direction_count; d++)
    {
        // Where we're checking
        char cursor_row = row + direction_row[d];
        size_t cursor_column = column + direction_column[d];

        // Move towards the direction while we're on the opposite color
        bool found_opposite = false;
        while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn_))
        {
            found_opposite = true;
            cursor_row += direction_row[d];
            cursor_column += direction_column[d];
        }

        // Check if the next thing after is our color
        bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn_);

        // If this direction is valid, the move is valid, so short circuit and return
        if (found_opposite && found_same) {
            return true;
        }
    }
    return false;
}

bool Reversi::is_game_over() const
{
    for (unsigned char row = 'a'; row < 'a' + board_.dimension(); row++)
    {
        for (size_t column = 1; column <= board_.dimension(); column++)
        {
            if (is_legal_choice(row, column, turn_))
            {
                return false;
            }
        }
    }
    return true;
}



