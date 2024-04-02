//
// Created by manwel on 01/04/2024.
//
#include "Block.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include "Transaction.h"

Block::Block() {
    nonce = 0;
}

Block::Block(const Block &previous_block) {
    nonce = 0;
    balances = previous_block.balances;
}

bool Block::add_transaction(const Transaction &transaction) {
    // Check if the sender has enough balance for the transaction
    if (balances[transaction.sender_public_key] < transaction.amount) {
        return false;
    }

    // Check if the transaction amount is positive
    if (transaction.amount <= 0) {
        return false;
    }

    // Check if the sender and recipient are different
    if (transaction.sender_public_key == transaction.recipient_public_key) {
        return false;
    }

    // Check if transaction is signature
    if (transaction.is_signature_valid()) {
        return false;
    }

    // Update the balance of the sender and recipient
    balances[transaction.sender_public_key] -= transaction.amount;
    balances[transaction.recipient_public_key] += transaction.amount;

    // Add the transaction to the list of transactions
    transactions.push_back(transaction);

    return true;
}

// Serialize the block
std::string Block::to_bytes() {
    std::stringstream ss;
    ss << nonce;
    for (Transaction &transaction : transactions) {
        ss << transaction.recipient_public_key;
        ss << transaction.previous_transaction_index;
        ss << transaction.signature;
    }
    return ss.str();
}

// Deserialize the block
void Block::from_bytes(std::string bytes) {
    std::stringstream ss(bytes);
    ss >> nonce;
    while (!ss.eof()) {
        Transaction transaction;
        ss >> transaction.recipient_public_key;
        ss >> transaction.previous_transaction_index;
        ss >> transaction.signature;
        transactions.push_back(transaction);
    }
}

void Block::increment_nonce() {
    this->nonce++;
}

bool Block::is_valid() {
    // Check if the transactions are valid
    for (const Transaction& transaction : transactions) {
        if (!transaction.is_signature_valid()) {
            return false;
        }
    }

    // Check if the hash is valid
    return is_pow_valid(sha256(this.to_bytes()));
}

std::string Block::sha256(const std::string &input) {
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource s(input, true,
                             new CryptoPP::HashFilter(hash,
                                                      new CryptoPP::HexEncoder(
                                                              new CryptoPP::StringSink(digest))));
    return digest;
}

bool Block::is_pow_valid(const std::string& hash) {
    // Check if hash starts with 5 zeros
    return hash.substr(0, 5) == "00000";
}

std::string Block::calculate_pow_hash() {
    std::string hash;
    do {
        hash = sha256(this->to_bytes());
        increment_nonce();
    } while (!is_pow_valid(hash));
    return hash;
}
