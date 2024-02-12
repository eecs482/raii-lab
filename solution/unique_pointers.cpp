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

    // We see that the transaction is started properly:
    //     Started transaction with recipient 1 and amount 200
    // However, the transaction is never completed, as the delete operator is never called, which never calls the destructor.

    // 2. Create a unique pointer to a Transaction object with "std::make_unique" with DIFFERENT arguments as transaction1.
    //    The arguments of this function are forwarded to the constructor of Transaction under the hood.
    //    https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
    //    Run the code at this stage.
    //    How does this behavior compare to part 1? What must unique pointer be doing under the hood on scope exit?

    auto transaction2 = std::make_unique<Transaction>(2, 200);

    // We see that the transaction is completed properly:
    //     Started transaction with recipient 2 and amount 1000000
    //     Completed transaction with recipient 2 and amount 1000000
    // This means the unique pointer must have called delete of its managed object in its destructor.
    // Delete would then call the destructor of the managed object, which in our case prints the completion message.

    // 3. Note the default initialized unique pointer to a Transaction object below.
    //    Print the value of ".get()" on transaction2 and transaction3 (this is the raw underlying pointer).
    //    What do you observe? What state are these objects in?

    std::unique_ptr<Transaction> transaction3;

    std::cout << "transaction2.get(): " << transaction2.get() << std::endl;
    std::cout << "transaction3.get(): " << transaction3.get() << std::endl;

    // Note: unique_ptr also can be directly printed: https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_ltlt

    // We observe that transaction3 is in an empty state, as its underlying pointer is null.
    // transaction2 is in a valid state, as its underlying pointer is non-null.

    // 4. Try to assign transaction2 to transaction3.
    //    What happens? Why would it be a problem if this succeeded?
    //    Think about your answer to 2. What happens in the unique pointer destructor?

    // The following will fail to even compile:
    // transaction3 = transaction2;
    // If this succeeded then BOTH objects would have a pointer to the same underlying object.
    // Their destructors would both call delete on the same pointer, which would be undefined behavior.
    // This is known as a double free and could be exploited by an attacker to execute arbitrary code.

    // 5. Now use the "std::move" function to move transaction2 INTO transaction3.
    //    What happens to transaction2? What happens to transaction3? Print the value of ".get()" to confirm your answer.

    transaction3 = std::move(transaction2);

    std::cout << "transaction2.get(): " << transaction2.get() << std::endl;
    std::cout << "transaction3.get(): " << transaction3.get() << std::endl;

    // transaction2's pointer was moved to transaction3, leaving transaction2 in an empty state.
    // The invariant that one unique pointer owns the underlying object is preserved.
    // Therefore only transaction3 will call delete (instead of transaction2 which will now do nothing) on scope exit.
    // Note that all std::move does is cast the unique pointer to an rvalue reference, which is then used to call the move constructor.

    // 6. Create two more transactions transaction4 and transaction5 managed by unique pointers.
    //    Can you make transaction4 complete before transaction5?
    //    Why does the standard method of creating two in a row NOT work? Think about destructor order.
    //    Hint: Curly brackets can be used to create scopes with a well-defined lifetime.

    {
        auto transaction4 = std::make_unique<Transaction>(4, 400);
    }
    auto transaction5 = std::make_unique<Transaction>(5, 500);

    // Destructors are called in reverse order of construction.
    // This is why just creating two in a row does not work.
    //
    // Alternative solution using move semantics and a temporary scope:
    //
    // auto transaction4 = std::make_unique<Transaction>(4, 400);
    // auto transaction5 = std::make_unique<Transaction>(5, 500);
    // {
    //     auto _ = std::move(transaction4);
    // }
}
