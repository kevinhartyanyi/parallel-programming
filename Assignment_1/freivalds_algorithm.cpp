#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <future>
#include "matrix.hpp"


#include <iterator>
#include <sstream>
// Külön cpp fájlban (matrix.cpp) nem fordította le a BEAD
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




int randomNumber(int min, int max)
{
    static thread_local std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

std::vector<int> randomVector(size_t size)
{
    std::vector<int> re;
    re.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        re.push_back(randomNumber(0,1));
    }
    return re;    
}

bool check(const Matrix& A, const Matrix& B, const Matrix& C, size_t size)
{
    auto alpha = randomVector(size);    

    auto beta  = A * (B * alpha);
    auto gamma = C * alpha;
    return beta == gamma;
}

bool solution(const Matrix& A, const Matrix& B, const Matrix& C, size_t size, int tryCount)
{
    bool re = true;
    std::vector<std::future<bool>> futures;
    futures.reserve(tryCount);
    for (size_t i = 0; i < tryCount; i++)
    {
        futures.push_back(std::async(std::launch::async, check, std::ref(A), std::ref(B), std::ref(C), size));
    }
    
    for (auto&& fut : futures)
    {
        if(!fut.get()) re = false;
    }
    return re;
}

int main() {

    int N; // Matrix size
    int tryCount;

    std::string line;

    std::ifstream reader("input.txt");
    getline(reader, line, ' ');
    N = std::stoi(line);
    getline(reader, line);
    tryCount = std::stoi(line);

    Matrix A(N,N);
    Matrix B(N,N);
    Matrix C(N,N);   
    
    Matrix* fill = &A;
    for(int i = 1; !reader.eof(); ++i) 
    {
        getline(reader, line);
        fill->addRow(line);
        if(i == N) fill = &B;
        else if(i == 2*N) fill = &C;
    }
    reader.close();
    
    auto result = (solution(A,B,C,N,tryCount))? "accepted" : "rejected";

    std::ofstream out("output.txt");
    out << result;
    out.close();

    return 0;
}