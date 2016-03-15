//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : naming_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_NAMING_SUITE_H
#define TDOG_NAMING_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Tests concerned with test name and suite name matching.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(naming_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  // A dummy test case that we can instatiate simply.
  class dummy_test :public tdog::basic_test
  {
  protected:

    virtual void _run_unprotected() {} //<- defined but not used

  public:
    dummy_test(const std::string& tname)
      : tdog::basic_test(tname, tdog::TT_TEST, "", 0) {}
  };

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    // Test case and suite of the same name - this is OK
    TDOG_TEST_CASE(same_name)
    {
      TDOG_ASSERT(true);
    }

    TDOG_SUITE(same_name)
    {
      TDOG_TEST_CASE(subtest)
      {
        TDOG_ASSERT(true);
      }

      TDOG_CLOSE_SUITE
    }

    TDOG_TEST_CASE(exists_test)
    {
      // Test we cannot register a test twice
      tdog::runner tr;
      tr.set_default_report(tdog::RS_NONE);

      TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(same_name) ) ); // <- success
      TDOG_ASSERT_NOT( tr.register_test( TDOG_GET_TCPTR(same_name) ) ); // <- fail

      TDOG_ASSERT_EQ(tdog::RAN_NONE, tr.run("*")); // <- none run

      // Grab a copy of the report and look for expected strings
      std::stringstream stm;
      tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
      std::string s = stm.str();

      // Look for declaration error
      TDOG_ASSERT_NEQ(NPOS, s.find("Naming conflict."));
    }

    // internal_suite
    TDOG_CLOSE_SUITE
  }


  TDOG_TEST_CASE(name_matching_impl)
  {
    // Test underlying matching routine.
    TDOG_SET_AUTHOR("Andy Thomas");

    // A test with a name in the default suite.
    // We must use suite_manager to make the test
    // non-registering.
    tdog::suite_manager suite_open(true, "", false);
    dummy_test test_def("test_name");
    tdog::suite_manager suite_close(false);

    // Not only test that suite open/closures are
    // correct, but the following will avoid "unused
    // variable" compiler warnings, and prevent the
    // compiler optimising out the suite names.
    TDOG_ASSERT_EQ(0, suite_open.counter());
    TDOG_ASSERT_EQ(0, suite_close.counter());

    // Positive matches
    TDOG_ASSERT( test_def.name_equals("test_name", false) );
    TDOG_ASSERT( test_def.name_equals("::test_name", false) );
    TDOG_ASSERT( test_def.name_equals("::", false) );
    TDOG_ASSERT( test_def.name_equals("*", false) );
    TDOG_ASSERT( test_def.name_equals("::*", false) );

    // Negative
    TDOG_ASSERT_NOT( test_def.name_equals("wrong_name", false) );
    TDOG_ASSERT_NOT( test_def.name_equals("test*", false) ); // <- incomplete
    TDOG_ASSERT_NOT( test_def.name_equals("test::*", false) ); // <- incomplete
    TDOG_ASSERT_NOT( test_def.name_equals("", false) );
  }

  TDOG_TEST_CASE(suite_matching_impl)
  {
    // Test underlying matching routine.
    TDOG_SET_AUTHOR("Andy Thomas");

    // A test with a name in a named suite
    tdog::suite_manager suite_open(true, "suite1::suite2", false);
    dummy_test test_named("test_name");
    tdog::suite_manager suite_close(false);

    // Positive matches
    TDOG_ASSERT( test_named.name_equals("suite1::suite2::test_name", false) );
    TDOG_ASSERT( test_named.name_equals("::suite1::suite2::test_name", false) );
    TDOG_ASSERT( test_named.name_equals("suite1::suite2::", false) );
    TDOG_ASSERT( test_named.name_equals("::suite1::suite2::", false) );
    TDOG_ASSERT( test_named.name_equals("suite1::*", false) );
    TDOG_ASSERT( test_named.name_equals("*", false) );
    TDOG_ASSERT( test_named.name_equals("::*", false) );

    // Negative
    TDOG_ASSERT_NOT( test_named.name_equals("suite1::suite2::wrong_name", false) );
    TDOG_ASSERT_NOT( test_named.name_equals("suite1::wrong_suite::test_name", false) );
    TDOG_ASSERT_NOT( test_named.name_equals("suite1::", false) ); // <- need "::*"
    TDOG_ASSERT_NOT( test_named.name_equals("suite1::suite::*", false) ); // <- incomplete
    TDOG_ASSERT_NOT( test_named.name_equals("", false) );
    TDOG_ASSERT_NOT( test_named.name_equals("*", true) ); // <- precise is true
    TDOG_ASSERT_NOT( test_named.name_equals("::*", true) ); // <- precise is true
  }

  TDOG_TEST_CASE(postive_tests)
  {
    // Run the internal tests
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/naming_postive.txt");

    // Register tests with local runner
    tr.register_test( TDOG_GET_TCPTR(internal_suite::same_name) );
    tr.register_test( TDOG_GET_TCPTR(internal_suite::same_name::subtest) );
    tr.register_test( TDOG_GET_TCPTR(internal_suite::exists_test) );

    // Check tests exist
    TDOG_ASSERT( tr.exists("naming_suite::internal_suite::same_name") );
    TDOG_ASSERT( tr.exists("naming_suite::internal_suite::same_name::subtest") );
    TDOG_ASSERT( tr.exists("naming_suite::*") );

    // Leading prefix
    TDOG_ASSERT( tr.exists("::*") );
    TDOG_ASSERT( tr.exists("::naming_suite::internal_suite::same_name") );
    TDOG_ASSERT( tr.exists("::naming_suite::internal_suite::same_name::subtest") );

    // Check suites exist (note "::" suffix)
    TDOG_ASSERT( tr.exists("naming_suite::internal_suite::") );
    TDOG_ASSERT( tr.exists("naming_suite::internal_suite::same_name::") );

    // Run the tests
    TDOG_ASSERT_EQ(0, tr.run());
  }

  TDOG_TEST_CASE(negative_tests)
  {
    // Run the internal tests
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/naming_negative.txt");

    // Register tests with local runner
    tr.register_test( TDOG_GET_TCPTR(internal_suite::same_name) );
    tr.register_test( TDOG_GET_TCPTR(internal_suite::same_name::subtest) );
    tr.register_test( TDOG_GET_TCPTR(internal_suite::exists_test) );

    // Negative tests
    TDOG_ASSERT_NOT( tr.exists("::") ); // <- no test directly under it
    TDOG_ASSERT_NOT( tr.exists("naming_suite::") ); // <- no test directly under it
    TDOG_ASSERT_NOT( tr.exists("naming_suite::internal_suite") ); // <- no suffix
    TDOG_ASSERT_NOT( tr.exists("naming_suite::internal_suite::same_") );

    // Run the tests
    TDOG_ASSERT_EQ(0, tr.run());
  }

  // naming_suite
  TDOG_CLOSE_SUITE
}
//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
