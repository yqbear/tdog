//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : runner_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_RUNNER_SUITE_H
#define TDOG_RUNNER_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "comp_unit.hpp"

#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Ensure we can specify a multi-name run list.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(runner_suite)
{
  // A dummy test case that we can instatiate simply.
  class dummy_test :public tdog::basic_test
  {
  protected:

    virtual void _run_unprotected() {} //<- defined but not used

  public:
    dummy_test(const std::string& tname)
      : tdog::basic_test(tname, tdog::TT_TEST, "", 0) {}
  };

  // Parameter injection data
  struct fullsort_inj { static const bool fullsort = true; };
  struct partsort_inj { static const bool fullsort = false; };

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    TDOG_TEST_CASE(pass_assert1)
    {
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(pass_assert2)
    {
      TDOG_ASSERT(true);
    }

    TDOG_TEST_CASE(fail_assert)
    {
      TDOG_ASSERT(true);
      TDOG_ASSERT(false);
      TDOG_ASSERT(true);
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

  TDOG_TEST_CASE(stats_all)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/runner_stats_all.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fail_assert) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass2) ) );

    // Now, run all - 5 out of 5
    int rslt = tr.run("*");
    TDOG_ASSERT_EQ(1, rslt);

    // Now a total of 5, with one fail
    TDOG_ASSERT_EQ(5, tr.statistic_count(tdog::CNT_TOTAL, "*"));
    TDOG_ASSERT_EQ(5, tr.statistic_count(tdog::CNT_RAN, "*"));
    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_FAILED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_SKIPPED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_DISABLED, "*"));
  }

  TDOG_TEST_CASE(stats_some)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/runner_stats_some.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fail_assert) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass2) ) );

    // RUN SOME
    // Expect 1 fail
    // Note one comma, then a space separator.
    // Both should be OK
    int rslt = tr.run("runner_suite::internal_suite::pass_assert1, \
runner_suite::internal_suite::fail_assert runner_suite::internal_suite::nested_suite::");
    TDOG_ASSERT_EQ(1, rslt);

    // Run 4 out 5, with one fail
    // Not the ones we don't run are still in CNT_TOTAL
    TDOG_ASSERT_EQ(5, tr.statistic_count(tdog::CNT_TOTAL, "*"));
    TDOG_ASSERT_EQ(4, tr.statistic_count(tdog::CNT_RAN, "*"));
    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_FAILED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_SKIPPED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_DISABLED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_ERRORS, "*"));
  }

  TDOG_TEST_PROTECTED(parsing, tdog::runner)
  {
    // Test internal parsing for run command
    std::string rout, dout;

    const char* argv1[] = {"--trun", "TEST1", "SUITE1::TEST2,,", "TEST3", "--help",
      "--tdis", "TEST4"};
    TDOG_ASSERT( _run_parse<char>(7, argv1, rout, dout) );
    TDOG_ASSERT_EQ("TEST1,SUITE1::TEST2,TEST3", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    const char* argv2[] = {"--help", "--tdis", "TEST4", "--trun", "TEST1",
      "SUITE1::TEST2,,", "TEST3"};
    TDOG_ASSERT( _run_parse<char>(7, argv2, rout, dout) );
    TDOG_ASSERT_EQ("TEST1,SUITE1::TEST2,TEST3", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    const char* argv3[] = {"--trall", "--help", "--tdis", "TEST4"};
    TDOG_ASSERT( _run_parse<char>(4, argv3, rout, dout) );
    TDOG_ASSERT_EQ("*", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    const char* argv4[] = {"/trun", "TEST1", "SUITE1::TEST2,,", "TEST3", "/tdis", "TEST4"};
    TDOG_ASSERT( _run_parse<char>(6, argv4, rout, dout) );
    TDOG_ASSERT_EQ("TEST1,SUITE1::TEST2,TEST3", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    const char* argv5[] = {"/trun:", "TEST1", "SUITE1::TEST2,,", "TEST3", "/tdis:", "TEST4"};
    TDOG_ASSERT( _run_parse<char>(6, argv5, rout, dout) );
    TDOG_ASSERT_EQ("TEST1,SUITE1::TEST2,TEST3", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    // Wide
    const wchar_t* argv6[] = {L"someapp.exe", L"/trun:TEST1", L"SUITE1::TEST2,,", L"TEST3",
      L"/tdis:TEST4"};
    TDOG_ASSERT( _run_parse<wchar_t>(5, argv6, rout, dout) );
    TDOG_ASSERT_EQ("TEST1,SUITE1::TEST2,TEST3", rout);
    TDOG_ASSERT_EQ("TEST4", dout);

    // Negative
    const char* argv7[] = {"--help", "--tdis", "TEST4"};
    TDOG_ASSERT_NOT( _run_parse<char>(3, argv7, rout, dout) );
    TDOG_ASSERT_EQ("", rout);
  }

  TDOG_TEST_CASE(cmdline)
  {
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/runner_cmdline.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::pass_assert2) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::fail_assert) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass1) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::pass2) ) );

    // Run some, disable the failure
    const char* argv[] = {"--trun", "runner_suite::internal_suite::*", "--tdis",
      "runner_suite::internal_suite::fail_assert"};

    // Run command has same const interface
    // as main() -- must cast to remove const.
    TDOG_ASSERT_EQ(0, tr.run_cmdline(4, const_cast<char**>(argv)) );

    // Run 4 out 5, with one disabled
    TDOG_ASSERT_EQ(5, tr.statistic_count(tdog::CNT_TOTAL, "*"));
    TDOG_ASSERT_EQ(4, tr.statistic_count(tdog::CNT_RAN, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_FAILED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_SKIPPED, "*"));
    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_DISABLED, "*"));
    TDOG_ASSERT_EQ(0, tr.statistic_count(tdog::CNT_ERRORS, "*"));
  }

  TDOG_TEST_CASE(singleton_method)
  {
    // Test our singleton method gives the same
    // instance accross different compilation units.
    std::string name = tdog::runner::global().project_name();

    TDOG_ASSERT_NOT_MSG(name.empty(), "The runner project name must not be empty");

    if (!name.empty())
    {
      TDOG_ASSERT_EQ(name, tdog_test::global_project_name());
      TDOG_PRINTF("Project name: %s", name);
    }
  }

  TDOG_TEST_CASE(immutable_test)
  {
    // Ensure code withing test cases can't change global runner
    // properties using a macro. This is a safety feature.
    TDOG_ASSERT_THROW(TDOG_SET_PROJECT_NAME("New name"), std::logic_error);

    TDOG_ASSERT_NEQ("New name", tdog::runner::global().project_name());
  }

  TDOG_DEFINE_REPEATED(sort_type)
  {
    // Test declared here in correct sort order.
    // A test with a name in the default suite.
    // We must use suite_manager to make the test
    // non-registering, and control the suite name.
    tdog::suite_manager suite_open(true, "", false);
    dummy_test su1("setup01");
    dummy_test su2("SETUP02");
    dummy_test tc1("tc1");
    dummy_test tc2("TC2");
    dummy_test tc2_("TC2_");
    dummy_test td1("_teardown01");
    dummy_test td2("TEARDOWN02");

    tdog::suite_manager suiteA_open(true, "suiteA", false);
    dummy_test su1a("_setup01");
    dummy_test su2a("SetUp02");
    dummy_test tc1a("tc1");
    dummy_test tc2a("TC2");
    dummy_test tc2a_("TC2_");
    dummy_test td1a("_teardown01");
    dummy_test td2a("TEARDOWN02");

    tdog::suite_manager suiteAB_open(true, "suiteB", false);
    dummy_test su1ab("_setup01");
    dummy_test su2ab("SetUp02");
    dummy_test tc1ab("tc1");
    dummy_test tc2ab("TC2");
    dummy_test tc2ab_("TC2_");
    dummy_test td1ab("_teardown01");
    dummy_test td2ab("TEARDOWN02");

    tdog::suite_manager suiteAB_close(false);
    tdog::suite_manager suiteA_close(false);

    tdog::suite_manager suiteC_open(true, "suiteC", false);
    dummy_test su1c("_setup01");
    dummy_test su2c("SetUp02");
    dummy_test tc1c("tc1");
    dummy_test tc2c("TC2");
    dummy_test tc2c_("TC2_");
    dummy_test td1c("_teardown01");
    dummy_test td2c("TEARDOWN02");

    tdog::suite_manager suiteC_close(false);
    tdog::suite_manager suite_close(false);

    // Not only test that suite open/closures are
    // correct, but the following will avoid "unused
    // variable" compiler warnings, and prevent the
    // compiler optimising out the suite names.
    TDOG_ASSERT_EQ(0, suite_open.counter());
    TDOG_ASSERT_EQ(0, suite_close.counter());
    TDOG_ASSERT_EQ(0, suiteA_open.counter());
    TDOG_ASSERT_EQ(0, suiteA_close.counter());
    TDOG_ASSERT_EQ(0, suiteAB_open.counter());
    TDOG_ASSERT_EQ(0, suiteAB_close.counter());
    TDOG_ASSERT_EQ(0, suiteC_open.counter());
    TDOG_ASSERT_EQ(0, suiteC_close.counter());

    // Register out of order order
    tdog::runner tr;
    tr.register_test(&td2c);
    tr.register_test(&td1c);
    tr.register_test(&td2ab);
    tr.register_test(&td1ab);
    tr.register_test(&td2a);
    tr.register_test(&td1a);
    tr.register_test(&td2);
    tr.register_test(&td1);

    tr.register_test(&su2c);
    tr.register_test(&su1c);
    tr.register_test(&su2ab);
    tr.register_test(&su1ab);
    tr.register_test(&su2a);
    tr.register_test(&su1a);
    tr.register_test(&su2);
    tr.register_test(&su1);

    tr.register_test(&tc2c_);
    tr.register_test(&tc2c);
    tr.register_test(&tc1c);
    tr.register_test(&tc2ab_);
    tr.register_test(&tc2ab);
    tr.register_test(&tc1ab);
    tr.register_test(&tc2a_);
    tr.register_test(&tc2a);
    tr.register_test(&tc1a);
    tr.register_test(&tc2_);
    tr.register_test(&tc2);
    tr.register_test(&tc1);

    // Set sort order
    TDOG_PRINTF("FULL SORT: %b", USER_TYPE::fullsort);
    tr.set_sorted(USER_TYPE::fullsort);
    TDOG_ASSERT_EQ(USER_TYPE::fullsort , tr.sorted());

    // Enumerate suite names
    std::vector<std::string> names;
    tr.enumerate_suite_names(names);

    TDOG_PRINT("\nSuite names...");
    for(std::size_t n = 0; n < names.size(); ++n)
    {
      TDOG_PRINT(names[n]);
    }
    TDOG_PRINT("");

    TDOG_ASSERT_EQ(4 , (int)names.size());
    TDOG_ASSERT_EQ("", names[0]);
    TDOG_ASSERT_EQ("suiteA", names[1]);
    TDOG_ASSERT_EQ("suiteA::suiteB", names[2]);
    TDOG_ASSERT_EQ("suiteC", names[3]);

    // Enumerate tests names
    tr.enumerate_test_names(names);

    TDOG_PRINT("\nTest names...");
    for(std::size_t n = 0; n < names.size(); ++n)
    {
      TDOG_PRINT(names[n]);
    }
    TDOG_PRINT("");

    TDOG_ASSERT_EQ(28 , (int)names.size());

    if (USER_TYPE::fullsort)
    {
      // Check full sort
      TDOG_ASSERT_EQ("setup01", names[0]);
      TDOG_ASSERT_EQ("SETUP02", names[1]);
      TDOG_ASSERT_EQ("tc1", names[2]);
      TDOG_ASSERT_EQ("TC2", names[3]);
      TDOG_ASSERT_EQ("TC2_", names[4]);

      TDOG_ASSERT_EQ("suiteA::_setup01", names[5]);
      TDOG_ASSERT_EQ("suiteA::SetUp02", names[6]);
      TDOG_ASSERT_EQ("suiteA::tc1", names[7]);
      TDOG_ASSERT_EQ("suiteA::TC2", names[8]);
      TDOG_ASSERT_EQ("suiteA::TC2_", names[9]);

      TDOG_ASSERT_EQ("suiteA::suiteB::_setup01", names[10]);
      TDOG_ASSERT_EQ("suiteA::suiteB::SetUp02", names[11]);
      TDOG_ASSERT_EQ("suiteA::suiteB::tc1", names[12]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TC2", names[13]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TC2_", names[14]);
      TDOG_ASSERT_EQ("suiteA::suiteB::_teardown01", names[15]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TEARDOWN02", names[16]);

      TDOG_ASSERT_EQ("suiteA::_teardown01", names[17]);
      TDOG_ASSERT_EQ("suiteA::TEARDOWN02", names[18]);

      TDOG_ASSERT_EQ("suiteC::_setup01", names[19]);
      TDOG_ASSERT_EQ("suiteC::SetUp02", names[20]);
      TDOG_ASSERT_EQ("suiteC::tc1", names[21]);
      TDOG_ASSERT_EQ("suiteC::TC2", names[22]);
      TDOG_ASSERT_EQ("suiteC::TC2_", names[23]);
      TDOG_ASSERT_EQ("suiteC::_teardown01", names[24]);
      TDOG_ASSERT_EQ("suiteC::TEARDOWN02", names[25]);

      // Global last
      TDOG_ASSERT_EQ("_teardown01", names[26]);
      TDOG_ASSERT_EQ("TEARDOWN02", names[27]);
    }
    else
    {
      // Check partial sorted
      // Suites and setup/teardown sorted,
      // but test names in original order.
      TDOG_ASSERT_EQ("setup01", names[0]);
      TDOG_ASSERT_EQ("SETUP02", names[1]);
      TDOG_ASSERT_EQ("TC2_", names[2]);
      TDOG_ASSERT_EQ("TC2", names[3]);
      TDOG_ASSERT_EQ("tc1", names[4]);

      TDOG_ASSERT_EQ("suiteA::_setup01", names[5]);
      TDOG_ASSERT_EQ("suiteA::SetUp02", names[6]);
      TDOG_ASSERT_EQ("suiteA::TC2_", names[7]);
      TDOG_ASSERT_EQ("suiteA::TC2", names[8]);
      TDOG_ASSERT_EQ("suiteA::tc1", names[9]);

      TDOG_ASSERT_EQ("suiteA::suiteB::_setup01", names[10]);
      TDOG_ASSERT_EQ("suiteA::suiteB::SetUp02", names[11]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TC2_", names[12]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TC2", names[13]);
      TDOG_ASSERT_EQ("suiteA::suiteB::tc1", names[14]);
      TDOG_ASSERT_EQ("suiteA::suiteB::_teardown01", names[15]);
      TDOG_ASSERT_EQ("suiteA::suiteB::TEARDOWN02", names[16]);

      TDOG_ASSERT_EQ("suiteA::_teardown01", names[17]);
      TDOG_ASSERT_EQ("suiteA::TEARDOWN02", names[18]);

      TDOG_ASSERT_EQ("suiteC::_setup01", names[19]);
      TDOG_ASSERT_EQ("suiteC::SetUp02", names[20]);
      TDOG_ASSERT_EQ("suiteC::TC2_", names[21]);
      TDOG_ASSERT_EQ("suiteC::TC2", names[22]);
      TDOG_ASSERT_EQ("suiteC::tc1", names[23]);
      TDOG_ASSERT_EQ("suiteC::_teardown01", names[24]);
      TDOG_ASSERT_EQ("suiteC::TEARDOWN02", names[25]);

      // Global last
      TDOG_ASSERT_EQ("_teardown01", names[26]);
      TDOG_ASSERT_EQ("TEARDOWN02", names[27]);
    }
  }

  TDOG_TEST_REPEATED(full_sorted, sort_type, fullsort_inj);
  TDOG_TEST_REPEATED(part_sorted, sort_type, partsort_inj);

  // runner_suite
  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
