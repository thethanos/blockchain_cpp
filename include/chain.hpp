#pragma once

#include "proof_of_work.hpp"
#include "leveldb.hpp"

#include <memory>

using BlockPtr = std::unique_ptr<Block>;

class ChainIterator
{
public:
    ChainIterator(std::unique_ptr<LevelDB>& db_ptr, const string& last_hash):db_ptr(db_ptr), prev_hash(last_hash){}

public:
    Block next()
    {
        Block cur_block;
        cur_block.deserialize(db_ptr->get(prev_hash));

        prev_hash = cur_block.get_prev_hash();

        return cur_block;
    }

private:
    string prev_hash;
    std::unique_ptr<LevelDB>& db_ptr;
};

using unspent_outputs = std::pair<std::map<string, vector<int>>, int>;

class Chain
{
public:
    Chain(const string& address, const string& db_path);
    Chain(const string& db_path);

public:
    int  get_difficulty() { return difficulty; }
    void set_difficulty(int difficulty) { this->difficulty = difficulty; }
    void add_block(TxVector transactions);

    TxVector         get_unspent_txs(const string& address);
    vector<TxOutput> get_utxo(const string& address); 
    unspent_outputs  get_unspent_outs(const string& address, int amount);

public:
    ChainIterator get_iter() { return ChainIterator(db_ptr, last_hash); }

private:
    int difficulty = 18;
    string last_hash;

    std::unique_ptr<LevelDB> db_ptr;
};