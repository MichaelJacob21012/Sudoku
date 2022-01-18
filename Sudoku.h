#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include "SudokuSquare.h"
#include <cmath>
#include <iostream>
using std::cout;

class Sudoku : public Searchable{
private:
	int size;
	vector<vector<SudokuSquareSet>> grid;
public:
	Sudoku(int sizeIn)
:grid(sizeIn), size(sizeIn){
		for (auto& row : grid){
			row.resize(size);
			for (auto& cell : row){
				for (int i = 1; i < size+1; ++i){
					cell.insert(i);
				}
			}
		}
	}

	int getSquare(int row, int col) const{
		const SudokuSquareSet* cell = &grid[row][col];
		if (cell->size() == 1){
			auto itr = cell->begin();
			return *itr;
		}
		return -1;
	}

	bool setSquare(int row, int col, int value){
		SudokuSquareSet* cell = &grid[row][col];
		vector <int> check = vector<int>();
		SudokuSquareSet mySet = grid[row][col];
		for (auto number : mySet){
			if (number != value){
				check.push_back(number);
			}
		}
		cell->clear();
		cell->insert(value);
		for (auto number : check){
			if (!setValueAll(row, col, number)){
				return false;
			}
		}


		for (int i = 0; i < size; ++i){
			if (i != col){
				SudokuSquareSet*  current = &grid[row][i];
				int erased = current->erase(value);
				if (current->size() == 0){
					return false;
				}
				if (erased !=0){
					if (current->size() == 1){
						auto itr = current->begin();
						if (!setSquare(row,i,*itr)){
							return false;

						}
					}
					else {
						if (!sameSet(row,i)){
							return false;
						}
					}

					if (!setValueNotRow(row, i, value)){
						return false;
					}

				}
			}
		}

		for (int i = 0; i < size; ++i){
			if (i != row){
				SudokuSquareSet*  current = &grid[i][col];
				int erased = current->erase(value);
				if (current->size() == 0){
					return false;
				}
				if (erased !=0){
					if (current->size()== 1){
						auto itr = current->begin();
						if (!setSquare(i,col,*itr)){
							return false;

						}
					}
					else {
						if (!sameSet(i,col)){
							return false;
						}
					}
					if (!setValueNotCol(i, col, value)){
						return false;
					}
				}
			}
		}


		for (int j = boxInit(row); j < boxFin(row); ++j){
			for (int k = boxInit(col); k < boxFin(col); ++k){

				if (!((j == row) && (k == col))){
					SudokuSquareSet* current = &grid[j][k];

					int erased = current->erase(value);
					if (current->size() == 0){
						return false;
					}

					if (erased !=0){
						if (current->size() == 1){
							auto itr = current->begin();
							if (!setSquare(j,k,*itr)){
								return false;
							}
						}
						else {
							if (!sameSet(j,k)){
								return false;
							}
						}

						if (!setValueNotBox(j, k, value)){
							return false;
						}

					}
				}
			}
		}
		return true;
	}
	bool setValueRow(int row, int value){
		int count = 0;
		int column = 0;
		for (int i = 0; i < size; ++i){
			SudokuSquareSet*  current = &grid[row][i];
			auto itr = current->find(value);
			if (itr != current->end()){
				count++;
				column = i;
			}
		}
		if (count == 1){
			SudokuSquareSet current = grid[row][column];
			int erased = 0;
			for (auto number : current){
				if (number != value){
					erased += current.erase(number);
				}
			}
			if (current.size() == 0){
				return false;
			}
			if (erased != 0){
				if (current.size() == 1){
					if (!setSquare(row,column,value)){
						return false;
					}
				}
				else{
					if (!sameSet(row, column)){
						return false;
					}
				}
			}

		}
		return true;
	}
	bool setValueCol(int col, int value){
		int count = 0;
		int r = 0;
		for (int i = 0; i < size; ++i){
			SudokuSquareSet*  current = &grid[i][col];
			auto itr = current->find(value);
			if (itr != current->end()){
				count++;
				r = i;
			}
		}
		if (count == 1){
			SudokuSquareSet current = grid[r][col];
			int erased = 0;
			for (auto number : current){
				if (number != value){
					erased += current.erase(number);
				}
			}
			if (current.size() == 0){
				return false;
			}
			if (erased != 0){
				if (current.size() == 1){

					if (!setSquare(r,col,value)){
						return false;
					}
				}
				else{
					if (!sameSet(r, col)){
						return false;
					}
				}
			}
		}
		return true;
	}
	bool setValueBox(int row, int col, int value){
		int count = 0;
		int column = 0;
		int r = 0;
		for (int j = boxInit(row); j < boxFin(row); ++j){
			for (int k = boxInit(col); k < boxFin(col); ++k){
				SudokuSquareSet* current = &grid[j][k];
				auto itr = current->find(value);
				if (itr != current->end()){
					count++;
					r = j;
					column = k;
				}
			}
		}
		if (count == 1){
			SudokuSquareSet current = grid[r][column];
			int erased = 0;
			for (auto number : current){
				if (number != value){
					erased += current.erase(number);
				}
			}
			if (current.size() == 0){
				return false;
			}
			if (erased != 0){
				if (current.size() == 1){
					if (!setSquare(r,column,value)){
						return false;
					}
				}
				else {
					if (!sameSet(r,column)){
						return false;
					}
				}
			}

		}
		return true;
	}
	bool setValueAll(int row, int col, int value){
		if(!setValueRow(row, value)){
			return false;
		}
		if(!setValueCol(col, value)){
			return false;
		}
		if(!setValueBox(row, col, value)){
			return false;
		}
		return true;
	}

	bool setValueNotRow(int row, int col, int value){
		if(!setValueCol(col, value)){
			return false;
		}
		if(!setValueBox(row, col, value)){
			return false;
		}
		return true;
	}
	bool setValueNotCol(int row, int col, int value){
		if(!setValueRow(row, value)){
			return false;
		}
		if(!setValueBox(row, col, value)){
			return false;
		}
		return true;
	}
	bool setValueNotBox(int row, int col, int value){
		if(!setValueRow(row, value)){
			return false;
		}
		if(!setValueCol(col, value)){
			return false;
		}
		return true;
	}

	int boxInit(int place){
		int root = sqrt(size);
		for (int i = size; i > -1; i -= root){
			if (place >= i){
				return i;
			}
		}
		return -1;
	}

	int boxFin(int place){
		int root = sqrt(size);
		for (int i = 0; i <= size; i += root){
			if (place < i){
				return i;
			}
		}
		return -1;
	}
	virtual bool isSolution() const override{
		for (auto row : grid){
			for (auto cell : row){
				if (cell.size() != 1){
					return false;
				}
			}
		}
		return true;
	}
	virtual void write(ostream & o) const override{
		for (auto row : grid){
			for (auto cell : row){
				for (auto h : cell){
					o << h;
				}
				o << " ";
			}
			o << "\n";
		}
	}
	virtual int heuristicValue() const override{
		int count = 0;
		for (auto row : grid){
			for (auto cell : row){
				if (cell.size() > 1){
					count++;
				}
			}
		}
		return count;
	}
	virtual vector<unique_ptr<Searchable>> successors() const override{
		vector<unique_ptr<Searchable>> successor;
		int row = 0;
		int col = 0;
		bool found = false;
		for (int i = 0; i < size; ++i){
			for (int j = 0; j < size; ++j){
				if (grid[i][j].size() != 1){
					row = i;
					col = j;
					found = true;
					break;
				}
			}
			if (found){
				break;
			}
		}
		const SudokuSquareSet cell = grid[row][col];
		for (auto opt : cell){
			Sudoku* ptr = new Sudoku(0);
			*ptr = *this;
			unique_ptr<Sudoku> uni(ptr);
			if (uni->setSquare(row,col,opt)){
				successor.push_back(std::move(uni));
			}

		}
		if (successor.size() == 1 && !(successor[0]->isSolution())){
			successor = successor[0]->successors();
		}
		return successor;
	}
	bool sameSet(int row, int col){
		SudokuSquareSet  values = grid[row][col];
		int count = 0;
		for (int i = 0; i < size; ++i){
			SudokuSquareSet  current = grid[row][i];
			if (current == values){
				count++;
			}
		}
		if (count == values.size()){
			for (auto number : values){
				for (int i = 0; i < size; ++i){
					SudokuSquareSet  current = grid[row][i];
					if (current != values){
						int erased = current.erase(number);
						if (current.size() == 0){
							return false;
						}
						if (erased != 0){
							if (!setValueNotRow(row,i,number)){
								return false;
							}
						}
					}
				}
			}
		}

		count = 0;
		for (int i = 0; i < size; ++i){
			SudokuSquareSet  current = grid[i][col];
			if (current == values){
				count++;
			}
		}

		if (count == values.size()){
			for (auto number : values){
				for (int i = 0; i < size; ++i){
					SudokuSquareSet  current = grid[i][col];
					if (current != values){
						int erased = current.erase(number);
						if (current.size() == 0){
							return false;
						}
						if (erased != 0){
							if (!setValueNotCol(i,col,number)){
								return false;
							}
						}
					}
				}
			}
		}
		count = 0;
		for (int j = boxInit(row); j < boxFin(row); ++j){
			for (int k = boxInit(col); k < boxFin(col); ++k){
				SudokuSquareSet current = grid[j][k];
				if (current == values){
					count++;
				}
			}
		}
		if (count == values.size()){
			for (auto number : values){
				for (int j = boxInit(row); j < boxFin(row); ++j){
					for (int k = boxInit(col); k < boxFin(col); ++k){
						SudokuSquareSet  current = grid[j][k];
						if (current != values){
							int erased = current.erase(number);
							if (current.size() == 0){
								return false;
							}
							if (erased != 0){
								if (!setValueNotBox(j,k,number)){
									return false;
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
};





#endif
