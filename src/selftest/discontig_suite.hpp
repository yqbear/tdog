//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : discontig_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_DISCONTIG_SUITE_H
#define TDOG_DISCONTIG_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#if defined(__COUNTER__)
#include "discontig_suite1.hpp"
#include "discontig_suite2.hpp"
#endif

#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test that we can split a suite across files.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(discontig_suite) // <- shared across files
{
  TDOG_TEST_CASE(perform_test)
  {
    // Can only do this with __COUNTER__ macro
    TDOG_SET_AUTHOR("Andy Thomas");

  #if defined(__COUNTER__)
    tdog::runner tr;

    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/discontig_suite.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::test1a) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::test1b) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::test2a) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::test2b) ) );

    // No failures
    TDOG_ASSERT_EQ(0, tr.run());

    // Test stats and status
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_FAILED, "*"));
    TDOG_ASSERT_EQ(4, tr.statistic_count(tdog::CNT_PASSED, "*"));

    TDOG_ASSERT_EQ(4, tr.statistic_count(tdog::CNT_PASSED, "discontig_suite::internal_suite::"));
    TDOG_ASSERT_EQ(tdog::TS_PASS_OK, tr.test_status("discontig_suite::internal_suite::test1a"));
    TDOG_ASSERT_EQ(tdog::TS_PASS_OK, tr.test_status("discontig_suite::internal_suite::test1b"));
    TDOG_ASSERT_EQ(tdog::TS_PASS_OK, tr.test_status("discontig_suite::internal_suite::test2a"));
    TDOG_ASSERT_EQ(tdog::TS_PASS_OK, tr.test_status("discontig_suite::internal_suite::test2b"));
  #else

    // See TDOG_COUNTER_ID
    TDOG_TEST_WARNING("IGNORED: __COUNTER__ macro not supported");

  #endif
  }

  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
