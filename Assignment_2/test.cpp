#include <iostream>
#include <fstream>

int main()
{
    std::ofstream out("input3.dat");
    out << 10000 << std::endl;

    for (size_t i = 1; i <= 10000; i++)
    {
        int middle;
        if (i % 2 == 0)
            middle = 0;
        else
            middle = 1;
        
        out << i << " " << middle << " " << "string" << i << std::endl;
    }
    out.close();
}
