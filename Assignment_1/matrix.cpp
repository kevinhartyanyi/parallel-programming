#include <iostream>
#include <iterator>
#include <sstream>
#include "matrix.hpp"

Matrix::Matrix(int rows, int cols): mRows(rows), mCols(cols) 
{
    mData.reserve(rows * cols);
}

int Matrix::operator()(size_t y, size_t x)
{
    return mData[y * mCols + x];
}

void Matrix::addRow(const std::string& line)
{
    auto splitLine = split(line, ' '); 
    mData.insert(mData.end(), splitLine.begin(), splitLine.end());
}

std::vector<int> Matrix::split(const std::string& line, char delimiter)
{
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

std::vector<int> Matrix::operator*(const std::vector<int> vec) const
{    
    std::vector<int> re;
    re.reserve(mRows);

    int sum = 0;
    
    for (size_t i = 0; i < mData.size(); i++)
    {
        sum += mData[i] * vec[i % mCols];
        if((i + 1) % mCols == 0)
        {  
            re.push_back(sum);
            sum = 0;
        }
    }
    
    return re;   
}