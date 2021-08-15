The deadlock of two tales
=====

This example shows how a deadlock can be achieved between two copiers: each tries to copy the idea of another.
Then __when there is no new idea from the opponent__, both wait __forever__ for new ideas, which leads to __deadlock__!

## The Copier: Learning is Copying!

When another opponent has a higher `idea` value, the copier tries to copy from the target by calling `copy(Copier&)` function.
Note that, both opponents must be instances of `Copier` class.

```cpp
class Copier {
public:
    Copier(const int& idea) : _idea(idea) {}
    Copier(const Copier& another) {
        this->_idea = another.idea();
    }
    virtual ~Copier() = default;

    void copy(Copier& another) {
        this->_idea += another.idea();
    }

    int idea() const {
        return this->_idea;
    }

    bool is_nothing_left_to_copy(const std::vector<Copier>& targets) {
        int sum_idea = 0;

        for (auto& target : targets) {
            sum_idea += target.idea();
        }

        if (this->_idea >= sum_idea) {
            std::cerr << "There is nothing to copy from them" << std::endl;
            return true;
        }
        else {
            std::cout << "Oh, they still have something for us to copy" << std::endl;
            return false;
        }
    }

private:
    int _idea;
};
```

There is also a method `is_nothing_left_to_copy(const std::vector<Copier>&)` to check whether the opponents still have ideas for the `Copier` to copy.

## Deadlock: Everyone is just waiting for ideas from others!

```cpp
#include "copy.hpp"

std::shared_ptr<Copier> copierA, copierB;
std::mutex mtxA, mtxB;

#ifdef _WIN32
#include <windows.h>

void sleep(unsigned milliseconds)
{
    Sleep(milliseconds);
}
#else
#include <unistd.h>

void sleep(unsigned milliseconds)
{
    usleep(milliseconds * 1000);
}
#endif

void copy(int n) {
    for (int i = n * 10; i < n * 10 + 10; i++) {
        if (copierA->idea() < copierB->idea()) {
            std::cout << "A(idea :" << copierA->idea() << ") needs to copy from B(idea :" << copierB->idea() << ") ..." << std::endl;
            std::lock_guard<std::mutex> lockA(mtxA);
            std::lock_guard<std::mutex> lockB(mtxB);
            copierA->copy(*copierB);
            sleep(100);
        }
        else {
            std::cout << "B(idea :" << copierB->idea() << ") needs to copy from A(idea :" << copierA->idea() << ") ..." << std::endl;
            std::lock_guard<std::mutex> lockB(mtxB);
            std::lock_guard<std::mutex> lockA(mtxA);
            copierB->copy(*copierA);
            sleep(100);
        }
    }
}

int main() {
    copierA = std::make_shared<Copier>(Copier(1));
    copierB = std::make_shared<Copier>(Copier(1));
    std::cout << copierA->idea() << std::endl;
    std::cout << copierB->idea() << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread(copy, i));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << copierA->idea() << std::endl;
    std::cout << copierB->idea() << std::endl;
    return 0;
}
```

In the `copy(int)` function, the following rule is used to decide who will copy from whom:

* if A has lower `idea` value than `B`'s than A must copy from B, and vice versa.

## The result: Forever waiting

```bash
.\Debug\deadlock.exe
1
1
B(idea :1) needs to copy from A(idea :B(idea :1) needs to copy from A(idea :1B(idea :B(idea :1) needs to copy from A(idea :1) ...
) ...
1) needs to copy from A(idea :1) ...
B(idea :1) needs to copy from A(idea :1) ...
B(idea :1) needs to copy from A(idea :1) ...
1) ...
B(idea :1) needs to copy from A(idea :B(idea :1) needs to copy from A(idea :1) ...
1) ...
B(idea :1) needs to copy from A(idea :1) ...
B(idea :1) needs to copy from A(idea :1) ...
A(idea :1) needs to copy from B(idea :2) ...
A(idea :1) needs to copy from B(idea :3) ...
B(idea :3) needs to copy from A(idea :4) ...
A(idea :4) needs to copy from B(idea :7) ...
```

It stucks of waiting each other to release the lock now.