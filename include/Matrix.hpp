#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "varray.hpp"

namespace gm
{
using namespace std;

#define PAD(X) (div_down((X),L1_LINE_DN)*(L1_LINE_DN*(L1_LINE_DN-1))/2)
// Optm: test switching, the below doesnt work probably
//#define PAD(X) ((long)floor((X)/(double)L1_LINE_DN)*(L1_LINE_DN*(L1_LINE_DN-1))/2)

#define PADDING true

// Non member access functions

template<template<class> class Cont, class Elem>
Elem& at(Cont<Elem>& C, long i, long j){
	return C.at(i,j);
}
template<template<class> class Cont, class Elem>
const Elem& at(Cont<Elem> const& M, long i, long j){
	return M.at(i,j);
}

/**
 * @brief Stores values of matrix in a vector, Row Major Order
 */
template<class Elem>
class Matrix
{
public:
	varray<Elem> varr;
	size_t mSize;
	size_t mSizeVec; // n of vec<element>s
	
	size_t mSizeMem;
	size_t mSizeVecMem;
	
	size_t mEndVec;
	
	void mem_alloc(long mSize){
		varr.alloc_size(mSize*mSize);
	}
	/**
	 * @param size of matrix, total number of lines
	 */
	Matrix(long size)
	:	mSize(size){
		mSizeVec = mSize/(varr.regEN());
		mSizeMem = mSize;
		if(PADDING){
			mSizeMem += mod(-mSize,(long)L1_LINE_DN);
			// make sure m_size is odd multiple of cache line
			if(((mSizeMem/L1_LINE_DN) % 2) == 0){
				mSizeMem = mSizeMem + L1_LINE_DN;
			}
		}
		mSizeVecMem = mSizeMem/varr.regEN();
		mEndVec = Lower_Multiple(mSize, varr.regEN());
		mem_alloc(mSizeMem);
	}
	
	/** @brief size of the varray */
	size_t size(){ return mSize; }
	/** @brief size of the vectorized varray */
	size_t sizeVec(){ return mSizeVec; }
	/** @brief end of the vectorized varray */
	size_t endVec(){ return mEndVec; }
	
	long m_posv(long i, long j) const {
		return i*mSizeVecMem + j;
	}
	vec<Elem>& atv(long i, long j) {
		return varr.atv(m_posv(i,j));
	}
	const vec<Elem>& atv(long i, long j) const {
		return varr.atv(m_posv(i,j));
	}
	long m_pos(long i, long j) const {
		return i*mSizeMem + j;
	}
	Elem& at(long i, long j){
		return varr.at(m_pos(i,j));
	}
	const Elem& at(long i, long j) const {
		return varr.at(m_pos(i,j));
	}
};

/**
 * @brief Stores values of matrix in a vector, Column Major Order
 */
template<class Elem>
class MatrixColMajor : public Matrix<Elem>
{
public:
	using Matrix<Elem>::Matrix;
	
	long m_posv(long i, long j) const {
		return j*this->mSizeVecMem + i;
	}
	vec<Elem>& atv(long i, long j) {
		return this->varr.atv(m_posv(i,j));
	}
	const vec<Elem>& atv(long i, long j) const {
		return this->varr.atv(m_posv(i,j));
	}
	long m_pos(long i, long j) const {
		return j*this->mSizeMem + i;
	}
	Elem& at(long i, long j){
		return this->varr.at(m_pos(i,j));
	}
	const Elem& at(long i, long j) const {
		return this->varr.at(m_pos(i,j));
	}
	/**/
};

template<class Mat>
void swap_rows(Mat& M, long row0, long row1){
	if(row0 == row1)
		return;
	for(long j = 0; j < M.size; j++){
		swap(M.at(row0, j), M.at(row1, j));
	}
}
/**
 * @brief M += B
 * @param sign -1 with you want to add -b
 */
template<class Mat>
void add(Mat& M, Mat& B, double sign = 1){
	for(long i=0; i < M.size; i++){
		for(long j=0; j < M.size; j++){
			M.at(i,j) += sign*B.at(i,j);
		}
	}
}
/**
 * @brief copy matrix A to yourself
 */
template<class Mat, class elem>
void set(Mat& M, const Matrix<elem>& A){
	for(long i=0; i < M.size; i++){
		for(long j=0; j < M.size; j++){
			M.at(i,j) = A.at(i,j);
		}
	}
}
/**
 * @brief sets all matrix to parameter
 */
template<class Mat>
void set(Mat& M, double x){
	for(long i=0; i < M.size; i++){
		for(long j=0; j < M.size; j++){
			M.at(i,j) = x;
		}
	}
}

template<class Mat>
void print(Mat& M){
	for(long i = 0; i < M.size; i++){
		for(long j = 0; j < M.size; j++){
			cout << M.at(i, j) <<" ";
		}
		cout << endl;
	}
}
/**
 * @brief sets I to identity
 */
template<class Mat>
void identity(Mat& I){
	for(long i = 0; i < I.size; i++){
		for(long j = 0; j < I.size; j++){
			I.at(i, j) = 0;
		}
		I.at(i, i) = 1;
	}
}
/**
 * @brief Assigns random matrix to M
 * @param M needs to have been allocated
 */
template<class Mat>
void randomMatrix(Mat& M){
	long i, j;
	double invRandMax = 1.0/(double)RAND_MAX;
	
	for(i = 0; i < M.size; i++){
		for(j = 0; j < M.size; j++){
			M.at(i,j) = (double)rand() * invRandMax;
		}
	}
}
/**
 * @brief prints matrix with size in the first line
 */
template<class Mat>
void printm(Mat& M){
	cout<<  M.size <<"\n";
	print(M);
}


}
#endif