#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <deque>
#include <algorithm>
#include "utils.hpp"
#include "partition_good.hpp"



int main() 
{
    int N; 
    std::ifstream reader("input.txt");
    reader >> N;   

    std::cout << "N: " << N << std::endl;
    std::vector<Pipe<query>> channel(N + 1);
    std::vector<std::thread> part;

    for (size_t i = 0; i < N + 1; i++)
    {
        if (i > 0 && i < N + 1)
        {
            std::cout << "Start Thread" << std::endl;
            part.push_back(std::thread(&read, i, std::ref(channel[i-1]), std::ref(channel[i])));
        }        
    }
    
    int Q;
    reader >> Q;
    std::cout << "Q: " << Q << std::endl;

    for (size_t i = 0; i < Q; i++)
    {
        int C;
        reader >> C;
        std::cout << "C: " << C << std::endl;
        query q = std::make_shared<query_condition>();
        for (size_t i = 0; i < C; i++)
        { 
            int condtype;
            reader >> condtype;
            std::cout << "CondType: " << condtype << std::endl;
            switch (condtype)
            {
                case 1:
                {
                    bool flag;
                    reader >> flag;
                    std::cout << "bool condition" << std::endl;
                    std::cout << "bool: " << flag << std::endl;
                    auto cond = std::make_shared<bool_condition>();
                    cond->flag = flag;                    
                    q->conditions.push_back(cond);
                    break;
                }
                case 2:
                {
                    int value;
                    int check;
                    reader >> value;
                    reader >> check;
                    std::cout << "int condition" << std::endl;
                    auto cond = std::make_shared<int_condition>();
                    cond->value = value;
                    cond->check = (check == -1)? Check::Smaller : (check == 0)? Check::Equal : Check::Bigger;
                    q->conditions.push_back(cond);
                    break;
                }
                case 3:
                {
                    std::string data;
                    reader >> data;
                    std::cout << "string condition" << std::endl;
                    auto cond = std::make_shared<string_condition>();
                    std::cout << data << std::endl;
                    cond->data = (data == "NULL")? nullptr : std::make_shared<std::string>(data);
                    q->conditions.push_back(cond);
                    break;
                }
            }
        }
        channel.front().push(q);
    }    
    
    channel.front().push(nullptr);
    reader.close();
    
    std::cout << "Channel numbers: " << channel.size() << std::endl;
    std::thread wth(&write, std::ref(channel.back()));

    for (size_t i = 0; i < part.size(); i++)
    {
        part[i].join();
    }
    wth.join();

    return 0;
}