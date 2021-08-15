#ifndef COPY_HPP_
#define COPY_HPP_
#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>

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

#endif

