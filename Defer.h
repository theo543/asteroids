#ifndef OOP_DEFER_H
#define OOP_DEFER_H
#include <functional>

#include <SFML/System.hpp>

/// Simple RAII class to run cleanup code when it goes out of scope.
class DeferWrapper : sf::NonCopyable {
    std::function<void()> deferred;
public:
    [[maybe_unused]] explicit DeferWrapper(std::function<void()> f) : deferred(std::move(f)) {}
    [[maybe_unused]] void cancel() {deferred = [](){};}
    ~DeferWrapper() {deferred();}
};

#endif //OOP_DEFER_H
