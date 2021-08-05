#ifndef PARTITION
#define PARTITION

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include "utils.hpp"

struct bool_condition : condition
{
    bool flag;
    bool eval(const record& rec) const override
    {
        return rec.flag == flag;
    }
};

enum Check {Smaller, Equal, Bigger};
struct int_condition : condition
{
    Check check;
    int value;
    bool eval(const record& rec) const override
    {
        bool re;
        switch (check)
        {
        case Smaller:
            re = rec.value > value;
            break;
        case Equal:
            re = rec.value == value;
            break;
        case Bigger:
            re = rec.value < value;
            break;
        }
        return re;
    }
};

struct string_condition : condition
{
    std::shared_ptr<std::string> data;
    bool eval(const record& rec) const override
    {
        bool re = true;
        if((!rec.data && data) || (rec.data && !data))
        {
            re = false;
        }
        else if(rec.data && data)
        {
            re = (*rec.data == *data);
        }
        return re;
    }
};

void read(int part_num, Pipe<query>& from, Pipe<query>& to)
{
    std::string filename = std::string("input") + std::to_string(part_num) + ".dat";

    std::ifstream reader(filename);
    std::vector<record> records;
    int K;   
    reader >> K;

    for (size_t i = 0; i < K; i++)
    {
        int value;
        bool flag;
        std::string rawdata;
        reader >> value;
        reader >> flag;        
        reader >> rawdata;

        auto data = (rawdata == "NULL")? nullptr : std::make_shared<std::string>(rawdata);
        records.push_back(record(value, flag, data));
    }
    reader.close();

    while (1)
    {
        auto q = from.pop();
        if(q == nullptr)
        {          
            to.push(q);
            break;
        }
        for(auto rec : records)
        {
            bool check = true;
            for(auto condition : q->conditions)
            {                
                
                if (!condition->eval(rec))
                {
                    check = false;
                    break;
                }
                
                
            }
            if(check)
            {
                q->hits.push_back(rec);
            }
        }
        to.push(q);
    }
    
}

void write(Pipe<query>& channel)
{
    std::ofstream out("output.txt");

    bool first = true;
    while (1)
    {
        query q = channel.pop();
        if (q == nullptr)
        {
            break;
        }
        if (q->hits.size() == 0)
        {
            if (first)
            {
                out << "{could not find records}" << std::endl;
                first = false;
            }
            else
            {
                out << std::endl << "{could not find records}" << std::endl;
            }
        }
        else
        {
            for(auto h : q->hits)
            {      
                if (first)
                {
                    out << h << std::endl; 
                    first = false;
                }
                else
                {
                    out << std::endl << h << std::endl; 
                }          
                
            }
        }
    }    
    out.close();
}

#endif // !PARTITION
