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