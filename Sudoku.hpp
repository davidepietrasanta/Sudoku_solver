#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <iostream>
#include <list>
#include <iterator>
#include <stdexcept>
#include <cmath>

//#define DEBUGGING

/**
	@file Sudoku.hpp 
	@brief Sudoku template class declaration
*/

/**
	@brief out_of_range_input.

	Custom exception class that derives from std :: runtime_error
	It is thrown when you try to access indexes outside the sudoku table.
*/
class out_of_range_input : public std::runtime_error {
public:
	/**
		Default constructor
	*/
    out_of_range_input() : std::runtime_error("Out of range input") {}
};

/**
	@brief bad_input.

	Custom exception class that derives from std :: runtime_error
	It is thrown when you try to insert bad input inside the sudoku table. 
*/
class bad_input : public std::runtime_error {
public:
	/**
		Default constructor
	*/
    bad_input() : std::runtime_error("Bad input") {}
};

/**
	@brief my_uninitialized_value
	Custom exception class that derives from std::runtime_error.
	It is generated when trying to access values
	not yet initialized.
*/
class my_uninitialized_value: public std::runtime_error {
public:
	/**
		Default constructor
	*/
    my_uninitialized_value() : std::runtime_error("Value not inizializated") {}
};



/**
	@brief Sudoku solver.

	Class that implements a Sudoku table.

	@param table A 9x9 matrix of Cell. (private).
	@param changed It tells us if there have been any changes since the last check.

*/
class Sudoku{
private:

	/**
		@brief Sudoku cell.

		Structure that implements a Sudoku table cell.

		@param flag [bool] If true we have found the number.
		@param value [int] Real value of the cell.
		If -1 we have not found the number.
		@param possible_values [std::list<int>*] Possible values in a cell.
	*/
	struct Cell {
		
		bool flag; 
		int value; 
		std::list<int>* possible_values; 

		/**
			@brief Default constructor.

			Default constructor that instantiates an ampty Cell.
			'flag' is set to false, 'value' is set to -1 and 
			'possible_values' has all the possible values.
		*/
		Cell(): flag(false), value(-1) {
			possible_values = new std::list<int>();
			for(int i=0; i < 9; i++){
				possible_values->push_back(i+1);				
			}
		}

		/**
			@brief Constructor.

			Constructor that instantiates an depp copy Cell.
		*/
		Cell(const Cell &other) {
			this->flag = other.flag;
			this->value = other.value;
			//this->possible_values = new std::list<int>(other.possible_values);
			delete this->possible_values;
			this->possible_values = new std::list<int> ();
			std::list<int>::iterator it;
			for (it = other.possible_values->begin(); it != other.possible_values->end(); ++it){
				this->possible_values->push_back( *it );
			}
		}

		/**
			@brief Constructor.

			Constructor that instantiates an depp copy Cell.
		*/
		Cell(const Cell *other) {
			this->flag = other->flag;
			this->value = other->value;
			//this->possible_values = new std::list<int>(other->possible_values);
			delete this->possible_values;
			this->possible_values = new std::list<int> ();
			std::list<int>::iterator it;
			for (it = other->possible_values->begin(); it != other->possible_values->end(); ++it){
				this->possible_values->push_back( *it );
			}
		}

		/**
			@brief Constructor.

			Constructor that instantiates a Cell.
			'flag' is set to true, 'value' is set to n and 
			'possible_values' is empty.

			@param n [int] Actual value of the Cell.
		*/
		explicit Cell(int n): flag(true), value(n) {
			possible_values = new std::list<int> (1, n);
		}

		/**
			@brief Destroyer.

			Removes the allocated memory from the Cell.
		*/
		~Cell(){
			//delete this->possible_values;
		}

		/**
			@brief Assignment operator

			Redefine operator =.
			Allows copying between Cells.

			@param other Cell to copy
		*/
		Cell& operator=(const Cell &other){	
			this->flag = other.flag;
			this->value = other.value;

			if( !other.flag ){
				//this->possible_values =  new std::list<int> (other.possible_values);
				delete this->possible_values;
				this->possible_values = new std::list<int> ();
				std::list<int>::iterator it;
				for (it = other.possible_values->begin(); it != other.possible_values->end(); ++it){
					this->possible_values->push_back( *it );
				}
			}

			return *this;
		}

		/**
			@brief Assignment operator

			Redefine operator =.
			Allows copying between Cells.

			@param other Cell to copy
		*/
		Cell& operator=(const Cell *other){	
			this->flag = other->flag;
			this->value = other->value;

			if( !other->flag ){
				//this->possible_values = new std::list<int> (other->possible_values);
				delete this->possible_values;
				this->possible_values = new std::list<int> ();
				std::list<int>::iterator it;
				for (it = other->possible_values->begin(); it != other->possible_values->end(); ++it){
					this->possible_values->push_back( *it );
				}

			}

			return *this;
		}

		/**
			@brief Assignment operator

			Redefine operator =.
			Allows copying between Cell and int.

			@param value Cell to copy
		*/
		Cell& operator=(int value){	
			this->flag = true;
			this->value = value;
			delete this->possible_values;
			this->possible_values = new std::list<int> ();

			return *this;
		}


	};

	Cell table [9][9]; 
	bool changed;

	/**
		@brief Check if value is possible in that column.

		If 'val' is in the (x,y) Cell return true.
		Else, check if 'val' can be a possible value looking only the column.

		@param val [int] Value to check, in [1,9].
		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].
	*/
	bool check_col(int val, int x, int y){

		if( this->get(x, y) == val ){
			return true;
		}
		bool flag = true;
		for(int i=1; i <= 9; i++){
			if( (this->get(x, i) == val) && (i != y) ){
				return false;
			}
		}
		return true;
	}

	/**
		@brief Check if value is possible in that row.

		If 'val' is in the (x,y) Cell return true.
		Else, check if 'val' can be a possible value looking only the row.

		@param val [int] Value to check, in [1,9].
		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].
	*/
	bool check_row(int val, int x, int y){
		if( this->get(x, y) == val ){
			return true;
		}
		bool flag = true;
		for(int i=1; i <= 9; i++){
			if( ( this->get(i, y) == val) && (i != x) ){
				return false;
			}
		}
		return true;
	}

	/**
		@brief Check if value is possible in that block.

		If 'val' is in the (x,y) Cell return true.
		Else, check if 'val' can be a possible value looking only the block.

		@param val [int] Value to check, in [1,9].
		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].
		@throw bad_input.
	*/
	bool check_block(int val, int x, int y){
		if( this->get(x, y) == val ){
			return true;
		}
		x = x-1;
		y = y-1;

		int x_block = (int) floor(x / 3); 
		int y_block = (int) floor(y / 3);

		int x_block_position = x_block * 3; //x_block_starting_position
		int y_block_position = y_block * 3; //y_block_starting_position

		for(int i=x_block_position; i <= ( x_block_position + 2 ); i++){
			for(int j=y_block_position; j <= ( y_block_position + 2 ); j++){
				if( ( table[i][j].value == val) && (i != x) && (j != y) ){
					return false;
				}
			}
		}

		return true;
	}

	/**
		@brief Check if value is possible in that place.

		If 'val' is in the (x,y) Cell return true.
		Else, check if 'val' can be a possible value.

		@param val [int] Value to check, in [1,9].
		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].
	*/
	bool is_Valid_place(int value, int x, int y){

		bool flag_value = value >= 1 && value <= 9;
		if( !flag_value ){
            #ifdef DEBUGGING
			std::cout << "Bad input from 'is_Valid_place()' " << std::endl;
            #endif //DEBUGGING
			throw bad_input();
		}

		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;

		if( ! (flag_x && flag_y) ){
			throw out_of_range_input();
		}

		bool  row = check_row( value, x, y);
		bool  col = check_col( value, x, y);
		bool  block = check_block( value, x, y);

		return ( row && col && block );
	}

	/**
		@brief Check if a value is in a std::list.

		@param val [int] Value to check.
		@param cpp_list [std::list<int>] A C++ list.
	*/
	bool is_value_in_list(int value, std::list<int>* cpp_list){
		for (std::list<int>::iterator it = cpp_list->begin(); it != cpp_list->end(); ++it){
			if( *it == value ){
				return true;
			}
		}
		return false;
	}

	/**
		@brief If 'value' in 'list' is removed and 'true' is returned.
		Else return 'false'.

		If 'x' or 'y' not in [1,9] throw out_of_range_input().


		@param value Value to remove.
		@param list List in which to search.
	*/
	bool remove_if_exist(std::list<int> *list, int value){
		bool flag_value = value >= 1 && value <= 9;
		if( !flag_value ){
			return false;
		}
		size_t len = list->size();
		list->remove(value);
		return len != list->size();
	}

	/**
		@brief Simplify column 'col'.
		Remove the impossible value from column 'col' looking only that row.
		If 'col' not in [1,9] throw out_of_range_input.
		If a possible_value is removed return 'true'.

		@param col Column number, in [1,9].

		@throw out_of_range_input
	*/
	bool simplify_col(int col){
		//Scan to search real values
		std::list<int> real_values;
		std::list<int>::iterator it;

		bool flag_col = col >= 1 && col <= 9;
		if( !flag_col ){
			throw out_of_range_input();
		}
		col = col - 1;

		bool flag = false;
		bool flag_t = false;
		for(int i=0; i < 9; i++){
			if( table[i][col].flag )
				real_values.push_back( table[i][col].value );
		}
		//Scan to remove possible values if in real values
		for(int i=0; i < 9; i++){
			if( !table[i][col].flag ){
				for (it = real_values.begin(); it != real_values.end(); ++it){
					flag_t = remove_if_exist( table[i][col].possible_values, *it);
					if( flag_t ){
						changed = true;
					}
					if( table[i][col].possible_values->size() == 1 ){
						table[i][col].value = table[i][col].possible_values->front();
						table[i][col].flag = true;
					}
					if( flag_t ){
						flag = true;
					}
				}
			}	
		}

		return flag;
	}

	/**
		@brief Simplify row 'row'.
		Remove the impossible value from row 'row' looking only that row.
		If 'row' not in [1,9] throw out_of_range_input.
		If a possible_value is removed return 'true'.

		@param row Row number, in [1,9].

		@throw out_of_range_input
	*/
	bool simplify_row(int row){
		//Scan to search real values
		std::list<int> real_values;
		std::list<int>::iterator it;

		bool flag_row = row >= 1 && row <= 9;
		if( !flag_row ){
			throw out_of_range_input();
		}
		row = row - 1;

		bool flag = false;
		bool flag_t = false;
		for(int i=0; i < 9; i++){
			if( table[row][i].flag )
				real_values.push_back( table[row][i].value );
		}
		//Scan to remove possible values if in real values
		for(int i=0; i < 9; i++){
			if( !table[row][i].flag ){
				for (it = real_values.begin(); it != real_values.end(); ++it){
					flag_t = remove_if_exist( table[row][i].possible_values, *it);
					if( flag_t ){
						changed = true;
					}
					if( table[row][i].possible_values->size() == 1 ){
						table[row][i].value = table[row][i].possible_values->front();
						table[row][i].flag = true;
					}
					if( flag_t ){
						flag = true;
					}
				}
			}	
		}

		return flag;
	}

	/**
		@brief Simplify block 'block'.
		Remove the impossible value from block 'block' looking only that row.
		If 'block' not in [1,9] throw out_of_range_input.
		If a possible_value is removed return 'true'.

		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].

		@throw out_of_range_input
	*/
	bool simplify_block(int x, int y){
		//Scan to search real values
		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;

		if( !(flag_x && flag_y) ){
			throw out_of_range_input();
		}

		std::list<int> real_values;
		std::list<int>::iterator it;
		bool flag = false;
		bool flag_t = false;

		int x_block = (int) floor(x / 3); 
		int y_block = (int) floor(y / 3);

		int x_block_position = x_block * 3; //x_block_starting_position
		int y_block_position = y_block * 3; //y_block_starting_position

		for(int i=x_block_position; i <= ( x_block_position + 2 ); i++){
			for(int j=y_block_position; j <= ( y_block_position + 2 ); j++){
				if( table[i][j].flag )
					real_values.push_back( table[i][j].value );
			}
		}

		for(int i=x_block_position; i <= ( x_block_position + 2 ); i++){
			for(int j=y_block_position; j <= ( y_block_position + 2 ); j++){
				if( !table[i][j].flag ){
					for (it = real_values.begin(); it != real_values.end(); ++it){
						flag_t = remove_if_exist( table[i][j].possible_values, *it);
						if( flag_t ){
							changed = true;
						}
						if( table[i][j].possible_values->size() == 1 ){
							table[i][j].value = table[i][j].possible_values->front();
							table[i][j].flag = true;
						}
						if( flag_t ){
							flag = true;
						}
					}
				}
			}
		}
		return flag;
	}

	/**
		@brief Search for number that appear one time in the row.
		If a number is possible just in a cell so that cell is solved.

		@param row Row number, in [1,9].

		@throw out_of_range_input
	*/
	bool one_possibility_row(int row){
		int real_values[9];
		std::list<int>::iterator it;
		bool flag_row = row >= 1 && row <= 9;

		if( !flag_row ){
			throw out_of_range_input();
		}
		row = row - 1;
		
		for(int i=0; i < 9; i++){
			real_values[i] = 0;
		}

		//Scan to search real values
		for(int i=0; i < 9; i++){
			if( !table[i][row].flag ){
				for (it = table[i][row].possible_values->begin(); it != table[i][row].possible_values->end(); ++it){
					real_values[(*it-1)]++;
				}
			}
		}

		//Scan to search one possibility
		std::list <int> values;
		for(int i=0; i < 9; i++){
			if( real_values[i] == 1 ){
				values.push_back( i + 1 );
			}
		}

		//Set values.
		bool flag = false;
		for (it = values.begin(); it != values.end(); ++it){
			for(int i=0; i < 9; i++){
				if( !table[i][row].flag ){
					if( is_value_in_list(*it, table[i][row].possible_values) ){
						if( is_Valid_place(*it, row+1, i+1) ){
							table[i][row] = *it;
							flag = true;
							changed = true;
                            #ifdef DEBUGGING
							std::cout << "row " << row+1 << ", value " << *it << std::endl;
                            #endif //DEBUGGING
						}
					}
				}
			}
		}
		
		return flag;
	}

	/**
		@brief Search for number that appear one time in the column.
		If a number is possible just in a cell so that cell is solved.

		@param col Column number, in [1,9].

		@throw out_of_range_input
	*/
	bool one_possibility_col(int col){
		int real_values[9];
		std::list<int>::iterator it;
		bool flag_col = col >= 1 && col <= 9;

		if( !flag_col ){
			throw out_of_range_input();
		}
		col = col - 1;
		
		for(int i=0; i < 9; i++){
			real_values[i] = 0;
		}

		//Scan to search real values
		for(int i=0; i < 9; i++){
			if( !table[col][i].flag ){
				for (it = table[col][i].possible_values->begin(); it != table[col][i].possible_values->end(); ++it){
					real_values[(*it-1)]++;
				}
			}
		}

		//Scan to search one possibility
		std::list <int> values;
		for(int i=0; i < 9; i++){
			if( real_values[i] == 1 ){
				values.push_back( i + 1 );
			}
		}

		//Set values.
		bool flag = false;
		for (it = values.begin(); it != values.end(); ++it){
			for(int i=0; i < 9; i++){
				if( !table[col][i].flag ){
					if( is_value_in_list(*it, table[col][i].possible_values) ){
						if( is_Valid_place(*it, i+1, col+1) ){
							table[col][i] = *it;
							flag = true;
							changed = true;
                            #ifdef DEBUGGING
							std::cout << "col " << col+1 << ", value " << *it << std::endl;
                            #endif //DEBUGGING
						}
					}
				}
			}
		}
		
		return flag;
	}

	/**
		@brief Search for number that appear one time in the block.
		If a number is possible just in a cell so that cell is solved.

		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].

		@throw out_of_range_input
	*/
	bool one_possibility_block(int x, int y){

		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;

		if( !(flag_x && flag_y) ){
			throw out_of_range_input();
		}

		int x_block = (int) floor(x / 3); 
		int y_block = (int) floor(y / 3);

		int x_block_position = x_block * 3; //x_block_starting_position
		int y_block_position = y_block * 3; //y_block_starting_position

		int real_values[9];
		std::list<int>::iterator it;
		for(int i=0; i < 9; i++){
			real_values[i] = 0;
		}

		//Scan to search real values
		for(int i=x_block_position; i <= ( x_block_position + 2 ); i++){
			for(int j=y_block_position; j <= ( y_block_position + 2 ); j++){
				if( !table[i][j].flag ){
					for (it = table[i][j].possible_values->begin(); it != table[i][j].possible_values->end(); ++it){
						real_values[(*it-1)]++;
					}
				}
			}
		}

		//Scan to search one possibility
		std::list <int> values;
		for(int i=0; i < 9; i++){
			if( real_values[i] == 1 ){
				values.push_back( i + 1 );
			}
		}


		//Set values.
		bool flag = false;
		for (it = values.begin(); it != values.end(); ++it){
			for(int i=x_block_position; i <= ( x_block_position + 2 ); i++){
				for(int j=y_block_position; j <= ( y_block_position + 2 ); j++){
					if( !table[i][j].flag ){
						if( is_value_in_list(*it, table[i][j].possible_values) ){
							if( is_Valid_place(*it, i+1, j+1) ){
								table[i][j] = *it;
								flag = true;
								changed = true;
                                #ifdef DEBUGGING
								std::cout << "block " << i+1 <<","<< j+1 << ", value " << *it << std::endl;
                                #endif //DEBUGGING
                            }
						}
					}
				}
			}
		}
		
		return flag;
	}

	/**
		@brief Backtracking bruteforse algorithm.

		@param row row-coordinate, in [0,8].
		@param col col-coordinate, in [0,8].
	*/
	bool brute_force_temp(int row, int col){
		// End
		if (row == 8 && col == 9)
			return true;

		// Move to the next row
		if (col == 9) {
			row++;
			col = 0;
		}
	
		if ( table[row][col].flag )
			return brute_force_temp(row, col + 1);

		std::list<int>::iterator it;
		if( !table[row][col].flag ){
			for (it = table[row][col].possible_values->begin(); it != table[row][col].possible_values->end(); ++it){ 
				if( is_Valid_place(*it, row+1, col+1) ){
					table[row][col].value = *it;
					table[row][col].flag = true;
				
					if ( brute_force_temp(row, col + 1) )
						return true;
				}
			}
		
			// Removing the assigned value
			table[row][col].value = -1; 
			table[row][col].flag = false;
		}
		return false;
	}


public:

	/**
		@brief Default constructor.

		Default constructor that instantiates an empty Sudoku table.
	*/
	Sudoku(): changed(false) {
		/*
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				table[i][j] = new Cell();
			}
		}
		*/
	}
	
	/**
		@brief Destroyer.

		Removes the allocated memory from the Sudoku table.
	*/
	~Sudoku(){
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				delete table[i][j].possible_values;
			}
		}
	}

	/**
		@brief Default constructor.

		Default constructor that instantiates an empty Sudoku table.
	*/
	Sudoku(const Sudoku *s): changed(false) {
		if( s == nullptr ){
			throw my_uninitialized_value();
		}

		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				this->table[i][j] = s->table[i][j];
			}
		}
	}

    /**
		@brief Default constructor.

		Default constructor that instantiates an empty Sudoku table.
	*/
	Sudoku(const Sudoku &s): changed(false) {

		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				this->table[i][j] = s.table[i][j];
			}
		}
	}

    /**
        @brief Assignment operator

        Redefine operator =.
        Allows copying between Sudoku.

        @param other Sudoku to copy
    */
    Sudoku& operator=(const Sudoku &other){	
        this->changed = other.changed;

        for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				this->table[i][j] = other.table[i][j];
			}
		}

        return *this;
    }

    /**
        @brief Assignment operator

        Redefine operator =.
        Allows copying between Sudoku.

        @param other Sudoku to copy
    */
    Sudoku& operator=(const Sudoku *other){	
        if( other == nullptr ){
			throw my_uninitialized_value();
		}

        this->changed = other->changed;

        for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				this->table[i][j] = other->table[i][j];
			}
		}

        return *this;
    }

	/**
		@brief Set 'value' in Cell (x,y) if possible.

		If 'value', 'x' and 'y' are in [1,9], set 'value' in Cell (x,y).
		If 'x' or 'y' not in [1,9] throw out_of_range_input.
		If 'value' not in [1,9] throw bad_input.

		@param value [int] Value, in [1,9].
		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].
	*/
	bool set(int value, int x, int y){

		if( is_Valid_place(value, x, y) ){
			table[x-1][y-1].value = value;
			table[x-1][y-1].flag = true;
			this->changed = true;

            //The possible values now are just 'value'
			delete table[x-1][y-1].possible_values;
            table[x-1][y-1].possible_values = new std::list <int> (1, value);

			return true;
		}
		else{
			return false;
		}
	}

	/**
		@brief Check if the Sudoku table is changed.
		True if the Sudoku table was changed since the last check.

	*/
	bool is_changed(){
		bool r = this->changed;

		if( this->changed ){
			this->changed = false;
		}

		return r;
	}

	/**
		@brief Check if the Sudoku table is solved.
		True if the Sudoku table is solved, False otherwise.

	*/
	bool is_solved(){
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if( !table[i][j].flag ){
					return false;
				}
			}
		}
		return true;
	}

	/**
		@brief Get value in Cell (x,y) if possible.

		If 'x' or 'y' not in [1,9] throw out_of_range_input().


		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].

		@throw out_of_range_input
	*/
	int get(int x, int y){
		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;
		if( flag_x && flag_y ){
			return table[x-1][y-1].value;
		}
		else{
			throw out_of_range_input();
		}
	}

	/**
		@brief Get flag in Cell (x,y) if possible.

		If 'x' or 'y' not in [1,9] throw out_of_range_input().


		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].

		@throw out_of_range_input
	*/
	bool get_flag(int x, int y){
		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;
		if( flag_x && flag_y ){
			return table[x-1][y-1].flag;
		}
		else{
			throw out_of_range_input();
		}
	}

	/**
		@brief Get possible values in Cell (x,y) if possible.

		If 'x' or 'y' not in [1,9] throw out_of_range_input().


		@param x x-coordinate, in [1,9].
		@param y y-coordinate, in [1,9].

		@throw out_of_range_input
	*/
	std::list<int> get_possible_values(int x, int y){
		bool flag_x = x >= 1 && x <= 9;
		bool flag_y = y >= 1 && y <= 9;
		if( flag_x && flag_y ){
			std::list<int> possible_values( *table[x-1][y-1].possible_values );
			return possible_values;
		}
		else{
			throw out_of_range_input();
		}
	}

	/**
		@brief Simplify.
		Remove the impossible value from every Cells in the Sudoku table.
		If a possible_value is removed return 'true'.
		It can solve the simplest sudoku puzzles.
	*/
	bool simplify(){
		bool flag_r = false; //To check if table is modified.
		bool flag = false;
		bool row = false;
		bool col = false;
		bool block = false;
		do{
			flag = false;
			for(int i=1; i <= 9; i++){
				row = simplify_row(i);
				col = simplify_col(i);
				
				if( row || col ){
					flag = true;
					flag_r = true;
				}				
			}

			for(int i=1; i <= 9; i+=3){
				for(int j=1; j <= 9; j+=3){
					block = simplify_block(i,j);

					if( block ){
						flag = true;
						flag_r = true;
					}
				}		
			}

		} while( flag );
		
		return flag_r;
	}

	/**
		@brief Search for number that appear one time in the column, row or block.
		If a number is possible just in a cell so that cell is solved.
	*/
	bool one_possibility(){

		bool flag_r = false; //To check if table is modified.
		bool flag = false;
		bool row = false;
		bool col = false;
		bool block = false;
		do{
			flag = false;
			for(int i=1; i <= 9; i++){
				row = one_possibility_row(i);
				col = one_possibility_col(i);
				
				if( row || col ){
					flag = true;
					flag_r = true;
				}				
			}

			for(int i=1; i <= 9; i+=3){
				for(int j=1; j <= 9; j+=3){
					block = one_possibility_block(i,j);

					if( block ){
						flag = true;
						flag_r = true;
					}
				}		
			}

		} while( flag );
		
		return flag_r;
	}

	/**
		@brief Simplify the Sudoku.
		Sometimes, if the sudoku is simple it can solve it.
		If he modified the Sudoku table return 'true', else 'false'.
	*/
	bool pre_solver(){
		if( this->is_solved() ){
			return false;
		}

		bool flag = false; //To check if table is modified.
		bool flag_simplify = false;
		bool flag_one_possibility = false;
		size_t n_loop = 0;


		do{	
			simplify();
			one_possibility();
			n_loop++;

		} while( this->is_changed() );

		if( n_loop > 1 ){
			flag = true;
		}

		return flag;
	}

	/**
		@brief Backtracking bruteforse algorithm.
		Return 'true' if a solution is founded.
	*/
	bool brute_force(){
		return brute_force_temp(0, 0);
	}

	/**
		@brief Solve the Sudoku.
		Return 'true' if it solve the sudoku.
	*/
	bool solve(){
		if( this->is_solved() ){
			return true;
		}

		this->pre_solver();

		if( this->is_solved() ){
			return true;
		}

		//Brute force
		brute_force();

		return this->is_solved();
	}

	/**
		@brief Print possible value of each Sudoku cell.
	*/
	void print_possible_values(){
		for(int i=0; i< 9; i++){
			for(int j=0; j< 9; j++){
				if ( ! table[j][i].flag ){
					std::cout << "(" ;
					for (auto const &i: *table[j][i].possible_values) {
						std::cout << i ;
						std::cout << "," ;
					}
					std::cout << ")|" ;
				}
				else{
					std::cout << table[j][i].value;
					std::cout << "|" ;
				}
				
				if( (j+1) % 3 == 0 ){
					std::cout << "|" ;
				}
			}

			if( (i+1) % 3 == 0 ){
				std::cout << "\n";
				std::cout << "--------------------" ;
			}
			std::cout << "\n";
		}
	}

};


/**
	@brief Print value of each Sudoku cell.
	If a cell is unknown a '?' is printed.
	Usefull to print with std::cout or std::ostream in general.
*/
std::ostream & operator << (std::ostream &out, Sudoku &srt) {

	for(int i=1; i<=9; i++){
		for(int j=1; j<=9; j++){
			if ( srt.get_flag(j,i) ){
				out <<  srt.get(j,i);
				out << "|" ;
			}
			else{
				out << "?|";
			}
			
			if( j % 3 == 0 ){
				out << " " ;
			}
		}

		if( i % 3 == 0 ){
			out << "\n";
			out << "--------------------" ;
		}
		
		out << "\n";
	}

	return out;
}

/**
	@brief Print value of each Sudoku cell.
	If a cell is unknown a '?' is printed.
	Usefull to print with std::cout or std::ostream in general.
*/
std::ostream & operator << (std::ostream &out, Sudoku *srt) {

	for(int i=1; i<=9; i++){
		for(int j=1; j<=9; j++){
			if ( srt->get_flag(j,i) ){
				out <<  srt->get(j,i);
				out << "|" ;
			}
			else{
				out << "?|";
			}
			
			if( j % 3 == 0 ){
				out << " " ;
			}
		}

		if( i % 3 == 0 ){
			out << "\n";
			out << "--------------------" ;
		}
		
		out << "\n";
	}

	return out;
}


#endif //SUDOKU_HPP
