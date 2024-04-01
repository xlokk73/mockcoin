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

    bool is_valid() const;
};

// Check if the hash is valid
bool is_pow_valid(std::string hash);

// Hashing function
std::string sha256(const std::string &input);

// The main function of the Node
int main() {
    Block block = Block();

    std::cout << "Hello, World!" << std::endl;

    // Start the proof-of-work
    std::string hash = sha256(block.to_bytes());
    while (!is_pow_valid(hash)) {
        block.increment_nonce();
        hash = sha256(block.to_bytes());
    }

    std::cout << "Proof of work hash: " << hash << std::endl;
    std::cout << "Nonce: " << block.nonce << std::endl;


    return 0;
}

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

bool Block::is_valid() const {
    // Check if the transactions are valid
    for (const Transaction& transaction : transactions) {
        if (!transaction.is_signature_valid()) {
            return false;
        }
    }

}

bool Transaction::is_signature_valid() const {
    std::string hash = sha256(std::to_string(previous_transaction_index) + recipient_public_key);
    return hash == signature;
}

std::string sha256(const std::string &input) {
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource s(input, true,
                             new CryptoPP::HashFilter(hash,
                                                      new CryptoPP::HexEncoder(
                                                              new CryptoPP::StringSink(digest))));
    return digest;
}

bool is_pow_valid(std::string hash) {
    // Check if hash starts with 5 zeros
    return hash.substr(0, 5) == "00000";
}