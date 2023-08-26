/**
 * @file VectorOperations.h
 * @author Alper Yalman (alperyalman@gmail.com)
 * @brief VectorOperations class includes mathematical operations for vectors and matrices created by 2D vectors.
 * @version 0.1
 * @date 2023-08-26
 *
 * Following operations are included in the class.
 *
 * - Vector sum: v1 + v2
 * - Vector substraction, v1- v2
 * - Vector multiplication: v1*v2
 * - Vector multiplication by a constant number: v1*c1
 * - Erase vector's value at spesific line
 * - Get specific row of a matrix
 * - Get specific column of a matrix
 * - Erase specific column and row of a matrix
 * - Calculate new A and b matrices after erasing specific row and column of A and b in equation Ax = b
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>

/**
 * @brief Mathematical operations for vectors and matrices created using 2D vectors.
 *
 */
class VectorOperations
{
public:
    VectorOperations();
    ~VectorOperations();

    template <typename T>
    std::vector<T> sumVectors(const std::vector<T> &v1, const std::vector<T> &v2);

    template <typename T>
    std::vector<T> substractVectors(const std::vector<T> &v1, const std::vector<T> &v2);

    template <typename T>
    std::vector<T> multiplyVectors(const std::vector<T> &v1, const std::vector<T> &v2);

    template <typename T>
    std::vector<T> multiplyVectorByConstant(const std::vector<T> &v1, const T &c1);

    template <typename T>
    std::vector<T> eraseVectorVal(const std::vector<T> &v1, const T &c1);

    template <typename T>
    std::vector<T> getMatrixRow(const std::vector<std::vector<T>> &matrix, int rowNumber);

    template <typename T>
    std::vector<T> getMatrixColumn(const std::vector<std::vector<T>> &matrix, int columnNumber);

    template <typename T>
    std::vector<std::vector<T>> eraseMatrixRowAndColumn(const std::vector<std::vector<T>> &matrix, const int &eraseNum);

    template <typename T>
    void CalculateNewMatrices(const std::vector<std::vector<T>> &A, const std::vector<T> &b, const int &k, const T x_k, std::vector<std::vector<T>> &A_new, std::vector<T> &b_new);

    template <typename T>
    void printMatrix(const std::vector<std::vector<T>> &matrix);

    template <typename T>
    void printVector(const std::vector<T> &vec);
};

/**
 * @brief Construct a new Vector Operations:: Vector Operations object
 *
 */
VectorOperations::VectorOperations()
{
}

/**
 * @brief Destroy the Vector Operations:: Vector Operations object
 *
 */

VectorOperations::~VectorOperations()
{
}

/**
 * @brief Get sum of two vectors
 *
 * sumVectors() function returns sum of two vectors: v1 + v2
 *
 * @tparam T Data type
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @return std::vector<T>
 */
template <typename T>
std::vector<T> VectorOperations::sumVectors(const std::vector<T> &v1, const std::vector<T> &v2)
{
    // Make sure both vectors are of the same size
    if (v1.size() != v2.size())
    {
        throw std::runtime_error("Vector sizes must be equal.");
    }

    std::vector<T> result;
    result.reserve(v1.size()); // Reserve space to avoid reallocations

    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result),
                   [](T a, T b)
                   { return a + b; });

    return result;
}

/**
 * @brief Get substraction of two vectors
 *
 * substractVectors() function returns substraction of two vectors: v1 - v2
 *
 * @tparam T Data Type
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @return std::vector<T>
 */
template <typename T>
std::vector<T> VectorOperations::substractVectors(const std::vector<T> &v1, const std::vector<T> &v2)
{
    // Make sure both vectors are of the same size
    if (v1.size() != v2.size())
    {
        throw std::runtime_error("Vector sizes must be equal.");
    }

    std::vector<T> result;
    result.reserve(v1.size()); // Reserve space to avoid reallocations

    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result),
                   [](T a, T b)
                   { return a - b; });

    return result;
}

/**
 * @brief Get multiplication of two vectors
 *
 * multiplyVectors() function returns multiplication of two vectors: v1*v2
 *
 * @tparam T Data Type
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @return std::vector<T>
 */
template <typename T>
std::vector<T> VectorOperations::multiplyVectors(const std::vector<T> &v1, const std::vector<T> &v2)
{
    // Make sure both vectors are of the same size
    if (v1.size() != v2.size())
    {
        throw std::runtime_error("Vector sizes must be equal.");
    }

    std::vector<T> result;
    result.reserve(v1.size()); // Reserve space to avoid reallocations

    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result),
                   [](T a, T b)
                   { return a * b; });

    return result;
}

/**
 * @brief Get multiplication of two vectors
 *
 * multiplyVectors() function multiplies a vector by a constant number, and returns the result: v1*c1
 *
 * @tparam T Data Type
 * @param v1 Vector 1
 * @param c1 Constant value
 * @return std::vector<T>
 */
template <typename T>
std::vector<T> VectorOperations::multiplyVectorByConstant(const std::vector<T> &v1, const T &c1)
{
    std::vector<T> result = v1;
    result.reserve(v1.size()); // Reserve space to avoid reallocations

    for (auto &val : result)
        val *= c1;

    return result;
}

/**
 * @brief Erase specified row in a vector
 *
 * multiplyVectors() function erases specified row from the input vector and returns the result.
 *
 * @tparam T Data Type
 * @param v1 Vector 1
 * @param k Row number to be deleted
 * @return std::vector<T>
 */

template <typename T>
std::vector<T> VectorOperations::eraseVectorVal(const std::vector<T> &v1, const T &k)
{
    // Erase column
    std::vector<T> result = v1;
    result.erase(result.begin() + k - 1);
    return result;
}

/**
 * @brief Get specified row in a matrix
 *
 * getMatrixRow() function returns specified row of the input matrix.
 *
 * @tparam T Data Type
 * @param matrix Input matrix
 * @param rowNumber Row number to be deleted
 * @return std::vector<T>
 */

template <typename T>
std::vector<T> VectorOperations::getMatrixRow(const std::vector<std::vector<T>> &matrix, int rowNumber)
{
    // Make sure both vectors are of the same size
    if (rowNumber > matrix.size())
    {
        throw std::runtime_error("Row number exceeds the size.");
    }

    return matrix[rowNumber - 1];
}

/**
 * @brief Get specified column in a matrix
 *
 * getMatrixColumn() function returns specified column of the input matrix.
 *
 * @tparam T Data Type
 * @param matrix
 * @param columnNumber
 * @return std::vector<T>
 */
template <typename T>
std::vector<T> VectorOperations::getMatrixColumn(const std::vector<std::vector<T>> &matrix, int columnNumber)
{
    // Make sure both vectors are of the same size
    if (columnNumber > matrix.size())
    {
        throw std::runtime_error("Column number exceeds the size.");
    }

    std::vector<T> result;
    result.reserve(matrix.size()); // Reserve space to avoid reallocations

    for (int i = 0; i < matrix.size(); i++)
        result.push_back(matrix[i][columnNumber - 1]);

    return result;
}

/**
 * @brief Erase the specified row and column from the matrix
 *
 * eraseMatrixRowAndColumn() erases specified column and row from the (nxn) input matrix and returns (n-1)x(n-1) matrix.
 *
 * @tparam T Data Type
 * @param matrix Input Matrix
 * @param eraseNum Column and row number to be erased
 * @return std::vector<std::vector<T>>
 */
template <typename T>
std::vector<std::vector<T>> VectorOperations::eraseMatrixRowAndColumn(const std::vector<std::vector<T>> &matrix, const int &eraseNum)
{
    // Make sure both vectors are of the same size
    if (eraseNum > matrix.size() - 1)
    {
        throw std::runtime_error("Erase number exceeds the size.");
    }

    // Erase line
    std::vector<std::vector<T>> result = matrix;
    result.erase(result.begin() + eraseNum - 1);

    // Erase column
    for (std::vector<T> &vect : result)
    {
        vect.erase(vect.begin() + eraseNum - 1);
    }

    return result;
}

/**
 * @brief
 *
 * CalculateNewMatrices() function takes the (nxn) and (nx1) input matrices A and b, and calculates (n-1)x(n-1)
 * and (n-1)x1 output matrices, A_new and b_new.
 *
 * @tparam T Data type
 * @param A (nxn) Input matrix A
 * @param b (nx1) Input matrix b
 * @param k Row and column number to be deleted
 * @param x_k Known x_k value
 * @param A_new (n-1)x(n-1) output matrix A_new
 * @param b_new (n-1)x1 output matrix b_new
 */
template <typename T>
void VectorOperations::CalculateNewMatrices(const std::vector<std::vector<T>> &A, const std::vector<T> &b, const int &k, const T x_k, std::vector<std::vector<T>> &A_new, std::vector<T> &b_new)
{
    // Calculate new A matrix
    A_new = eraseMatrixRowAndColumn(A, k);

    // Calculate new b matrix
    std::vector<T> removedColumn = eraseVectorVal(getMatrixColumn(A, k), k);
    b_new = substractVectors(eraseVectorVal(b, k), multiplyVectorByConstant(removedColumn, x_k));
}

/**
 * @brief Print matrix elements
 *
 * @tparam T Data type
 * @param matrix Input matrix
 */
template <typename T>
void VectorOperations::printMatrix(const std::vector<std::vector<T>> &matrix)
{
    for (auto &vect : matrix)
    {
        for (auto &val : vect)
            std::cout << val << "\t";

        std::cout << std::endl
                  << std::endl;
    }
}

/**
 * @brief Print vector elements
 *
 * @tparam T Data type
 * @param vec Input vector
 */
template <typename T>
void VectorOperations::printVector(const std::vector<T> &vec)
{

    for (auto &val : vec)
        std::cout << val << "\n";
    std::cout << std::endl
              << std::endl;
}