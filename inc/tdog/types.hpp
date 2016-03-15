//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : types.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TYPES_H
#define TDOG_TYPES_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "compat.hpp"

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------
/**
 * \file
 * \brief This file provides data types associated with testing.
 */

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
namespace tdog {

/**
* \brief Return constant indicating that no test were run.
* \sa TDOG_RUN(), tdog::runner::run()
*/
const int RAN_NONE = -1;

/**
 * \brief An enumerated type used to designate the style of test run reports.
 * \details Non-verbose reports contain only summary information. Verbose
 * reports, on the other hand, contain maximum trace information. For verbose XML,
 * the report will contain test case output information within the "<system-out>"
 * element. HTML reports are suitable for viewing in a browser.
 * \sa runner::set_default_report(), runner::generate_report(), runner::add_report()
 */
enum report_style_t
{
  RS_NONE,            //!< No report is generated
  RS_TEXT_SUMMARY,    //!< Human readable test summary
  RS_TEXT_VERBOSE,    //!< Human readable verbose test report
  RS_HTML_SUMMARY,    //!< HTML summary report
  RS_HTML_VERBOSE,    //!< HTML verbose report
  RS_XML,             //!< JUnit compatible XML report
};

/**
 * \brief An enumerated type used to designate a required statistic from
 * the test run.
 * \sa runner::statistic_count()
 */
enum stat_count_t
{
  CNT_TOTAL,           //!< Total number of test cases
  CNT_RAN,             //!< Number of test cases executed
  CNT_PASSED,          //!< Number of tests which passed (inc. warnings)
  CNT_FAILED,          //!< Number of test cases that failed
  CNT_ERRORS,          //!< Number of test cases with implementation errors
  CNT_SKIPPED,         //!< Number of test cases skipped (exc. disabled)
  CNT_DISABLED,        //!< Number of disabled test cases (exc. skipped)
  CNT_WARNINGS,        //!< Number of tests with warnings
  CNT_PASS_RATE,       //!< Percentage of tests that passed
  CNT_ASSERT_TOTAL,    //!< Total number of assert statements
  CNT_ASSERT_FAILED,   //!< Number of assert failures
  CNT_DURATION         //!< Duration of in milliseconds
};

/**
 * \brief An enumerated type used to designate the a test case status.
 * \sa runner::test_status()
 */
enum status_t
{
  TS_NOT_EXIST,     //!< Test case does not exist
  TS_READY,         //!< Test is ready to run
  TS_PASS_OK,       //!< Test passed OK
  TS_PASS_WARN,     //!< Test passed with warning
  TS_FAILED,        //!< Test failed
  TS_ERROR,         //!< Test failed because of a possible test case implementation error
  TS_SKIPPED,       //!< Test is enabled, but was skipped because by the author
  TS_DISABLED,      //!< Test is disabled
  TS_DECL_ERROR     //!< Test run aborted because of test declaration errors
};

/**
 * \brief An enumerated type identifying the test type.
 * \sa tdog::runner::test_type()
 */
enum test_type_t
{
  TT_TEST,      //!< Vanilla test case
  TT_REPEATED,  //!< Reteated test
  TT_FIXTURE,   //!< Fixture, a test with setup and teardown routines
  TT_PROTECTED  //!< Used to test protected methods of a class
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
