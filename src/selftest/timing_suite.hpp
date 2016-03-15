//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : timing_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TIMING_SUITE_H
#define TDOG_TIMING_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

#include <ctime>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test test case time limits.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(timing_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    void wait_delay(int min_sec)
    {
      // Crude platform independent "sleep" function which
      // waits for a *minimum* number of seconds. Unfortunately,
      // it will hammer the CPU a tad (sorry about that), but
      // we want to test without relying on our own sleep call.
      if (min_sec >= 0)
      {
        std::time_t until = std::time(0) + min_sec;
        while(time(0) <= until);
      }
    }

    TDOG_TEST_CASE(local_timeout)
    {
      // Test expected to fail.
      // Make sure the time limit is working
      // before we use "real" sleep
      TDOG_SET_TEST_TIMEOUT(1200);

      // Wait for (more than) 1 second
      // This avoids our own sleep.
      wait_delay(2);
    }

    TDOG_TEST_CASE(short_sleep)
    {
      // Test expected to fail
      // This tests our own sleep call
      TDOG_SET_TEST_TIMEOUT(800);

      // Wait for (more than) 1 second.
      TDOG_TEST_SLEEP(1500);
    }

    TDOG_TEST_CASE(long_sleep)
    {
      // Test expected to fail
      // This tests our own sleep call
      TDOG_SET_TEST_TIMEOUT(2000);

      // Wait for (more than) 1 second.
      TDOG_TEST_SLEEP(2500);
    }

    TDOG_TEST_CASE(global_timeout)
    {
      // Wait for (more than) 1 second
      TDOG_TEST_SLEEP(1500);
    }

    TDOG_TEST_CASE(global_exempt)
    {
      // Expect to pass
      TDOG_SET_GLOBAL_LIMIT_EXEMPT();
      TDOG_TEST_SLEEP(1500);
    }

    TDOG_TEST_CASE(check_limit)
    {
      // Wait while checking limit
      int count = 0;
      while(count++ < 20)
      {
        TDOG_TEST_SLEEP(100);

        if (TDOG_HAS_TEST_TIMED_OUT())
        {
          TDOG_TEST_WARNING("Exceed limit detected");
          return;
        }
      }

      TDOG_TEST_ERROR("Never detected timout");
    }

    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(local_timeout)
  {
    // Test sleep call with value less than 1 sec
    // This does not use our own sleep call.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_local_timeout.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::local_timeout) ) );

    TDOG_ASSERT_EQ(1, tr.run()); // <- expect fails
  }

  TDOG_TEST_CASE(short_sleep)
  {
    // Test sleep call with value less than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_short_sleep.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::short_sleep) ) );

    TDOG_ASSERT_EQ(1, tr.run()); // <- expect fails
  }

  TDOG_TEST_CASE(long_sleep)
  {
    // Test sleep call with value more than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_long_sleep.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::long_sleep) ) );

    TDOG_ASSERT_EQ(1, tr.run()); // <- expect fails
  }

  TDOG_TEST_CASE(global_timeout)
  {
    // Test sleep call with value more than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_global_timeout.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::global_timeout) ) );

    tr.set_global_time_limit(1000);

    TDOG_ASSERT_EQ(1, tr.run()); // <- expect fails
  }


  TDOG_TEST_CASE(global_exempt)
  {
    // Test sleep call with value more than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_global_exempt.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::global_exempt) ) );

    tr.set_global_time_limit(1000);

    TDOG_ASSERT_EQ(0, tr.run()); // <- expect success
  }

  TDOG_TEST_CASE(time_warning)
  {
    // Test sleep call with value more than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_time_warning.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::global_exempt) ) );

    tr.set_global_time_limit(1000);
    tr.set_global_time_warning(1000);

    TDOG_ASSERT_EQ(0, tr.run()); // <- expect success

    // Look for expected string
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string s = stm.str();

    TDOG_ASSERT_NEQ(NPOS, s.find("time warning threshold of 1000 ms exceeded"));
  }

  TDOG_TEST_CASE(check_limit)
  {
    // Test sleep call with value more than 1 sec
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/timing_check_limit.txt");

    // Register tests with local runner
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::check_limit) ) );

    tr.set_global_time_limit(1000);

    TDOG_ASSERT_EQ(1, tr.run()); // <- expect fail

    // Look for expected string
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string s = stm.str();

    TDOG_ASSERT_NEQ(NPOS, s.find("Exceed limit detected"));
  }

  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
