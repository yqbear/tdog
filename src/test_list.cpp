//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : test_list.cpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "test_list.hpp"
#include "tdog/private/basic_test.hpp"
#include "tdog/private/suite_manager.hpp"
#include "util.hpp"

#include <algorithm>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

//---------------------------------------------------------------------------
// CLASS test_list : PRIVATED MEMBERS
//---------------------------------------------------------------------------
bool test_list::sort_functor::operator()(const basic_test* a, const basic_test* b) const
{
  // Comparison function object (i.e. an object that satisfies the
  // requirements of Compare) which returns ​true if the first argument
  // is less than (i.e. is ordered before) the second. Teardown() must
  // always come at end, even if suite has nested suites.

  // Sorted example:

  // ::setup
  // ::test
  // ::suiteA::setup
  // ::suiteA::testA
  // ::suiteA::zuiteB::setup
  // ::suiteA::zuiteB::test
  // ::suiteA::zuiteB::teardown
  // ::suiteA::teardown
  // ::teardown

  bool setup_a = a->is_setup();
  bool setup_b = b->is_setup();
  bool teardown_a = a->is_teardown();
  bool teardown_b = b->is_teardown();
  std::string sname_a = a->suite_name();
  std::string sname_b = b->suite_name();
  std::string tname_a = a->test_name();
  std::string tname_b = b->test_name();
  std::string tname_lwr_a = ascii_lower(tname_a);
  std::string tname_lwr_b = ascii_lower(tname_b);

  // Sort teardown
  if (teardown_a || teardown_b)
  {
    // Reverse suite sort, return false...
    // SuiteB::SuiteA::test
    // SuiteB::teardown()
    if (sname_a == sname_b)
    {
      if (!teardown_a || !teardown_b)
      {
        // Setup comes first
        if (setup_a || teardown_b) return true;
        if (setup_b || teardown_a) return false;
      }

      // Sort on the test name in same suite
      if (tname_lwr_a != tname_lwr_b) return (tname_lwr_a < tname_lwr_b);
      else return (tname_a < tname_b);
    }

    if (teardown_a && sname_b.substr(0, sname_a.size()) == sname_a) return false;
    if (teardown_b && sname_a.substr(0, sname_b.size()) == sname_b) return true;
  }

  // Sort suite different names
  std::string sname_lwr_a = ascii_lower(sname_a);
  std::string sname_lwr_b = ascii_lower(sname_b);

  if (sname_lwr_a != sname_lwr_b) return (sname_lwr_a < sname_lwr_b);
  if (sname_a != sname_b) return (sname_a < sname_b);

    // Sort pairs on order
  if ((setup_a && setup_b) || (teardown_a && teardown_b))
  {
    if (tname_lwr_a != tname_lwr_b) return (tname_lwr_a < tname_lwr_b);
    else return (tname_a < tname_b);
  }

  // Setup comes first
  if (setup_a || teardown_b) return true;
  if (setup_b || teardown_a) return false;

  if (full_sort)
  {
    // Sort test names
    if (tname_lwr_a != tname_lwr_b) return (tname_lwr_a < tname_lwr_b);
    else return (tname_a < tname_b);
  }

  return false;
}
//---------------------------------------------------------------------------
// CLASS test_list : PUBLIC MEMBERS
//---------------------------------------------------------------------------
void test_list::sort(bool full)
{
  // Sorts the list. The default suite will always be sorted first.
  // Additionally, within a suite, a test beginning "setup" or "_setup"
  // will always be sorted first, and tests beginning "_teardown" or
  // "_TEARDOWN" will be sorted last. Otherwise, if full is false,
  // the suite and test names will remain unsorted. If full is true,
  // the list will be sorted according to suite and test names is
  // ordinal order.
  sort_functor sort_fn;
  sort_fn.full_sort = full;
  std::stable_sort(storage.begin(), storage.end(), sort_fn);
}
//---------------------------------------------------------------------------
basic_test* test_list::find(const std::string& full_name, bool precise) const
{
  // Find test case and return pointer. If precise is true,
  // then name must match the full test name.
  std::string tn = trim_str(full_name);

  for(std::size_t n = 0; n < storage.size(); ++n)
  {
    if (storage[n]->name_equals(tn, precise))
    {
      return storage[n];
    }
  }

  return 0;
}
//---------------------------------------------------------------------------
