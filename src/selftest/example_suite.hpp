//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : example_suite.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_EXAMPLE_SUITE_H
#define TDOG_EXAMPLE_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Example code used in documentation. This makes sure the examples are
// compilable!

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Some function declaration
bool is_prime(int a)
{
  // Not real
  switch(a)
  {
  case 47: return true;
  default: return false;
  }
}

TDOG_SUITE(example_suite)
{
  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    // Global test
    TDOG_TEST_CASE(is_prime_func)
    {
      TDOG_SET_AUTHOR("Kuiper");
      TDOG_ASSERT( is_prime(47) );
      TDOG_ASSERT( !is_prime(8) );
      TDOG_ASSERT( !is_prime(21) );
    }

    // default
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(is_prime_func)
  {
    TDOG_SET_AUTHOR("Kuiper");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_SUMMARY, "./reports/internal/example_is_prime.txt");

    tr.register_test( TDOG_GET_TCPTR(internal_suite::is_prime_func) );

    tr.run();
  }

  TDOG_TEST_CASE(enum_print)
  {
    TDOG_SET_AUTHOR("Kuiper");

    enum {VAL0, VAL1};
    TDOG_PRINTF("Enum value: %g", VAL1);
  }

  struct user_type77
  {
    static const int value = 77;
  };

  struct user_type2001
  {
    static const int value = 2001;
  };

  TDOG_DEFINE_REPEATED(inject_type)
  {
    TDOG_PRINTF("Type value: %d", USER_TYPE::value);
  }

  TDOG_TEST_REPEATED(inject_test1, inject_type, user_type77);
  TDOG_TEST_REPEATED(inject_test2, inject_type, user_type2001);

  // example_suite
  TDOG_CLOSE_SUITE
}


// The beast example in standard namespace
namespace project_namespace
{
  TDOG_SUITE(example_suite)
  {
    TDOG_SUITE(beast_suite)
    {
      // Parameter injection - see main()
      int INIT_BEAST = 0;

      TDOG_TEST_CASE(neighbor_test)
      {
        int neighbor_of_beast = INIT_BEAST + 2;
        TDOG_ASSERT_EQ(668, neighbor_of_beast); // Passes
      }

      // beast_suite
      TDOG_CLOSE_SUITE
    }

    // example_suite
    TDOG_CLOSE_SUITE
  }
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
