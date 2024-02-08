#include <memory>
#include <cstdint>
#include <iostream>

struct Transaction
{
    std::uint64_t recipient_id;
    std::uint64_t amount;

    Transaction(std::uint64_t recipient_id, std::uint64_t amount)
        : recipient_id{recipient_id}, amount{amount}
    {
        // Assume there is code here to start a database transaction...
        std::cout << "Started transaction with recipient " << recipient_id << " and amount " << amount << std::endl;
    }

    ~Transaction()
    {
        // Assume there is code here to commit the database transaction...
        std::cout << "Completed transaction with recipient " << recipient_id << " and amount " << amount << std::endl;
    }
};

int main(int, char*[])
{
    // 1. Note the following code that creates a Transaction object on the heap with the new operator.
    //    Run the code at this stage.
    //    What is printed to terminal? Is the transaction completed properly? What is missing?

    auto* transaction1 = new Transaction{1, 100};

    // 2. Create a unique pointer to a Transaction object with "std::make_unique" with DIFFERENT arguments as transaction1.
    //    The arguments of this function are forwarded to the constructor of Transaction under the hood.
    //    https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
    //    Run the code at this stage.
    //    How does this behavior compare to part 1? What must unique pointer be doing under the hood on scope exit?

    // auto transaction2 = ...;

    // 3. Note the default initialized unique pointer to a Transaction object below.
    //    Print the value of ".get()" on transaction2 and transaction3 (this is the raw underlying pointer).
    //    What do you observe? What state are these objects in?

    std::unique_ptr<Transaction> transaction3;

    // 4. Try to assign transaction2 to transaction3.
    //    What happens? Why would it be a problem if this succeeded?
    //    Think about your answer to 2. What happens in the unique pointer destructor?

    // 5. Now use the "std::move" function to move transaction2 INTO transaction3.
    //    What happens to transaction2? What happens to transaction3? Print the value of ".get()" to confirm your answer.

    // 6. Create two more transactions transaction4 and transaction5 managed by unique pointers.
    //    Can you make trasaction4 complete before transaction5?
    //    Why does the standard method of creating two in a row NOT work? Think about destructor order.
    //    Hint: Curly brackets can be used to create scopes with a well-defined lifetime.
}
