#pragma once

#include "sha256.hpp"

#include <vector>
#include <string>
#include <map>

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using std::vector;

class TxInput
{
public:
    TxInput(){}
    TxInput(int index, const string& id, const string& key_data):output_index(index),transaction_id(id),pub_key_data(key_data){}

public:
    const int     get_output_idx() const { return output_index; }
    const string& get_txid()       const { return transaction_id; }
    const bool    can_unlock(const string& data) const { return pub_key_data == data; }

private:
    template <class Serializer>
    void serialize(Serializer& serializer, unsigned int version)
    {
        serializer & output_index;
        serializer & transaction_id;
        serializer & pub_key_data;
    }

private:
    friend class boost::serialization::access;

    int    output_index;
    string transaction_id;
    string pub_key_data;
};

class TxOutput
{
public:
    TxOutput(){}
    TxOutput(int value, const string& key):value(value), pub_key(key){}

public:
    const int  get_value() const { return value; }
    const bool can_be_unlocked(const string& data) const { return pub_key == data; }

private:
    template <class Serializer>
    void serialize(Serializer& serializer, unsigned int version)
    {
        serializer & value;
        serializer & pub_key;
    }

private:
    friend class boost::serialization::access;

    int    value;
    string pub_key; 
};

using unspent_outputs = std::pair<std::map<string, vector<int>>, int>;

class Transaction
{
public:
    Transaction(){}
    Transaction(const string& from, const string& to, int amount, const unspent_outputs& unspent, bool is_base = false);
    Transaction(TxInput input, TxOutput output, bool is_base = true);

public:
    const string&          get_id()      const { return id; }
    const vector<TxInput>  get_inputs()  const { return inputs; }
    const vector<TxOutput> get_outputs() const { return outputs; }

    bool is_coinbase() { return is_base; }

public:
    string serialize();
    void   deserialize(const string& data);

private:
    void generate_id();

    template <class Serializer>
    void serialize(Serializer& serializer, const unsigned int version)
    {   
        serializer & id;
        serializer & inputs;
        serializer & outputs;
        serializer & is_base;
    }

private:
    friend class boost::serialization::access;

    string           id;
    vector<TxInput>  inputs;
    vector<TxOutput> outputs;

    bool is_base;
};

class TxVector
{
public:
    TxVector(){}
    TxVector(const Transaction& tx) { txs.push_back(tx); }

public:
    void   push_back(Transaction&& tx)      { txs.push_back(std::move(tx)); }
    void   push_back(const Transaction& tx) { txs.push_back(tx); }
    string get_txhash();

public:
    auto begin() { return txs.begin(); }
    auto end()   { return txs.end();   }

private:
    template <class Serializer>
    void serialize(Serializer& serializer, unsigned int version) { serializer & txs; }

private:
    friend class boost::serialization::access;

    vector<Transaction> txs;
};