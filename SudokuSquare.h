#ifndef SUDOKUSQUARE_H
#define SUDOKUSQUARE_H

// Do not add any #include statements to this file
class SudokuSquareSet {

private :
	unsigned int values;
	int numberOfValues;
public :
	SudokuSquareSet()
:values(0),numberOfValues(0){

	}
	unsigned int getValues() const{
		return values;
	}
	int size() const{
		return numberOfValues;
	}
	bool empty() const{
		if (numberOfValues == 0){
			return true;
		}
		return false;
	}
	void clear(){
		values = 0;
		numberOfValues = 0;
	}
	bool operator== (const SudokuSquareSet &other) const{
		if (values == other.values){
			return true;
		}
		return false;
	}
	bool operator!= (const SudokuSquareSet &other) const{
		if (values != other.values){
			return true;
		}
		return false;
	}
	class SudokuSquareSetIterator{
	private:
		const SudokuSquareSet* set;
		int currentBit;

	public:

		SudokuSquareSetIterator(int currentIn, const SudokuSquareSet* setIn)
	: currentBit(currentIn), set(setIn) {
		}

		int operator*() const{
			return currentBit + 1;
		}

		void operator++(){
			unsigned int x = 1;
			x <<= currentBit + 1;
			int count = currentBit + 1;
			while (count < 32){
				currentBit++;
				if (set->getValues() & x){
					return;
				}
				x = x*2;
				count ++;
			}
			currentBit = -1;
		}

		bool operator==(const SudokuSquareSetIterator other) const{
			if (currentBit == (other.currentBit)){
				return true;
			}
			return false;
		}

		bool operator!=(const SudokuSquareSetIterator other) const{
			if (currentBit == (other.currentBit)){
				return false;
			}
			return true;
		}

	};
	SudokuSquareSetIterator begin() const{
		if (empty()){
			return end();
		}
		unsigned int x = 1;
		int count = 0;
		while (count < 32){
			if (values & x){
				return SudokuSquareSetIterator(count,this);
			}
			x = x*2;
			count ++;
		}
		return SudokuSquareSetIterator(0,this);
	}
	SudokuSquareSetIterator end() const {
		return SudokuSquareSetIterator(-1,this);
	}
	SudokuSquareSetIterator insert(int val){
		if (find(val) == end()){
			numberOfValues++;
		}
		int x = 1;
		x <<= val-1;
		values = values | x;

		return SudokuSquareSetIterator(val-1, this);
	}
	SudokuSquareSetIterator find(int val) const{
		int x = 1;
		x <<= val-1;
		if(values & x){
			return SudokuSquareSetIterator(val-1,this);
		}
		return end();
	}
	int erase(int val){
		if (find(val) != end()){
			int x = 1;
			x <<= val-1;
			values -= x;
			numberOfValues--;
			return 1;
		}
		return 0;
	}
	void erase (SudokuSquareSetIterator itr){
		erase(*itr);
	}

};



// Do not write any code below this line
static_assert(sizeof(SudokuSquareSet) == sizeof(unsigned int) + sizeof(int), "The SudokuSquareSet class needs to have exactly two 'int' member variables, and no others");


#endif
