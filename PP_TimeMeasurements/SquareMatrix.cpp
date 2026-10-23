#include "SquareMatrix.h"
#include <stdexcept>

using std::runtime_error;

int SquareMatrix::get(int row, int column) const
{
	if(!isValidIndex(row, column)) 
		throw runtime_error("Index out of range.");
	return data[row][column];
}

void SquareMatrix::set(int row, int column, int value)
{
	if (!isValidIndex(row, column))
		throw runtime_error("Index out of range.");
	data[row][column] = value;
}

void SquareMatrix::add(int row, int column, int value)
{
	if (!isValidIndex(row, column))
		throw runtime_error("Index out of range.");
	data[row][column] += value;
}

SquareMatrix SquareMatrix::operator*(const SquareMatrix& other) const
{
	if (size != other.size) 
		throw runtime_error("The size of both matrices must be equal.");
	SquareMatrix result(other.size);
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			for (int j = 0; j < size; j++) {
				result.data[i][j] += data[i][k] * other.data[k][j];
			}
		}
	}
	return result;
}