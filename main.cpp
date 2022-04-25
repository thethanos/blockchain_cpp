#include "chain.hpp"

#include "leveldb.hpp"

#include <iostream>
#include <boost/program_options.hpp>

using namespace boost::program_options;

void print_usage()
{
    std::cout << "Usage: \n";
    std::cout << "--add DATA_STRING\n";
    std::cout << "--print\n";
}

int main(int argc, char** argv)
{
    options_description desc;
    desc.add_options()
        ("add", value<std::string>(), "adds block to the chain")
        ("print", "prints the chain");

    variables_map vm;    
    store(parse_command_line(argc, argv, desc), vm);

    Chain chain("/home/master/study_workspace/leveldb", 18);

    if (vm.count("add"))
    {
        std::string data = vm["add"].as<std::string>();
        if (data.empty())
        {
            std::cout << "no data provided\n";
            print_usage();
            return 0;
        }

        chain.add_block(data);
        return 0;
    }

    if (vm.count("print"))
    {
        auto iter = chain.get_iter();

        while(true)
        {
            auto block = iter.next();

            if (validate(&block, chain.get_difficulty()))
                std::cout << "Valid: true\n";
            else
                std::cout << "Valid: false\n";

            std::cout << "Prev: " << block.get_prev_hash() << '\n';
            std::cout << "Data: " << block.get_data() << '\n';
            std::cout << "Hash: " << block.get_hash() << "\n\n";

            if (block.get_prev_hash().empty())
                return 0;
        }
    }

    print_usage();

    return 0;
}