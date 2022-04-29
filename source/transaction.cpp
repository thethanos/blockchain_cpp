#include "transaction.hpp"

#include <sstream>

Transaction::Transaction(const string& from, const string& to, int amount, const unspent_outputs& unspent, bool is_base):is_base(is_base)
{   
    int accumulated = unspent.second;

    for (auto[tx_id, tx_outputs] : unspent.first)
    {
        for (auto& output : tx_outputs)
            this->inputs.push_back({output, tx_id, from});
    }

    this->outputs.push_back(TxOutput{amount, to});

    if (accumulated > amount) {
        outputs.push_back(TxOutput{accumulated - amount, from});
    }

    generate_id();
}

Transaction::Transaction(TxInput input, TxOutput output, bool is_base):is_base(is_base)
{
    inputs.push_back(std::move(input));
    outputs.push_back(std::move(output));

    generate_id();
}

void Transaction::generate_id()
{
    id = sha::sum256(this->serialize());
}

string Transaction::serialize()
{
    std::stringstream ostream;
    boost::archive::text_oarchive serializer(ostream);

    serializer << *this;

    return ostream.str();
}

void Transaction::deserialize(const string& data)
{
    std::stringstream istream;
    boost::archive::text_iarchive deserializer(istream);

    deserializer >> *this;
}

string TxVector::get_txhash()
{
    string data;
    for (auto& tx : txs)
        data.append(tx.serialize());

    return sha::sum256(data);
}