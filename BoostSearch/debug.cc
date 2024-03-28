#include "searcher.hpp"
#include <iostream>
#include <string>

int main()
{
    Searching::Searcher *search = new Searching::Searcher();

    const std::string input = "data/output/raw";
    search->InitSearcher(input);

    std::string query;
    std::string json_string;
    char buffer[1024];
    while (true)
    {
        std::cout << "Please Enter Your Search Query# ";

        fgets(buffer, sizeof(buffer) - 1, stdin);

        buffer[strlen(buffer) - 1] = 0;

        query = buffer;

        search->Search(query, json_string);

        std::cout << json_string << std::endl;
    }

    delete search;

    return 0;
}
