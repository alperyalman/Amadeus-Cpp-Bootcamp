#include "VectorOperations.h"

int main()
{
    // Use current time as seed for random generator
    srand(time(0));

    VectorOperations vecMathObj;
    int maxRandVal = 10;
    int numOfEquations;
    int x_k;
    int k;

    std::cout << "\nEquation Ax = b where A is nxn square matrix, x and b are nx1 matrices.\n"
              << std::endl;

    std::cout << "A and b will be randomly generated according to number of equations.\n"
              << "It is assumed that all the coefficients are integer values. It can be changed in the program.\n"
              << "_____________________________________________________________________________________________\n"
              << std::endl;

    std::cout << "Enter the number of equations (n): ";
    std::cin >> numOfEquations;

    // Define matrices in the equation
    std::vector<std::vector<int>> A(numOfEquations, std::vector<int>(numOfEquations));
    std::vector<int> b(numOfEquations);

    // Fill the matrix with random values
    for (auto &vect : A)
        for (auto &num : vect)
            num = (rand() % maxRandVal) + 1; // Create random number from 1 to maxRandVal

    for (auto &num : b)
        num = (rand() % maxRandVal) + 1; // Create random number from 1 to maxRandVal

    std::cout << "\n-------------------------------\nA "
              << "(" << numOfEquations << "x" << numOfEquations << ") matrix is below\n-------------------------------\n"
              << std::endl;

    vecMathObj.printMatrix(A);

    std::cout << "\n-------------------------------\nb "
              << "(" << numOfEquations
              << "x1) matrix is below\n-------------------------------\n"
              << std::endl;
    vecMathObj.printVector(b);
    std::cout << "_____________________________________________________________________________________________\n"
              << std::endl;

    // Get the number for erasing column and row of the matrix
    std::cout << "Enter the row or column number you want to remove (index from 1 to " << numOfEquations << "): ";
    std::cin >> k;

    // Get the known x value
    std::cout << "\nEnter the value of x" << k << ": ";
    std::cin >> x_k;

    // Define new matrices to be calculated
    // std::vector<std::vector<int>> A_new(numOfEquations - 1, std::vector<int>(numOfEquations - 1));
    // std::vector<int> b_new(numOfEquations - 1);
    std::vector<std::vector<int>> A_new;
    std::vector<int> b_new;

    // auto A_new = vecMathObj.eraseMatrixRowAndColumn(A, k);
    // auto b_new = vecMathObj.substractVectors(vecMathObj.eraseVectorVal(b, k), vecMathObj.multiplyVectorByConstant(vecMathObj.eraseVectorVal(vecMathObj.getMatrixColumn(A, k), k), x_k));

    // Calculate new A and b matrices using the inital ones
    vecMathObj.CalculateNewMatrices(A, b, k, x_k, A_new, b_new);
    std::cout << "\nCalculations completed!" << std::endl;
    std::cout << "_____________________________________________________________________________________________\n"
              << std::endl;
    // Print new A matrix.
    std::cout << "\n---------------------------------\n"
              << "New A "
              << "(" << numOfEquations - 1 << "x" << numOfEquations - 1 << ") matrix is below"
              << "\n---------------------------------\n"
              << "\n"
              << k << ". row and column were removed.\n"
              << std::endl;

    vecMathObj.printMatrix(A_new);

    // Print new b matrix.
    std::cout << "\n-------------------------------\n"
              << "New b "
              << "(" << numOfEquations - 1 << "x1) matrix is below"
              << "\n-------------------------------\n"
              << "\n"
              << k << ". row was removed and new values were calculated.\n"
              << std::endl;

    vecMathObj.printVector(b_new);

    return 0;
}
