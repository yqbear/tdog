//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : status_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_STATUS_SUITE_H
#define TDOG_STATUS_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

#include <stdexcept>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test status and statistics.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(status_suite)
{
  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    TDOG_TEST_CASE(pass_default)
    {
      // Empty
    }

    TDOG_TEST_CASE(pass_assert)
    {
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(fail_assert)
    {
      TDOG_ASSERT(true);
      TDOG_ASSERT(false);
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(fail_macro)
    {
      TDOG_ASSERT(true);
      TDOG_TEST_FAIL("fail macro");
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(error_throw)
    {
      TDOG_ASSERT(true);
      throw std::runtime_error("message");
    }

    TDOG_TEST_CASE(error_macro)
    {
      TDOG_ASSERT(true);
      TDOG_TEST_ERROR("error macro");
    }

    TDOG_TEST_CASE(wait1)
    {
      TDOG_TEST_SLEEP(500);
    }

    TDOG_TEST_CASE(wait2)
    {
      TDOG_TEST_SLEEP(1000);
    }

    TDOG_TEST_CASE(disabled_test)
    {
      // To be disabled
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(skip_test)
    {
      // Will be disabled when run
      TDOG_ASSERT(true);
      TDOG_PRINT("Skipping now...");

      TDOG_SKIP_TEST();
    }

    TDOG_SUITE(nested_suite)
    {
      TDOG_TEST_CASE(pass1)
      {
        TDOG_ASSERT(true);
      }

      TDOG_TEST_CASE(pass2)
      {
        TDOG_ASSERT(true);
      }

      // nested suite
      TDOG_CLOSE_SUITE
    }

    // internal_suite
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(status_test)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/status_suite.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_default) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fail_assert) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fail_macro) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::error_throw) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::error_macro) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::wait1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::wait2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::disabled_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::skip_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass2) ) );

    // Disable test
    TDOG_ASSERT_EQ(1, tr.set_enabled("status_suite::internal_suite::disabled_test", false));

    // Check initial status
    TDOG_ASSERT_EQ(tdog::TS_NOT_EXIST, tr.test_status("status_suite::internal_suite")); // <- not valid
    TDOG_ASSERT_EQ(tdog::TS_NOT_EXIST, tr.test_status("status_suite::internal_suite::")); // <- not valid
    TDOG_ASSERT_EQ(tdog::TS_NOT_EXIST, tr.test_status("status_suite::internal_suite::*")); // <- not valid
    TDOG_ASSERT_EQ(tdog::TS_NOT_EXIST, tr.test_status("status_suite::internal_suite::wrong_name")); // <- not valid

    TDOG_ASSERT_EQ(tdog::TS_READY, tr.test_status("status_suite::internal_suite::pass_default"));

    TDOG_ASSERT_EQ(tdog::TS_DISABLED, tr.test_status("status_suite::internal_suite::disabled_test"));

    // Check counts
    tdog::i64_t total = tr.statistic_count(tdog::CNT_TOTAL, "*");
    TDOG_ASSERT_EQ(12, total); // <- must update with new tests
    TDOG_ASSERT_EQ(total, tr.statistic_count(tdog::CNT_TOTAL, "::*")); // <- any depth
    TDOG_ASSERT_EQ(2, tr.statistic_count(tdog::CNT_TOTAL, "status_suite::internal_suite::nested_suite::"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_SKIPPED, "*")); // <- doesn't know about the skip test yet
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_SKIPPED, "status_suite::internal_suite::"));
    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_DISABLED, "*"));
    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_DISABLED, "status_suite::internal_suite::"));

    // Nothing directly in the default suite
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_TOTAL, "::"));

    // RUN
    // Expect 4 failures/errors
    int run_rslt = tr.run();
    TDOG_ASSERT_EQ(4, run_rslt);

    // Check total hasn't changed
    TDOG_ASSERT_EQ(total, tr.statistic_count(tdog::CNT_TOTAL, "*"));

    // Just one disabled
    tdog::i64_t not_run = tr.statistic_count(tdog::CNT_DISABLED, "*");
    TDOG_ASSERT_EQ(1, not_run);

    not_run += tr.statistic_count(tdog::CNT_SKIPPED, "*");
    TDOG_ASSERT_EQ(2, not_run); // <- now includes skipped

    tdog::i64_t ran = tr.statistic_count(tdog::CNT_RAN, "*");
    TDOG_ASSERT_EQ((total - not_run), ran); // <- not disabled/skipped

    TDOG_ASSERT_EQ(2, tr.statistic_count(tdog::CNT_FAILED, "*")); // <- two failures
    TDOG_ASSERT_EQ(2, tr.statistic_count(tdog::CNT_ERRORS, "*")); // <- two errors
    TDOG_ASSERT_EQ((ran - run_rslt), tr.statistic_count(tdog::CNT_PASSED, "*"));

    // Asserts are being recorded?
    TDOG_ASSERT_GT(tr.statistic_count(tdog::CNT_ASSERT_TOTAL, "*"), 0);
    TDOG_ASSERT_GT(tr.statistic_count(tdog::CNT_ASSERT_FAILED, "*"), 0);

    // Duration
    // Possibly these may fail on some platforms, hence large margin.
    TDOG_ASSERT_GTE(tr.statistic_count(tdog::CNT_DURATION, "*"), 1500);
    TDOG_ASSERT_LT(tr.statistic_count(tdog::CNT_DURATION, "*"), 3000);
    TDOG_ASSERT_GTE(tr.statistic_count(tdog::CNT_DURATION, "status_suite::internal_suite::wait1"), 500);
    TDOG_ASSERT_LT(tr.statistic_count(tdog::CNT_DURATION, "status_suite::internal_suite::wait1"), 1000);
    TDOG_ASSERT_GTE(tr.statistic_count(tdog::CNT_DURATION, "status_suite::internal_suite::wait2"), 1000);
    TDOG_ASSERT_LT(tr.statistic_count(tdog::CNT_DURATION, "status_suite::internal_suite::wait2"), 1500);
  }

  // status_suite
  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
