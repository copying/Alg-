#include <catch.hpp>


#include "../include/call_if.cxx"
#include "../include/utils/void_optional.cxx"

#include <optional>

using namespace alg;



TEST_CASE("call_if makes a new functor that calls the function if, with the same arguments, the conditioning functions returns true", "[call_if]") {

    SECTION("Normal usage") {
        auto f10div = call_if {
            [](double i) { return 10.0/i; },
            [](double i) { return i != 0.0; }
        };

        REQUIRE( static_cast<bool>(f10div(10)) );
        REQUIRE( *(f10div(10)) == 1.0 );

        REQUIRE_FALSE( static_cast<bool>(f10div(0)) );
    }


    SECTION("Force the return function of a functor to be std::optional") {
        auto example = call_if {
            [](int i) { return i; }
        };

        REQUIRE( static_cast<bool>(example(10)) );
        REQUIRE( *(example(10)) == 10 );
        REQUIRE( static_cast<bool>(example(0)) );
        REQUIRE( *(example(0)) == 0 );

        REQUIRE(std::is_same_v<
            decltype(example(0)),
            std::optional<int>
        >);
    }


    SECTION("No arguments") {
        auto example = call_if {
            []{ return 0; }
        };

        REQUIRE( static_cast<bool>(example()) );
        REQUIRE(std::is_same_v<
            decltype(example()),
            std::optional<int>
        >);
    }

    SECTION("void as a return type") {
        auto example = call_if {
            []{}
        };

        REQUIRE( static_cast<bool>(example()) );
        REQUIRE(std::is_same_v<
            decltype(example()),
            utils::void_optional
        >);
    }

    SECTION("Maintain constexpr-ness") {
        constexpr auto example = call_if {
            []() constexpr -> int { return 5; }
        };

        constexpr int const result = *(example());
        REQUIRE( result == 5 );
    }

    SECTION("Maintain noexcept-ness") {
        auto example = call_if {
            []() noexcept -> int { return 5; }
        };

        REQUIRE( noexcept(example()) );
    }
}
