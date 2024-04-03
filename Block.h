//
// Created by manwel on 01/04/2024.
//

#ifndef MOCKCOIN_BLOCK_H
#define MOCKCOIN_BLOCK_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <unordered_map>
#include "Transaction.h"

class Block {
public:
    uint nonce;
    std::unordered_map<std::string, double> balances; // Balances of each public key
    std::vector<Transaction> transactions;     // List of transactions

    Block();
    Block(const Block& previous_block);

    void increment_nonce();

    // Add a transaction to the block
    bool add_transaction(const Transaction& transaction);

    // Serialize the block
    std::string to_bytes();

    // Deserialize the block
    void from_bytes(std::string bytes);

    bool is_valid();

    static std::string sha256(const std::string &input);

    // Calculate the proof-of-work hash
    std::string calculate_pow_hash();

    // Checks if the proof-of-work hash is valid
    static bool is_pow_valid(const std::string& hash);
};


#endif //MOCKCOIN_BLOCK_H
