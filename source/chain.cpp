#include "chain.hpp"

#include <set>

Chain::Chain(const string& address, const string& db_path)
{   
    db_ptr = std::make_unique<LevelDB>(db_path);

    string hash = db_ptr->get("last_hash");
    if (!hash.empty())
        throw std::runtime_error("blockchain already exists");

    TxVector txs;
    txs.push_back({TxInput{-1, "", "Genesis"}, TxOutput{100, address}});

    auto result = prove("", txs.get_txhash(), difficulty);

    Block genesis("", txs, result.first, result.second);

    db_ptr->put(genesis.get_hash(), genesis.serialize());
    db_ptr->put("last_hash", genesis.get_hash());

    last_hash = genesis.get_hash();
}

Chain::Chain(const string& db_path)
{
    db_ptr = std::make_unique<LevelDB>(db_path);

    last_hash = db_ptr->get("last_hash");
    if (last_hash.empty())
        throw std::runtime_error("empty hash");
}

void Chain::add_block(TxVector transactions)
{
    string hash = db_ptr->get("last_hash");
    if (hash.empty())
        throw std::runtime_error("empty hash");

    auto result = prove(hash, transactions.get_txhash(), difficulty);

    Block new_block(hash, transactions, result.first, result.second);
    db_ptr->put(new_block.get_hash(), new_block.serialize());
    db_ptr->put("last_hash", new_block.get_hash());

    last_hash = new_block.get_hash();

    std::cout << "new block added!\n";    
}

TxVector Chain::get_unspent_txs(const string& address)
{
    TxVector unspent;
    std::map<string, std::set<int>> spent;

    auto iter = get_iter();

    while(true)
    {
        auto block = iter.next();

        for (auto& tx : block.get_txvector())
        {
            string tx_id = tx.get_id();
            auto outputs = tx.get_outputs();

            for (int output_id(0); output_id < outputs.size(); output_id++)
            {
                if (spent.count(tx_id))
                {
                    if(spent[tx_id].count(output_id))
                        continue;
                }

                if (outputs[output_id].can_be_unlocked(address))
                    unspent.push_back(tx);
            }
            if (!tx.is_coinbase())
            {
                for (auto& input : tx.get_inputs())
                {
                    if (input.can_unlock(address))
                        spent[input.get_txid()].insert(input.get_output_idx());
                }
            }
        }

        if (block.get_prev_hash().empty())
            break;
    }

    return unspent;
}

vector<TxOutput> Chain::get_utxo(const string& address)
{
    auto unspent = get_unspent_txs(address);

    vector<TxOutput> outputs;
    for (auto& tx : unspent)
    {
        for (auto& out : tx.get_outputs())
        {
            if (out.can_be_unlocked(address))
                outputs.push_back(out);
        }
    }

    return outputs;
}

unspent_outputs Chain::get_unspent_outs(const string& address, int amount)
{
    int accumulated(0);
    std::map<string, vector<int>> unspent_out;

    auto unspent_txs = get_unspent_txs(address);

    for (auto& tx : unspent_txs)
    {   
        string tx_id = tx.get_id();
        auto outputs = tx.get_outputs();

        for (int output_id(0); output_id < outputs.size(); output_id++)
        {
            if (outputs[output_id].can_be_unlocked(address) && accumulated < amount)
            {
                accumulated += outputs[output_id].get_value();
                unspent_out[tx_id].push_back(output_id);

                if (accumulated >= amount)
                    return std::make_pair(unspent_out, accumulated);
            }
        }
    }

    return std::make_pair(unspent_out, accumulated);
}