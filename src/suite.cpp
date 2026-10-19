#include "suite.h"

#include <exception>
#include <vector>

namespace vtl
{
namespace
{
    std::vector<std::shared_ptr<suite>> suites{};
}

suite_register::suite_register(std::shared_ptr<suite> s)
{
    if (!s) return;
    suites.push_back(s);
}

void run()
{
    size_t total{};
    std::println("Running vtl tests...");
    for (auto& s : suites)
    {
        std::println("================");
        std::println("o {}:", s->name());
        size_t n = s->run();
        total += n;
        std::println("o {} failure(s)", n);
    }
    std::println("================");
    std::println("o {} total failure(s)", total);
}
}