#ifndef MATRIX

#include <vector>
#include <string>

class Matrix
{
public:
    Matrix(int rows, int cols);
    void addRow(const std::string& line);
    int operator()(size_t y, size_t x);
    std::vector<int> operator*(const std::vector<int> right) const;

private:
    std::vector<int> split(const std::string& line, char delimiter);

    std::vector<int> mData;
    int mRows;
    int mCols;
};

#endif // !MATRIX
