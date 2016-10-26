//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : main.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------

// Used for testing. Uncommenting
// will break the dicontig_suite.
//#define TDOG_COUNTER_ID

#include <tdog.hpp>

#include "size_suite.hpp"
#include "assert_suite.hpp"
#include "discontig_suite.hpp"
#include "error_suite.hpp"
#include "message_suite.hpp"
#include "naming_suite.hpp"
#include "timing_suite.hpp"
#include "status_suite.hpp"
#include "runner_suite.hpp"
#include "repeated_suite.hpp"
#include "fixture_suite.hpp"
#include "example_suite.hpp"

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// This application will run a number of test suites in order to self-test
// the TDOG framework. Reports will be generated to "./reports", and
// additionally, "./reports/internal". The "internal" reports will show
// failures. This is expected, as many of these are negative tests, i.e.
// they ensure the framework generates failure results. The information
// written to STDOUT should show success, however.
//
// See the documentation under the "doc" directory for more information.

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
bool g_setup_called = false;
bool g_test_called = false;
bool g_teardown_called = false;

// Test global setup/teardown
TDOG_TEST_CASE(setup_global_01)
{
  // This will be executed first, as the runner
  // will explicity sort these.
  g_setup_called = true;
  TDOG_ASSERT_NOT(g_test_called);
}

TDOG_TEST_CASE(setup_global_02)
{
  // This will be executed second
  TDOG_ASSERT(g_setup_called);
  TDOG_ASSERT_NOT(g_test_called);
}

TDOG_TEST_CASE(default_namespace_test)
{
  // This will be executed between setup() and teardown()
  g_test_called = true;
}

TDOG_TEST_CASE(teardown_global_01)
{
  // This will be executed second to last
  TDOG_ASSERT(g_setup_called);
  TDOG_ASSERT(g_test_called);
  g_teardown_called = true;
}

TDOG_TEST_CASE(teardown_global_02)
{
  // This will be executed last
  TDOG_ASSERT(g_teardown_called);
  TDOG_ASSERT(g_test_called);
}

//---------------------------------------------------------------------------
// MAIN FUNCTION
//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
  // Set project title
  TDOG_SET_PROJECT_NAME(TDOG_LIB_NAME);
  TDOG_SET_PROJECT_VERSION(TDOG_LIB_VERSION);
  TDOG_SET_PROJECT_DESC(
    "Reports in TEXT, HTML and XML format will be generated\n"
    "under './reports'. This location should pre-exist in the\n"
    "working directory.");

  // Define the report style
  TDOG_SET_DEFAULT_REPORT(tdog::RS_TEXT_SUMMARY);

  // Additional reports
  TDOG_ADD_REPORT(tdog::RS_TEXT_VERBOSE, "./reports/report.txt");
  TDOG_ADD_REPORT(tdog::RS_HTML_SUMMARY, "./reports/summary.html");
  TDOG_ADD_REPORT(tdog::RS_HTML_VERBOSE, "./reports/report.html");
  TDOG_ADD_REPORT(tdog::RS_XML, "./reports/report.xml");


  // See example_suite
  project_namespace::example_suite::beast_suite::INIT_BEAST = 666;

  // Run the tests
  // Returns zero on success
  return TDOG_RUN_CMD(argc, argv, true);
}
//---------------------------------------------------------------------------
