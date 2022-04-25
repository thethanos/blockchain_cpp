#pragma once

#include "proof_of_work.hpp"
#include "leveldb.hpp"

#include <vector>
#include <memory>

using BlockPtr = std::unique_ptr<Block>;

class ChainIterator
{
public:
    ChainIterator(std::unique_ptr<LevelDB>& db_ptr, const std::string& last_hash):db_ptr(db_ptr), prev_hash(last_hash){}

public:
    Block next()
    {
        Block cur_block;
        cur_block.deserialize(db_ptr->get(prev_hash));

        prev_hash = cur_block.get_prev_hash();

        return cur_block;
    }

private:
    std::string prev_hash;
    std::unique_ptr<LevelDB>& db_ptr;
};

class Chain
{
public:
    Chain(const std::string& db_path, int difficulty = 12);

public:
    int  get_difficulty() { return difficulty; }
    void set_difficulty(int difficulty) { this->difficulty = difficulty; }
    void add_block(const std::string& data);

public:
    ChainIterator get_iter() { return ChainIterator(db_ptr, last_hash); }

private:
    int difficulty;
    std::string last_hash;

    std::unique_ptr<LevelDB> db_ptr;
};