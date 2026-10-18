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
    std::cout << "Running VTL tests...\n";
    for (auto& s : suites)
    {
        try
        {
            s->run();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
}