//
// Created by manwel on 01/04/2024.
//

#ifndef MOCKCOIN_TRANSACTION_H
#define MOCKCOIN_TRANSACTION_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>


class Transaction {
public:
    std::string recipient_public_key;
    std::string sender_public_key;
    double amount; // Amount of coins being transferred
    uint previous_transaction_index; // Index of the previous transaction in the block
    std::string signature; // Hash of previous transaction + recipient public key
    bool is_signature_valid() const;

    /**
     * TODO:
     * - Add transaction fees
     * - Add proper signature verification
     */
};

#endif //MOCKCOIN_TRANSACTION_H
