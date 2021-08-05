#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <deque>
#include <algorithm>
#include "utils.hpp"
#include "partition.hpp"

int main() 
{
    int N; 
    std::ifstream reader("input.txt");
    reader >> N;   

    std::vector<Pipe<query>> channel(N + 1);
    std::vector<std::thread> part;

    for (size_t i = 0; i < N + 1; i++)
    {
        if (i > 0 && i < N + 1)
            part.push_back(std::thread(&read, i, std::ref(channel[i-1]), std::ref(channel[i])));
    }
    
    int Q;
    reader >> Q;

    for (size_t i = 0; i < Q; i++)
    {
        int C;
        reader >> C;
        query q = std::make_shared<query_condition>();
        for (size_t i = 0; i < C; i++)
        { 
            int condtype;
            reader >> condtype;
            switch (condtype)
            {
                case 1:
                {
                    bool flag;
                    reader >> flag;
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
                    auto cond = std::make_shared<string_condition>();
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

    std::thread wth(&write, std::ref(channel.back()));

    for (size_t i = 0; i < part.size(); i++)
        part[i].join();

    wth.join();

    return 0;
}