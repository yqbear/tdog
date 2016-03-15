//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : error_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_ERROR_SUITE_H
#define TDOG_ERROR_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

#include <stdexcept>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test unhandle exception.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(error_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    TDOG_TEST_CASE(throw_runtime)
    {
      TDOG_ASSERT(true);
      throw std::runtime_error("The what message");
    }

    TDOG_TEST_CASE(throw_int)
    {
      TDOG_ASSERT(true);
      throw 668;
    }

    // internal_suite
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(error_test)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/error_test.txt");

    // Register tests with local runner
    tr.register_test( TDOG_GET_TCPTR(internal_suite::throw_runtime) );
    tr.register_test( TDOG_GET_TCPTR(internal_suite::throw_int) );

    // Both errors
    TDOG_ASSERT_EQ(2, tr.run());

    // Error count
    tdog::i64_t error_cnt = static_cast<int>(tr.statistic_count(tdog::CNT_ERRORS, "*"));
    TDOG_ASSERT_EQ(2, error_cnt);

    // Check for message
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string s = stm.str();

    TDOG_ASSERT_NEQ(NPOS, s.find("runtime_error")); // <- exception name
    TDOG_ASSERT_NEQ(NPOS, s.find("The what message"));
  }

  // error_suite
  TDOG_CLOSE_SUITE
}
//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
