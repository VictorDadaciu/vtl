#pragma once

#include <string>
#include <iostream>
#include <memory>

namespace vtl
{
class suite
{
public:
    virtual void set_up() = 0;
    virtual void tear_down() = 0;

    virtual const char* name() const noexcept = 0;

    virtual void run() = 0;

    virtual ~suite() = default;
};

class suite_register final
{
public:
    suite_register(std::shared_ptr<suite>);
};

void run();
}

#define VTL_TESTS_BEGIN(suite_name)                          \
const char* name() const noexcept override                   \
{                                                            \
    return #suite_name;                                      \
}                                                            \
                                                             \
void run() override                                          \
{                                                            \
    std::cout << "\tRunning suite \"suite_name\"...\n";

#define VTL_TEST(case)                     \
{                                          \
    set_up();                              \
    case();                                \
    tear_down();                           \
    std::cout << "\t\t#case: OK\n";        \
}

#define VTL_TESTS_END() }

#define VTL_REGISTER_SUITE(suite_name)                                                       \
inline static suite_register suite_name##__COUNTER__(std::make_shared<suite_name>());
