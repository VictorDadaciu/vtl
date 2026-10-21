#pragma once

#include <memory>
#include <print>
#include <source_location>
#include <string>

namespace vtl
{
class suite
{
public:
    virtual void set_up() {}
    virtual void tear_down() {}

    virtual const char* name() const noexcept = 0;
    virtual size_t run() = 0;

    virtual ~suite() = default;
};

class suite_register final
{
public:
    suite_register(std::shared_ptr<suite>);
};

template <typename T>
struct suite_helper
{
    inline static std::string to_string(const T& x) requires (requires (const T& x) { std::to_string(x); })
    {
        return std::format("{}", x);
    }

    inline static bool equals(const T& actual, const T& expected) requires (requires (const T& a, const T& b) { a == b; })
    {
        return actual == expected;
    }
};

inline std::string source_location_formatter(std::source_location&& sl)
{
    return std::format("{}:{}:{}", sl.file_name(), sl.line(), sl.column());
}

template<typename T>
inline void assert_equals(const T& actual, const T& expected, std::source_location&& sl=std::source_location::current())
{
    if (!suite_helper<T>::equals(actual, expected))
        throw std::invalid_argument(std::format("{}: assertion false {} != {}",
            source_location_formatter(std::move(sl)),
            suite_helper<T>::to_string(actual),
            suite_helper<T>::to_string(expected)
        ));
}

inline void assert(bool actual, std::source_location&& sl=std::source_location::current())
{
    if (!actual)
        throw std::invalid_argument(std::format("{}: assertion false",
            source_location_formatter(std::move(sl))
        ));
}

void run();
}

#define VTL_TESTS_BEGIN(suite_name)               \
const char* name() const noexcept override        \
{                                                 \
    return #suite_name;                           \
}                                                 \
                                                  \
size_t run() override                             \
{                                                 \
    size_t n{};

#define VTL_TEST(case, ...)                           \
{                                                     \
    set_up();                                         \
    try                                               \
    {                                                 \
        case(__VA_ARGS__);                            \
        std::println("|   {}: OK", #case);            \
    }                                                 \
    catch(const std::exception& e)                    \
    {                                                 \
        ++n;                                          \
        std::println("|   {}: FAILED", #case);        \
        std::println("|     {}", e.what());           \
    }                                                 \
    tear_down();                                      \
}

#define VTL_TESTS_END()        \
    return n;                  \
}

#define VTL_REGISTER_SUITE(suite_name)                                                            \
inline static vtl::suite_register suite_name##__COUNTER__(std::make_shared<suite_name>());

#define VTL_EQUALS(actual, expected) vtl::assert_equals(actual, expected)
#define VTL_ASSERT(actual) vtl::assert(actual)
