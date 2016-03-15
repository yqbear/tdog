//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : fixture_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_FIXTURE_H
#define TDOG_FIXTURE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

#include <stdexcept>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test fixture and suite setup/tear down.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(fixture_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    std::string log;

    class fix_class_ok
    {
    protected:
    public:
      fix_class_ok() { log += "fix_c "; }
      ~fix_class_ok() { log += "fix_d "; }
      bool setup() { log += "fix_setup "; return true; }
      void teardown() { log += "fix_teardown "; }
    };

    class fix_class_fail
    {
    protected:
    public:
      fix_class_fail() { log += "fix_c "; }
      ~fix_class_fail() { log += "fix_d "; }
      bool setup() { log += "fix_setup "; return false; }
      void teardown() { log += "fix_teardown "; }
    };

    class fix_class_teardown_throw
    {
    protected:
    public:
      fix_class_teardown_throw() { log += "fix_c "; }
      ~fix_class_teardown_throw() { log += "fix_d "; }
      bool setup() { log += "fix_setup "; return true; }
      void teardown() { throw std::runtime_error("Some error"); }
    };

    TDOG_TEST_CASE(SETUP1)
    {
      log += "setup1 ";
    }

    TDOG_TEST_CASE(SETUP2)
    {
      log += "setup2 ";
    }

    TDOG_TEST_CASE(SETUP_FAIL)
    {
      log += "setup1 ";
      TDOG_ASSERT(false);
    }

    TDOG_TEST_CASE(SETUP_ERROR)
    {
      log += "setup1 ";
      throw std::runtime_error("Some error");
    }

    TDOG_TEST_FIXTURE(fixture_ok, fix_class_ok)
    {
    }

    TDOG_TEST_FIXTURE(fixture_fail, fix_class_fail)
    {
    }

    TDOG_TEST_FIXTURE(fixture_teardown_throw, fix_class_teardown_throw)
    {
    }

    TDOG_TEST_FIXTURE(fixture_test_throw, fix_class_ok)
    {
      throw std::runtime_error("Some error");
    }

    TDOG_TEST_CASE(_teardown1)
    {
      log += "teardown1 ";
    }

    TDOG_TEST_CASE(_teardown2)
    {
      log += "teardown2 ";
    }

    // internal_suite
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(setup_positive)
  {
    // Test suit setups/teardowns are called
    // in order, along with fixture's.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_setup_positive.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_ok) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::SETUP2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::SETUP1) ) );

    // Clear log
    internal_suite::log.clear();

    // Run, no failures
    TDOG_ASSERT_EQ(0, tr.run("*"));

    TDOG_ASSERT_EQ(
      "setup1 setup2 fix_c fix_setup fix_teardown fix_d teardown1 teardown2 ",
      internal_suite::log);

    // Look in report
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("OK: Fixture teardown()"));
  }

  TDOG_TEST_CASE(skip_on_fail)
  {
    // If suite setup fails, other tests skipped.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_skip_on_fail.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_ok) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::SETUP_FAIL) ) );

    // Clear log
    internal_suite::log.clear();

    // Run - setup in error
    TDOG_ASSERT_EQ(1, tr.run("*"));

    // Only set has run
    TDOG_ASSERT_EQ("setup1 ", internal_suite::log);

    // Check skip status
    TDOG_ASSERT_EQ(tdog::TS_FAILED, tr.test_status("fixture_suite::internal_suite::SETUP_FAIL"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::fixture_ok"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::_teardown1"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::_teardown2"));
  }

  TDOG_TEST_CASE(skip_on_error)
  {
    // If suite setup fails, other tests skipped.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_skip_on_error.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_ok) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::_teardown1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::SETUP_ERROR) ) );

    // Clear log
    internal_suite::log.clear();

    // Run - setup in error
    TDOG_ASSERT_EQ(1, tr.run("*"));

    // Only set has run
    TDOG_ASSERT_EQ("setup1 ", internal_suite::log);

    // Check skip status
    TDOG_ASSERT_EQ(tdog::TS_ERROR, tr.test_status("fixture_suite::internal_suite::SETUP_ERROR"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::fixture_ok"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::_teardown1"));
    TDOG_ASSERT_EQ(tdog::TS_SKIPPED, tr.test_status("fixture_suite::internal_suite::_teardown2"));
  }

  TDOG_TEST_CASE(class_setup_fail)
  {
    // If fixture type fail, test not executed and teardown is not called.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_class_setup_fail.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_fail) ) );

    // Clear log
    internal_suite::log.clear();

    // Run - setup in error
    TDOG_ASSERT_EQ(1, tr.run("*"));

    // Only set has run
    TDOG_ASSERT_EQ("fix_c fix_setup fix_d ", internal_suite::log);

    // Check skip status
    TDOG_ASSERT_EQ(tdog::TS_ERROR, tr.test_status("fixture_suite::internal_suite::fixture_fail"));
  }

  TDOG_TEST_CASE(class_test_throw)
  {
    // If test with fixture throws, teardown() still called.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_class_test_throw.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_test_throw) ) );

    // Clear log
    internal_suite::log.clear();

    // Run - setup in error
    TDOG_ASSERT_EQ(1, tr.run("*"));

    // Only set has run
    TDOG_ASSERT_EQ("fix_c fix_setup fix_teardown fix_d ", internal_suite::log);

    // Check skip status
    TDOG_ASSERT_EQ(tdog::TS_ERROR, tr.test_status("fixture_suite::internal_suite::fixture_test_throw"));
  }

  TDOG_TEST_CASE(class_teardown_throw)
  {
    // If test with fixture throws in teardown(), this is logged.
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/fixture_class_teardown_throw.txt");

    // Note we register out of order, but they will be correctly
    // sorted so that setup runs first, and teardown last in numerical order.
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fixture_teardown_throw) ) );

    // Clear log
    internal_suite::log.clear();

    // Run - setup in error
    TDOG_ASSERT_EQ(1, tr.run("*"));

    // Only set has run
    TDOG_ASSERT_EQ("fix_c fix_setup fix_d ", internal_suite::log);

    // Check skip status
    TDOG_ASSERT_EQ(tdog::TS_ERROR, tr.test_status("fixture_suite::internal_suite::fixture_teardown_throw"));

    // Look in report
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("FAIL: Fixture teardown()"));
  }

  // fixture_suite
  TDOG_CLOSE_SUITE
}
//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
