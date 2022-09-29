//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Tomas Tomala <xtomal02@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Tomas Tomala
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//
class MatrixConstructor : public ::testing::Test{
protected:
	Matrix matrix;
	void constructMatrix(size_t R, size_t C){
		//return Matrix(R,C);
		matrix=Matrix(R,C);
	}
};

TEST_F(MatrixConstructor,ConstructingInvalidMatrixes){
	EXPECT_ANY_THROW(constructMatrix(3,-1));
	EXPECT_ANY_THROW(constructMatrix(0,0));
	EXPECT_ANY_THROW(constructMatrix(0,1));
	EXPECT_ANY_THROW(constructMatrix(1,0));
}

TEST_F(MatrixConstructor,ConstructingValidMatrixes){
	EXPECT_NO_THROW(constructMatrix(1,1));
	EXPECT_NO_THROW(constructMatrix(3,3));
	EXPECT_NO_THROW(constructMatrix(6,6));
	EXPECT_NO_THROW(constructMatrix(2,4));
	EXPECT_NO_THROW(constructMatrix(4,2));
}

TEST_F(MatrixConstructor,SetNonExistingIndexes){
	EXPECT_NO_THROW(constructMatrix(1,1));
	EXPECT_FALSE(matrix.set(0,1,4));
	EXPECT_FALSE(matrix.set(1,0,6));
	EXPECT_FALSE(matrix.set(3,1,2));
	EXPECT_FALSE(matrix.set(1,3,1));
	EXPECT_NO_THROW(constructMatrix(4,4));
	EXPECT_FALSE(matrix.set(4,1,2));
	//Array of values
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2, 3}, {1, 2, 3}};
	EXPECT_FALSE(matrix.set(values));
	values = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
	EXPECT_FALSE(matrix.set(values));
}

TEST_F(MatrixConstructor,SetExistingIndexes){
	EXPECT_NO_THROW(constructMatrix(1,1));
	EXPECT_TRUE(matrix.set(0,0,2));
	EXPECT_TRUE(matrix.set(0,0,1));

	EXPECT_NO_THROW(constructMatrix(4,4));
	EXPECT_TRUE(matrix.set(2,3,0));
	EXPECT_TRUE(matrix.set(1,1,6));
	EXPECT_TRUE(matrix.set(3,1,3));

	EXPECT_NO_THROW(constructMatrix(3,3));
	std::vector<std::vector< double > > values = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
	EXPECT_TRUE(matrix.set(values));
}

TEST_F(MatrixConstructor,GetNonExistingIndexes){
	EXPECT_NO_THROW(constructMatrix(3,3));
	std::vector<std::vector< double > > values = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
	EXPECT_TRUE(matrix.set(values));
	EXPECT_ANY_THROW(matrix.get(-2,1));
	EXPECT_ANY_THROW(matrix.get(3,1));
	EXPECT_ANY_THROW(matrix.get(5,2));

	EXPECT_NO_THROW(constructMatrix(1,1));
	EXPECT_ANY_THROW(matrix.get(1,1));
	EXPECT_ANY_THROW(matrix.get(3,1));
	EXPECT_ANY_THROW(matrix.get(5,2));

}

TEST_F(MatrixConstructor,GettingExistingValues){
	EXPECT_NO_THROW(constructMatrix(3,3));
	std::vector<std::vector< double > > values = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
	EXPECT_TRUE(matrix.set(values));
	EXPECT_TRUE(matrix.get(0,0)==1);
	EXPECT_TRUE(matrix.get(0,2)==3);
	EXPECT_TRUE(matrix.get(2,2)==3);

	EXPECT_NO_THROW(constructMatrix(1,1));
	EXPECT_TRUE(matrix.get(0,0)==0);
	EXPECT_TRUE(matrix.set(0,0,1));
	EXPECT_TRUE(matrix.get(0,0)==1);
}

TEST_F(MatrixConstructor,OperatorEquals){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {1, 2}};
	EXPECT_TRUE(matrix.set(values));
	Matrix matrixSecond=matrix;
	EXPECT_TRUE(matrix.operator==(matrixSecond));
	EXPECT_TRUE(matrix.set(0,1,0));
	EXPECT_FALSE(matrix.operator==(matrixSecond));
	EXPECT_NO_THROW(constructMatrix(1,2));
	EXPECT_ANY_THROW(matrix.operator==(matrixSecond));
	//Different sizes

	EXPECT_NO_THROW(constructMatrix(1,1));
	values = {{1}};
	EXPECT_TRUE(matrix.set(values));
	matrixSecond=matrix;
	EXPECT_TRUE(matrix.operator==(matrixSecond));
	EXPECT_TRUE(matrix.set(0,0,2));
	EXPECT_FALSE(matrix.operator==(matrixSecond));
	EXPECT_NO_THROW(constructMatrix(1,2));
	EXPECT_ANY_THROW(matrix.operator==(matrixSecond));

}

TEST_F(MatrixConstructor,OperatorPlus){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {1, 2}};
	matrix.set(values);
	Matrix matrixDifferentSize = Matrix(2,3);
	EXPECT_ANY_THROW(matrix.operator+(matrixDifferentSize));
	Matrix matrixSecond = Matrix(2,2);
	std::vector<std::vector< double > > valuesSecond = {{4, 1}, {1, 5}};
	matrixSecond.set(valuesSecond);
	Matrix matrixExpected = Matrix(2,2);
	std::vector<std::vector< double > > valuesExpected = {{5, 3}, {2, 7}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator+(matrixSecond)));
	//Different sizes

	EXPECT_NO_THROW(constructMatrix(2,3));
	 values = {{1, 2, 3}, {1, 2, 3}};
	matrix.set(values);
	matrixDifferentSize = Matrix(3,3);
	EXPECT_ANY_THROW(matrix.operator+(matrixDifferentSize));
	matrixSecond = Matrix(2,3);
	valuesSecond = {{4, 1, 4}, {1, 5, 5}};
	matrixSecond.set(valuesSecond);
	matrixExpected = Matrix(2,3);
	valuesExpected = {{5, 3, 7}, {2, 7, 8}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator+(matrixSecond)));


}	
/*
TEST_F(MatrixConstructor,OperatorMinus){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {1, 2}};
	matrix.set(values);
	Matrix matrixDifferentSize = Matrix(2,3);
	EXPECT_ANY_THROW(matrix.operator-(matrixDifferentSize));
	Matrix matrixSecond = Matrix(2,2);
	std::vector<std::vector< double > > valuesSecond = {{4, 1}, {1, 5}};
	matrixSecond.set(valuesSecond);
	Matrix matrixExpected = Matrix(2,2);
	std::vector<std::vector< double > > valuesExpected = {{-3, 1}, {0, -3}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator-(matrixSecond)));
	//Different sizes
	
}
*/

TEST_F(MatrixConstructor,OperatorMultiplyMatrixes){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {1, 2}};
	matrix.set(values);
	Matrix matrixDifferentSize = Matrix(3,3);
	EXPECT_ANY_THROW(matrix.operator*(matrixDifferentSize));
	Matrix matrixSecond = Matrix(2,2);
	std::vector<std::vector< double > > valuesSecond = {{4, 1}, {1, 5}};
	matrixSecond.set(valuesSecond);
	Matrix matrixExpected = Matrix(2,2);
	std::vector<std::vector< double > > valuesExpected = {{6, 11}, {6, 11}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator*(matrixSecond)));
	//Different sizes

	EXPECT_NO_THROW(constructMatrix(3,3));
	values = {{1, 2, 5}, {3, 2, 6}, {4, 2, 7}};
	matrix.set(values);
	matrixDifferentSize = Matrix(4,4);
	EXPECT_ANY_THROW(matrix.operator*(matrixDifferentSize));
	matrixSecond = Matrix(3,3);
	valuesSecond = {{1, 2, 5}, {3, 2, 6}, {4, 2, 7}};
	matrixSecond.set(valuesSecond);
	matrixExpected = Matrix(3,3);
	valuesExpected = {{27, 16, 52}, {33, 22, 69},{38, 26, 81}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator*(matrixSecond)));
}

TEST_F(MatrixConstructor,OperatorMultiplyConstant){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {1, 2}};
	matrix.set(values);
	Matrix matrixExpected = Matrix(2,2);
	std::vector<std::vector< double > > valuesExpected = {{2, 4}, {2, 4}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator*(2)));
	
	EXPECT_NO_THROW(constructMatrix(2,3));
	values={{4, 1, 4}, {1, 5, 5}};
	matrix.set(values);
	matrixExpected = Matrix(2,3);
	 valuesExpected = {{12, 3, 12}, {3, 15, 15}};
	matrixExpected.set(valuesExpected);
	EXPECT_TRUE(matrixExpected.operator==(matrix.operator*(3)));
}
TEST_F(MatrixConstructor,SolveInvalidEquation){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 1}, {2, 2}};
	matrix.set(values);
	std::vector<double> rightSide = {2, 6};
	std::vector<double> results;
	EXPECT_ANY_THROW(results=matrix.solveEquation(rightSide));
}

TEST_F(MatrixConstructor,SolveEquation){
	EXPECT_NO_THROW(constructMatrix(2,2));
	std::vector<std::vector< double > > values = {{1, 2}, {3, 4}};
	matrix.set(values);
	std::vector<double> rightSide = {1, 2};
	std::vector<double> resultsExpected = {0, 0.5};
	std::vector<double> results;
	EXPECT_NO_THROW(results=matrix.solveEquation(rightSide));
	EXPECT_TRUE(results==resultsExpected);
	//Different sizes
	//3x3
	EXPECT_NO_THROW(constructMatrix(3,3));
	values = {{1, 2, 5}, {3, 4, 7},{6, 8, 9}};
	matrix.set(values);
	rightSide = {1,2,4};
	resultsExpected = {0,0.5,0};
	results;
	EXPECT_NO_THROW(results=matrix.solveEquation(rightSide));
	EXPECT_TRUE(results==resultsExpected);
	//1x1
	EXPECT_NO_THROW(constructMatrix(1,1));
	values = {{4}};
	matrix.set(values);
	rightSide = {4};
	resultsExpected = {1};
	results;
	EXPECT_NO_THROW(results=matrix.solveEquation(rightSide));
	EXPECT_TRUE(results==resultsExpected);
    //4x4
	EXPECT_NO_THROW(constructMatrix(4,4));
	values= {{1,1,1,1},{1,-2,-1,2},{2,1,1,3},{3,2,-1,1}};
	matrix.set(values);
	rightSide = {8,4,17,10};
	resultsExpected = {1,2,1,4};
	results;
	EXPECT_NO_THROW(results=matrix.solveEquation(rightSide));
	EXPECT_TRUE(results==resultsExpected);
}

TEST_F(MatrixConstructor,SolveEquationInvalidMatrix){
	constructMatrix(3, 2);
	std::vector<std::vector< double > > values = {{1,4},{2,5},{3,6}};
	matrix.set(values);
	std::vector<double> rightSide = {1,1};
	std::vector<double> results;
	EXPECT_ANY_THROW(results=matrix.solveEquation(rightSide));
	rightSide = {1,1,1};
	EXPECT_ANY_THROW(results=matrix.solveEquation(rightSide));


}


/*** Konec souboru white_box_tests.cpp ***/
