#include "Sudoku.hpp"
#include <iostream>
#include <cassert>
#include <chrono>
#include <bits/stdc++.h>

//#define DEBUGGING

/**
	@file main.cpp
	@brief File di testing per la classe templata Queue
*/

bool test_tutorial(){
	std::cout << "Test tutorial" << std::endl;
	Sudoku s;
	std::cout << "END Test tutorial" << std::endl;
	return true;
}

bool test_constructor(){
	std::cout << "Test constructor" << std::endl;
	bool flag;

	//Test Sudoku()
	Sudoku n; 

	assert( !n.is_changed() );

	//Check if empty
	for(int i=1; i<=9; i++){
		for(int j=1; j<=9; j++){
			assert( n.get(i,j) == -1 );
		}
	} 
	
	assert( !n.is_changed() );

	//Check 'get'
	flag = false;
	try{
		n.get(10,8);
	}
	catch(out_of_range_input e){
		#ifdef DEBUGGING
		std::cerr << e.what() << std::endl;
		#endif
		flag = true;
	}
	assert ( flag );

	flag = false;
	try{
		n.get(4,-2);
	}
	catch(out_of_range_input e){
		#ifdef DEBUGGING
		std::cerr << e.what() << std::endl;
		#endif
		flag = true;
	}
	assert ( flag );

	flag = false;
	try{
		n.get(0,1);
	}
	catch(out_of_range_input e){
		#ifdef DEBUGGING
		std::cerr << e.what() << std::endl;
		#endif
		flag = true;
	}
	assert ( flag );
	
	
	//Check 'set'
	assert( n.set(3, 1, 1) );
	
	assert( n.get(1,1) == 3 );
	assert( n.get_possible_values(1, 1).size() == 1 );
	assert( n.get_possible_values(3, 5).size() != 1 );

	//Check 'get_flag'
	assert( n.get_flag(1,1) );
	assert( !n.get_flag(3,5) );

	flag = false;
	try{
		n.get_flag(10,8);
	}
	catch(out_of_range_input e){
		#ifdef DEBUGGING
		std::cerr << e.what() << std::endl;
		#endif
		flag = true;
	}
	assert ( flag );


	assert( n.is_changed() );
	assert( !n.is_changed() );

	//Row
	assert( !n.set(3, 1, 2) );
	assert( !n.is_changed() );
	//Col
	assert( !n.set(3, 2, 1) );
	//Block
	assert( !n.set(3, 3, 3) );
	//Same item
	assert( n.set(3, 1, 1) );

	assert( n.set(8, 5, 5) );
	//Block
	assert( !n.set(8, 6, 6) );

	assert( n.set(1, 7, 1) );
	//Block
	assert( !n.set(1, 8, 2) );

	Sudoku n_hard;
	
	assert( n_hard.set(2,1,1) );
	assert( n_hard.set(7,3,3) );

	assert( n_hard.set(5,5,1) );
	assert( n_hard.set(3,4,2) );
	assert( n_hard.set(7,6,2) );
	assert( n_hard.set(6,5,3) );

	assert( n_hard.set(7,7,1) );
	assert( n_hard.set(8,8,2) );
	assert( n_hard.set(4,9,3) );

	assert( n_hard.set(4,1,5) );
	assert( n_hard.set(3,2,4) );
	assert( n_hard.set(2,2,6) );
	assert( n_hard.set(5,3,5) );

	assert( n_hard.set(1,6,4) );
	assert( n_hard.set(4,4,6) );

	assert( n_hard.set(3,7,5) );
	assert( n_hard.set(5,8,4) );
	assert( n_hard.set(9,8,6) );
	assert( n_hard.set(2,9,5) );

	assert( n_hard.set(8,1,7) );
	assert( n_hard.set(7,2,8) );
	assert( n_hard.set(3,3,9) );

	assert( n_hard.set(4,5,7) );
	assert( n_hard.set(6,4,8) );
	assert( n_hard.set(9,6,8) );
	assert( n_hard.set(8,5,9) );

	assert( n_hard.set(9,7,7) );
	assert( n_hard.set(1,9,9) );

	
	// Sudoku(const Sudoku &s) 
	Sudoku n_copy(n_hard);
	
	assert( n_hard.is_changed() );
	assert( !n_copy.is_changed() );

	assert( n_copy.get(1,7) == 8 );
	assert( n_copy.get(1,5) == 4 );
	assert( n_copy.get(6,2) == 7 );

	Sudoku* n_p = new Sudoku(n_hard);

	assert( n_p->get(1,7) == 8 );
	assert( n_p->get(1,5) == 4 );
	assert( n_p->get(6,2) == 7 );

	// Sudoku(const Sudoku *s) 
	Sudoku n_p_copy(n_p);
	assert( n_p_copy.get(1,7) == 8 );
	assert( n_p_copy.get(1,5) == 4 );
	assert( n_p_copy.get(6,2) == 7 );

	assert( !n_p_copy.is_solved() );
	
	//Sudoku& operator=(const Sudoku &other)
	Sudoku n1 = n_p_copy;

	assert( n1.get(1,7) == 8 );
	assert( n1.get(1,5) == 4 );
	assert( n1.get(6,2) == 7 );

	//Sudoku& operator=(const Sudoku *other)
	Sudoku n2 = n_p;

	assert( n2.get(1,7) == 8 );
	assert( n2.get(1,5) == 4 );
	assert( n2.get(6,2) == 7 );

	delete n_p;

	assert( n_p_copy.get(1,7) == 8 );
	assert( n_p_copy.get(1,5) == 4 );
	assert( n_p_copy.get(6,2) == 7 );

	std::cout << "END Test constructor" << std::endl;
	return true;
}

bool test_display(){
	std::cout << "Test display" << std::endl;

	Sudoku n;
	
	assert( n.set(3,1,1) );
	assert( n.set(1,3,2) );
	assert( n.set(5,5,2) );
	assert( n.set(9,3,8) );

	std::cout << "Sudoku:" << std::endl;
	std::cout << n << std::endl;


	Sudoku *n_p = new Sudoku();
	assert( n_p->set(3,1,1) );
	assert( n_p->set(1,3,2) );
	assert( n_p->set(5,5,2) );
	assert( n_p->set(9,3,8) );

	std::cout << "Sudoku p:" << std::endl;
	std::cout << n_p << std::endl;

	n_p->print_possible_values();
	delete n_p;

	std::cout << "END Test display" << std::endl;
	return true;

}

bool test_simple(){
	std::cout << "Test simple" << std::endl;

	Sudoku n;
	
	assert( n.set(4,1,1) );	
	assert( n.set(9,2,1) );
	assert( n.set(3,1,2) );
	assert( n.set(7,3,3) );

	assert( n.set(5,6,1) );
	assert( n.set(1,6,2) );
	assert( n.set(4,4,3) );
	
	assert( n.set(3,8,1) );
	assert( n.set(1,9,1) );
	assert( n.set(7,8,2) );
	assert( n.set(8,9,2) );

	assert( n.set(9,3,4) );
	assert( n.set(6,1,6) );
	assert( n.set(8,2,6) );

	assert( n.set(8,5,4) );
	assert( n.set(6,6,4) );
	assert( n.set(9,4,5) );
	assert( n.set(3,5,5) );
	assert( n.set(4,6,5) );
	assert( n.set(5,4,6) );
	assert( n.set(1,5,6) );

	assert( n.set(2,8,4) );
	assert( n.set(4,9,4) );
	assert( n.set(7,7,6) );
	
	assert( n.set(7,1,8) );
	assert( n.set(4,2,8) );
	assert( n.set(9,1,9) );
	assert( n.set(5,2,9) );

	assert( n.set(7,6,7) );
	assert( n.set(1,4,8) );
	assert( n.set(2,4,9) );

	assert( n.set(9,7,7) );
	assert( n.set(5,9,8) );
	assert( n.set(1,8,9) );
	assert( n.set(7,9,9) );
	
	std::cout << n << std::endl;
	std::cout << "Starting sudoku" << std::endl;
	//n.print_possible_values();

	n.simplify();
	std::cout << "Solved sudoku" << std::endl;
	n.print_possible_values();
	assert( n.is_solved() );

	std::cout << "END Test simple" << std::endl;
	return true;
}

bool test_one_possibility(){
	std::cout << "Test one possibility" << std::endl;

	Sudoku n;
	
	assert( n.set(2,1,1) );
	assert( n.set(7,3,3) );

	assert( n.set(5,5,1) );
	assert( n.set(3,4,2) );
	assert( n.set(7,6,2) );
	assert( n.set(6,5,3) );

	assert( n.set(7,7,1) );
	assert( n.set(8,8,2) );
	assert( n.set(4,9,3) );

	assert( n.set(4,1,5) );
	assert( n.set(3,2,4) );
	assert( n.set(2,2,6) );
	assert( n.set(5,3,5) );

	assert( n.set(1,6,4) );
	assert( n.set(4,4,6) );

	assert( n.set(3,7,5) );
	assert( n.set(5,8,4) );
	assert( n.set(9,8,6) );
	assert( n.set(2,9,5) );

	assert( n.set(8,1,7) );
	assert( n.set(7,2,8) );
	assert( n.set(3,3,9) );

	assert( n.set(4,5,7) );
	assert( n.set(6,4,8) );
	assert( n.set(9,6,8) );
	assert( n.set(8,5,9) );

	assert( n.set(9,7,7) );
	assert( n.set(1,9,9) );

	std::cout << n << std::endl;
	std::cout << "Starting sudoku" << std::endl;
	//n.print_possible_values();

	n.simplify();
	std::cout << "Solved simplified" << std::endl;
	n.print_possible_values();
	assert( !n.is_solved() );

	n.one_possibility();
	std::cout << "Solved one possibility" << std::endl;
	n.print_possible_values();
	assert( !n.is_solved() );

	n.pre_solver();
	std::cout << "Solved solver" << std::endl;
	n.print_possible_values();
	assert( !n.is_solved() );

	std::cout << "END Test one possibility" << std::endl;
	return true;
}

bool test_hard(){
	std::cout << "Test hard" << std::endl;

	Sudoku n;
	
	assert( n.set(2,1,1) );
	assert( n.set(7,3,3) );

	assert( n.set(5,5,1) );
	assert( n.set(3,4,2) );
	assert( n.set(7,6,2) );
	assert( n.set(6,5,3) );

	assert( n.set(7,7,1) );
	assert( n.set(8,8,2) );
	assert( n.set(4,9,3) );

	assert( n.set(4,1,5) );
	assert( n.set(3,2,4) );
	assert( n.set(2,2,6) );
	assert( n.set(5,3,5) );

	assert( n.set(1,6,4) );
	assert( n.set(4,4,6) );

	assert( n.set(3,7,5) );
	assert( n.set(5,8,4) );
	assert( n.set(9,8,6) );
	assert( n.set(2,9,5) );

	assert( n.set(8,1,7) );
	assert( n.set(7,2,8) );
	assert( n.set(3,3,9) );

	assert( n.set(4,5,7) );
	assert( n.set(6,4,8) );
	assert( n.set(9,6,8) );
	assert( n.set(8,5,9) );

	assert( n.set(9,7,7) );
	assert( n.set(1,9,9) );

	std::cout << n << std::endl;
	std::cout << "Starting sudoku" << std::endl;
	//n.print_possible_values();

	n.simplify();
	std::cout << "Solved simplified" << std::endl;
	n.print_possible_values();
	assert( !n.is_solved() );

	n.one_possibility();
	std::cout << "Solved one possibility" << std::endl;
	n.print_possible_values();

	n.solve();
	std::cout << "Solved solver" << std::endl;
	n.print_possible_values();
	assert( n.is_solved() );

	std::cout << "END Test hard" << std::endl;
	return true;
}

bool speed_test(){
	std::cout << "Speed Test" << std::endl;

	Sudoku n;
	
	assert( n.set(2,1,1) );
	assert( n.set(7,3,3) );

	assert( n.set(5,5,1) );
	assert( n.set(3,4,2) );
	assert( n.set(7,6,2) );
	assert( n.set(6,5,3) );

	assert( n.set(7,7,1) );
	assert( n.set(8,8,2) );
	assert( n.set(4,9,3) );

	assert( n.set(4,1,5) );
	assert( n.set(3,2,4) );
	assert( n.set(2,2,6) );
	assert( n.set(5,3,5) );

	assert( n.set(1,6,4) );
	assert( n.set(4,4,6) );

	assert( n.set(3,7,5) );
	assert( n.set(5,8,4) );
	assert( n.set(9,8,6) );
	assert( n.set(2,9,5) );

	assert( n.set(8,1,7) );
	assert( n.set(7,2,8) );
	assert( n.set(3,3,9) );

	assert( n.set(4,5,7) );
	assert( n.set(6,4,8) );
	assert( n.set(9,6,8) );
	assert( n.set(8,5,9) );

	assert( n.set(9,7,7) );
	assert( n.set(1,9,9) );

	std::cout << "Starting sudoku" << std::endl << std::endl;
	
	auto n_start = std::chrono::high_resolution_clock::now();
	n.solve();
	assert( n.is_solved() );
	auto n_finish = std::chrono::high_resolution_clock::now();
	double n_time = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(n_finish - n_start).count();

	n_time *= 1e-9;

	std::cout << "Hard problem solved with solve() in " << n_time << std::setprecision(9) << std::endl;

	Sudoku n_copy(n);

	auto n_copy_start = std::chrono::high_resolution_clock::now();
	n_copy.brute_force();
	assert( n_copy.is_solved() );
	auto n_copy_finish = std::chrono::high_resolution_clock::now();
	double n_copy_time = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(n_copy_finish - n_copy_start).count();

	n_copy_time *= 1e-9;

	std::cout << "Hard problem solved with brute_force() in " << n_copy_time << std::setprecision(9) << std::endl << std::endl;

	Sudoku n2;
	
	assert( n2.set(4,1,1) );	
	assert( n2.set(9,2,1) );
	assert( n2.set(3,1,2) );
	assert( n2.set(7,3,3) );

	assert( n2.set(5,6,1) );
	assert( n2.set(1,6,2) );
	assert( n2.set(4,4,3) );
	
	assert( n2.set(3,8,1) );
	assert( n2.set(1,9,1) );
	assert( n2.set(7,8,2) );
	assert( n2.set(8,9,2) );

	assert( n2.set(9,3,4) );
	assert( n2.set(6,1,6) );
	assert( n2.set(8,2,6) );

	assert( n2.set(8,5,4) );
	assert( n2.set(6,6,4) );
	assert( n2.set(9,4,5) );
	assert( n2.set(3,5,5) );
	assert( n2.set(4,6,5) );
	assert( n2.set(5,4,6) );
	assert( n2.set(1,5,6) );

	assert( n2.set(2,8,4) );
	assert( n2.set(4,9,4) );
	assert( n2.set(7,7,6) );
	
	assert( n2.set(7,1,8) );
	assert( n2.set(4,2,8) );
	assert( n2.set(9,1,9) );
	assert( n2.set(5,2,9) );

	assert( n2.set(7,6,7) );
	assert( n2.set(1,4,8) );
	assert( n2.set(2,4,9) );

	assert( n2.set(9,7,7) );
	assert( n2.set(5,9,8) );
	assert( n2.set(1,8,9) );
	assert( n2.set(7,9,9) );


	Sudoku n2_copy(n2);

	auto n2_start = std::chrono::high_resolution_clock::now();
	n2.solve();
	assert( n2.is_solved() );
	auto n2_finish = std::chrono::high_resolution_clock::now();
	double n2_time = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(n2_finish - n2_start).count();

	n2_time *= 1e-9;

	std::cout << "Easy problem solved with solve() in " << n2_time << std::setprecision(9) << std::endl;

	auto n2_copy_start = std::chrono::high_resolution_clock::now();
	n2_copy.brute_force();
	assert( n2_copy.is_solved() );
	auto n2_copy_finish = std::chrono::high_resolution_clock::now();
	double n2_copy_time = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(n2_copy_finish - n2_copy_start).count();

	n2_copy_time *= 1e-9;

	std::cout << "Easy problem solved with brute_force() in " << n2_copy_time << std::setprecision(9) << std::endl << std::endl;


	std::cout << "END Speed Test" << std::endl;
	return true;
}

bool test(){

	assert( test_tutorial() );
	assert( test_constructor() );
	assert( test_display() );
	assert( test_simple() );
	assert( test_one_possibility() );
	assert( test_hard() );
	assert( speed_test() );

	return true;
}

int main(){

	assert( test() );

}	
