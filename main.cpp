#include "chain.hpp"

#include "leveldb.hpp"

#include <iostream>
#include <boost/program_options.hpp>

using namespace boost::program_options;

void print_usage()
{
    std::cout << "Usage: \n";
    std::cout << "--create --user ADDRESS --path DB_PATH\n";
    std::cout << "--send --from ADDRESS --amount AMOUNT --to ADDRESS --path DB_PATH\n";
    std::cout << "--print --path DB_PATH\n";
    std::cout << "--balance ADDRESS -path DB_PATH\n";
}

int main(int argc, char** argv)
{
    options_description desc;
    desc.add_options()
        ("create", "create new blockchain")
        ("user", value<std::string>(), "blockchain user address")
        ("send", "send coins to another user")
        ("path", value<std::string>(), "path to the storage")
        ("from", value<std::string>(), "sender")
        ("amount", value<int>(), "amount")
        ("to", value<std::string>(), "receiver")
        ("balance", value<std::string>(), "print balance")
        ("print", "prints the chain");

    variables_map vm;    
    store(parse_command_line(argc, argv, desc), vm);

    if (!vm.count("path"))
    {
        std::cout << "path is not specified\n";
        return 0;
    }

    if (vm.count("create"))
    {
        if (!vm.count("user"))
        {
            std::cout << "user is not specified\n";
            return 0;
        }

        Chain chain(vm["user"].as<std::string>(), vm["path"].as<std::string>());
        std::cout << "new blockchain has been created\n";
        return 0;
    }

    if (vm.count("send"))
    {   
        Chain chain(vm["path"].as<std::string>());

        auto from   = vm["from"].as<std::string>();
        auto to     = vm["to"].as<std::string>();
        auto amount = vm["amount"].as<int>();

        Transaction tx(from, to, amount, chain.get_unspent_outs(from, amount));

        chain.add_block(TxVector(tx));
        return 0;
    }

    if (vm.count("balance"))
    {
        Chain chain(vm["path"].as<std::string>());

        std::string address = vm["balance"].as<std::string>();

        auto outputs = chain.get_utxo(address);

        int balance(0);
        for (auto& out : outputs) 
            balance += out.get_value();

        std::cout << "User: " << address << " balance: " << balance << '\n';

        return 0;
    }

    if (vm.count("print"))
    {   
        Chain chain(vm["path"].as<std::string>());

        auto iter = chain.get_iter();

        while(true)
        {
            auto block = iter.next();

            if (validate(&block, chain.get_difficulty()))
                std::cout << "Valid: true\n";
            else
                std::cout << "Valid: false\n";

            std::cout << "Prev: " << block.get_prev_hash() << '\n';
            std::cout << "Hash: " << block.get_hash() << "\n\n";

            if (block.get_prev_hash().empty())
                return 0;
        }
    }

    print_usage();

    return 0;
}