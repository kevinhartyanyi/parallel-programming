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
        std::cout << "bool condition" << std::endl;
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
        std::cout << "int condition" << std::endl;
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
        std::cout << "string condition" << std::endl;
        if (!rec.data)
        {
            std::cout << "Rec is Null" << std::endl;
        }
        else
            std::cout << "Rec is not Null" << std::endl;

        if (!data)
        {
            std::cout << "Data is Null" << std::endl;
        }
        else
        {
            std::cout << "Data is not Null" << std::endl;
        }      
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
    std::cout << "Read: " << std::endl;
    std::string filename = std::string("input") + std::to_string(part_num) + ".dat";
    std::cout << "Filename: " << filename << std::endl;

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
    std::cout << "Read End" << std::endl;
    for(auto r : records)
    {
        std::cout << "Record IS " << r << std::endl;  
    }

    while (1)
    {
        std::cout << "Quary " << std::this_thread::get_id() << std::endl;

        auto q = from.pop();
        std::cout << "HEREEEEEE" << std::endl;
        if(q == nullptr)
        {
            std::cout << "Nullptr" << std::endl;            
            to.push(q);
            break;
        }
        std::cout << "RECORDS" << std::endl;
        for(auto rec : records)
        {
            bool check = true;
            std::cout << "\n" << rec << std::endl;
            for(auto condition : q->conditions)
            {                
                std::cout << "Check Condition" << std::endl;
                
                if (!condition->eval(rec))
                {
                    std::cout << "False" << std::endl;
                    check = false;
                    break;
                }
                else
                {
                    std::cout << "True" << std::endl;
                }
                
                
            }
            if(check)
            {
                q->hits.push_back(rec);
            }
        }
        to.push(q);
        std::cout << "Query End\n" << std::endl;
    }
    
}

void write(Pipe<query>& channel)
{
    std::ofstream out("output.txt");

    std::cout << "OPEN output" << std::endl;
    bool first = true;
    while (1)
    {
        query q = channel.pop();
        if (q == nullptr)
        {
            std::cout << "BREAK" << std::endl;
            break;
        }
        std::cout << "HITS: " << q->hits.size() << std::endl;
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
