#include "chain.hpp"

Chain::Chain(const std::string& db_path, int difficulty)
{   
    this->difficulty = difficulty;

    db_ptr = std::make_unique<LevelDB>(db_path);

    std::string hash = db_ptr->get("last_hash");
    if (hash.empty())
    {
        auto result = prove("", "Genesis", difficulty);

        Block genesis("", "Genesis", result.first, result.second);

        db_ptr->put(genesis.get_hash(), genesis.serialize());
        db_ptr->put("last_hash", genesis.get_hash());

        last_hash = genesis.get_hash();
    }
    else
        last_hash = hash;
}

void Chain::add_block(const std::string& data)
{
    std::string hash = db_ptr->get("last_hash");
    if (hash.empty())
        throw std::runtime_error("empty hash");

    auto result = prove(hash, data, difficulty);

    Block new_block(hash, data, result.first, result.second);
    db_ptr->put(new_block.get_hash(), new_block.serialize());
    db_ptr->put("last_hash", new_block.get_hash());

    last_hash = new_block.get_hash();

    std::cout << "new block added!\n";    
}