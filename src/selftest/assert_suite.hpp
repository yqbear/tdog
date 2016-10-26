//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : assert_suite.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_ASSSERT_SUITE_H
#define TDOG_ASSSERT_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

#include <sstream>
#include <stdexcept>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test all assert conditions, including that asserts fail
// given a negative condition. We will also test that custom
// messages appear in reports

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(assert_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  // Test functions
  int tsqr(int x) { return x * x; }

  void throw_exception(bool flag)
  {
    if (flag) throw std::runtime_error("test error");
  }

  // ------------------------------
  // POSITIVE INTERNAL
  // ------------------------------
  TDOG_EXPLICIT_SUITE(positive_suite)
  {
    TDOG_TEST_CASE(assert)
    {
      TDOG_ASSERT(true);
      TDOG_ASSERT_MSG(true, "assert_msg()");
    }

    TDOG_TEST_CASE(assert_not)
    {
      TDOG_ASSERT(true);
      TDOG_ASSERT_MSG(true, "assert_msg()");
      TDOG_ASSERT_NOT(false);
      TDOG_ASSERT_NOT_MSG(false, "assert_not_msg()");
    }

    TDOG_TEST_CASE(assert_eq)
    {
      // Boolean
      TDOG_ASSERT_EQ(true, (5 == 5));

      // Integer type
      TDOG_ASSERT_EQ(25, tsqr(5));
      TDOG_ASSERT_EQ_MSG(25, tsqr(5), "assert_eq_msg(int)");

      // Unsigned
      unsigned int ua = 5;
      unsigned int ub = 5;
      TDOG_ASSERT_EQ(ua, ub);
      TDOG_ASSERT_EQ_MSG(ua, ub, "assert_eq_msg(unsigned)");

      // String
      TDOG_ASSERT_EQ("hello", std::string("hello"));
      TDOG_ASSERT_EQ(std::string("hello"), "hello");
      TDOG_ASSERT_EQ_MSG("hello", std::string("hello"), "assert_eq_msg(string)");

      // Wide string
      TDOG_ASSERT_EQ(L"hello", std::wstring(L"hello"));
      TDOG_ASSERT_EQ(std::wstring(L"hello"), L"hello");
      TDOG_ASSERT_EQ_MSG(L"hello", std::wstring(L"hello"), "assert_eq_msg(wstring)");

      // Vector type
      std::vector<int> va(3, 8);
      std::vector<int> vb(3, 8);
      TDOG_ASSERT_EQ(va, vb);
      TDOG_ASSERT_EQ_MSG(va, vb, "assert_eq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_neq)
    {
      // Boolean
      TDOG_ASSERT_NEQ(true, (5 == 6));

      // Integer type
      TDOG_ASSERT_NEQ(25, tsqr(6));
      TDOG_ASSERT_NEQ_MSG(25, tsqr(6), "assert_neq_msg(int)");

      // Unsigned
      unsigned int ua = 5;
      unsigned int uc = 6;
      TDOG_ASSERT_NEQ(ua, uc);
      TDOG_ASSERT_NEQ_MSG(ua, uc, "assert_neq_msg(unsigned)");

      // String
      TDOG_ASSERT_NEQ("hello", std::string("HELLO"));
      TDOG_ASSERT_NEQ(std::string("HELLO"), "hello");
      TDOG_ASSERT_NEQ_MSG("hello", std::string("HELLO"), "assert_neq_msg(string)");

      // Wide string
      TDOG_ASSERT_NEQ(L"hello", std::wstring(L"HELLO"));
      TDOG_ASSERT_NEQ(std::wstring(L"HELLO"), L"hello");
      TDOG_ASSERT_NEQ_MSG(L"hello", std::wstring(L"HELLO"), "assert_neq_msg(wstring)");

      // Vector type
      std::vector<int> va(3, 8);
      std::vector<int> vc(3, 9);
      TDOG_ASSERT_NEQ(va, vc);
      TDOG_ASSERT_NEQ_MSG(va, vc, "assert_neq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_lt)
    {
      TDOG_ASSERT_LT(tsqr(6), 37);
      TDOG_ASSERT_LT_MSG(tsqr(6), 37, "assert_lt_msg(37)");

      // Cannot use both literal
      TDOG_ASSERT_LT(std::string("ABC"), "abc");
      TDOG_ASSERT_LT(L"ABC", std::wstring(L"abc"));
      TDOG_ASSERT_LT(std::string("ABC"), std::string("abc"));
    }

    TDOG_TEST_CASE(assert_lte)
    {
      TDOG_ASSERT_LTE(tsqr(6), 37);
      TDOG_ASSERT_LTE(tsqr(6), 36);
      TDOG_ASSERT_LTE_MSG(tsqr(6), 37, "assert_lte_msg(37)");
      TDOG_ASSERT_LTE_MSG(tsqr(6), 36, "assert_lte_msg(36)");

      // Cannot use both literal
      TDOG_ASSERT_LTE(std::string("Hello"), "Hello");
      TDOG_ASSERT_LTE(L"Hello", std::wstring(L"Hello"));
      TDOG_ASSERT_LTE(std::string("Gello"), std::string("Hello"));
    }

    TDOG_TEST_CASE(assert_gt)
    {
      TDOG_ASSERT_GT(tsqr(6), 35);
      TDOG_ASSERT_GT_MSG(tsqr(6), 35, "assert_gt_msg(35)");

      // Cannot use both literal
      TDOG_ASSERT_GT(std::string("abc"), "ABC");
      TDOG_ASSERT_GT(L"abc", std::wstring(L"ABC"));
      TDOG_ASSERT_GT(std::string("abc"), std::string("ABC"));
    }

    TDOG_TEST_CASE(assert_gte)
    {
      TDOG_ASSERT_GTE(tsqr(6), 35);
      TDOG_ASSERT_GTE(tsqr(6), 36);
      TDOG_ASSERT_GTE_MSG(tsqr(6), 35, "assert_gte_msg(35)");
      TDOG_ASSERT_GTE_MSG(tsqr(6), 36, "assert_gte_msg(36)");

      // Cannot use both literal
      TDOG_ASSERT_GTE(std::string("Hello"), "Hello");
      TDOG_ASSERT_GTE(L"Hello", std::wstring(L"Hello"));
      TDOG_ASSERT_GTE(std::string("Iello"), std::string("Hello"));
    }

    TDOG_TEST_CASE(assert_double_eq)
    {
      TDOG_ASSERT_DOUBLE_EQ(5.0, 5.1, 0.2);
      TDOG_ASSERT_DOUBLE_EQ_MSG(5.0, 5.1, 0.2, "assert_double_eq_msg()");
    }

    TDOG_TEST_CASE(assert_double_neq)
    {
      TDOG_ASSERT_DOUBLE_NEQ(5.0, 6.1, 0.2);
      TDOG_ASSERT_DOUBLE_NEQ_MSG(5.0, 6.1, 0.2, "assert_double_neq_msg()");
    }

    TDOG_TEST_CASE(assert_stric_eq)
    {
      TDOG_ASSERT_STRIC_EQ("hello", "HELLO");
      TDOG_ASSERT_STRIC_EQ_MSG("hello", "HELLO", "assert_stric_eq_msg()");
    }

    TDOG_TEST_CASE(assert_stric_neq)
    {
      TDOG_ASSERT_STRIC_NEQ("hello", "hola");
      TDOG_ASSERT_STRIC_NEQ_MSG("hello", "hola", "assert_stric_neq_msg()");
    }

    TDOG_TEST_CASE(assert_array_eq)
    {
      // C array
      int a[] = {1, 4, 9};
      int b[] = {tsqr(1), tsqr(2), tsqr(3)};

      TDOG_ASSERT_ARRAY_EQ(a, b, 3);
      TDOG_ASSERT_ARRAY_EQ_MSG(a, b, 3, "assert_array_eq_msg(array)");

      // C++ container
      std::vector<int> va; va.push_back(1); va.push_back(4); va.push_back(9);
      std::vector<int> vb(va);

      TDOG_ASSERT_ARRAY_EQ(va, vb, 3);
      TDOG_ASSERT_ARRAY_EQ_MSG(va, vb, 3, "assert_array_eq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_array_neq)
    {
      // C array
      int a[] = {1, 4, 9};
      const int c[] = {tsqr(1), tsqr(2), tsqr(4)};

      TDOG_ASSERT_ARRAY_NEQ(a, c, 3);
      TDOG_ASSERT_ARRAY_NEQ_MSG(a, c, 3, "assert_array_neq_msg(array)");

      // C++ container
      std::vector<int> va; va.push_back(1); va.push_back(4); va.push_back(9);
      std::vector<int> vc(va); vc[2] = 16;

      TDOG_ASSERT_ARRAY_NEQ(va, vc, 3);
      TDOG_ASSERT_ARRAY_NEQ_MSG(va, vc, 3, "assert_array_neq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_throw)
    {
      TDOG_ASSERT_THROW( throw_exception(true), std::runtime_error );
      TDOG_ASSERT_THROW( throw_exception(true), std::exception );
    }

    TDOG_TEST_CASE(assert_no_throw)
    {
      TDOG_ASSERT_NO_THROW( throw_exception(false) );
      TDOG_ASSERT_NO_THROW( throw_exception(false) );
    }

    // positive_suite
    TDOG_CLOSE_SUITE
  }


  // ------------------------------
  // NEGATIVE INTERNAL
  // ------------------------------
  TDOG_EXPLICIT_SUITE(negative_suite)
  {
    TDOG_TEST_CASE(assert)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);
      TDOG_ASSERT(false);
      TDOG_ASSERT_MSG(false, "assert_msg()");
    }

    TDOG_TEST_CASE(assert_not)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);
      TDOG_ASSERT_NOT(true);
      TDOG_ASSERT_NOT_MSG(true, "assert_not_msg()");
    }

    TDOG_TEST_CASE(assert_eq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // Boolean
      TDOG_ASSERT_EQ(true, (5 == 6));

      // Integer type
      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_EQ(25, tsqr(6));
      TDOG_ASSERT_EQ_MSG(25, tsqr(6), "assert_eq_msg(int)");

      // Unsigned
      unsigned int ua = 5;
      unsigned int ub = 6;
      TDOG_ASSERT_EQ(ua, ub);
      TDOG_ASSERT_EQ_MSG(ua, ub, "assert_eq_msg(unsigned)");

      // String
      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_EQ("hello", std::string("HELLO"));
      TDOG_ASSERT_EQ_MSG("hello", std::string("HELLO"), "assert_eq_msg(string)");

      // Wide string
      TDOG_ASSERT_EQ(L"hello", std::wstring(L"HELLO"));
      TDOG_ASSERT_EQ_MSG(L"hello", std::wstring(L"HELLO"), "assert_eq_msg(wstring)");

      // Vector type
      // We have swapped correct values for incorrect ones
      std::vector<int> va(3, 8);
      std::vector<int> vc(3, 9);
      TDOG_ASSERT_EQ(va, vc);
      TDOG_ASSERT_EQ_MSG(va, vc, "assert_eq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_neq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // Boolean
      TDOG_ASSERT_NEQ(true, (5 == 5));

      // Integer type
      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_NEQ(25, tsqr(5));
      TDOG_ASSERT_NEQ_MSG(25, tsqr(5), "assert_neq_msg(int)");

      // Unsigned
      unsigned int ua = 5;
      unsigned int uc = 5;
      TDOG_ASSERT_NEQ(ua, uc);
      TDOG_ASSERT_NEQ_MSG(ua, uc, "assert_neq_msg(unsigned)");

      // String
      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_NEQ("hello", std::string("hello"));
      TDOG_ASSERT_NEQ_MSG("hello", std::string("hello"), "assert_neq_msg(string)");

      // Wide string
      TDOG_ASSERT_NEQ(L"hello", std::wstring(L"hello"));
      TDOG_ASSERT_NEQ_MSG(L"hello", std::wstring(L"hello"), "assert_neq_msg(wstring)");

      // Vector type
      // We have swapped correct values for incorrect ones
      std::vector<int> va(3, 9);
      std::vector<int> vb(3, 9);
      TDOG_ASSERT_NEQ(va, vb);
      TDOG_ASSERT_NEQ_MSG(va, vb, "assert_neq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_lt)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      TDOG_ASSERT_LT(tsqr(6), 36);
      TDOG_ASSERT_LT_MSG(tsqr(6), 36, "assert_lt_msg(36)");
    }

    TDOG_TEST_CASE(assert_lte)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      TDOG_ASSERT_LTE(tsqr(6), 35);
      TDOG_ASSERT_LTE_MSG(tsqr(6), 35, "assert_lte_msg(35)");
    }

    TDOG_TEST_CASE(assert_gt)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      TDOG_ASSERT_GT(tsqr(6), 36);
      TDOG_ASSERT_GT_MSG(tsqr(6), 36, "assert_gt_msg(36)");
    }

    TDOG_TEST_CASE(assert_gte)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      TDOG_ASSERT_GTE(tsqr(6), 37);
      TDOG_ASSERT_GTE_MSG(tsqr(6), 37, "assert_gte_msg(37)");
    }

    TDOG_TEST_CASE(assert_double_eq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // Erroneous delta
      TDOG_ASSERT_DOUBLE_EQ(5.0, 5.1, 0.02);
      TDOG_ASSERT_DOUBLE_EQ_MSG(5.0, 5.1, 0.02, "assert_double_eq_msg()");
    }

    TDOG_TEST_CASE(assert_double_neq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // Erroneous delta
      TDOG_ASSERT_DOUBLE_NEQ(5.0, 6.1, 2.0);
      TDOG_ASSERT_DOUBLE_NEQ_MSG(5.0, 6.1, 2.0, "assert_double_neq_msg()");
    }

    TDOG_TEST_CASE(assert_stric_eq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_STRIC_EQ("hello", "hola");
      TDOG_ASSERT_STRIC_EQ_MSG("hello", "hola", "assert_stric_eq_msg()");
    }

    TDOG_TEST_CASE(assert_stric_neq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_STRIC_NEQ("hello", "HELLO");
      TDOG_ASSERT_STRIC_NEQ_MSG("hello", "HELLO", "assert_stric_neq_msg()");
    }

    TDOG_TEST_CASE(assert_array_eq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // C array
      int a[] = {1, 4, 9};
      int c[] = {tsqr(1), tsqr(2), tsqr(4)};

      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_ARRAY_EQ(a, c, 3);
      TDOG_ASSERT_ARRAY_EQ_MSG(a, c, 3, "assert_array_eq_msg(array)");

      // C++ container
      std::vector<int> va; va.push_back(1); va.push_back(4); va.push_back(9);
      std::vector<int> vc(va); vc[2] = 16;

      TDOG_ASSERT_ARRAY_EQ(va, vc, 3);
      TDOG_ASSERT_ARRAY_EQ_MSG(va, vc, 3, "assert_array_eq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_array_neq)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // C array
      int a[] = {1, 4, 9};
      int b[] = {tsqr(1), tsqr(2), tsqr(3)};

      // We have swapped correct values for incorrect ones
      TDOG_ASSERT_ARRAY_NEQ(a, b, 3);
      TDOG_ASSERT_ARRAY_NEQ_MSG(a, b, 3, "assert_array_neq_msg(array)");

      // C++ container
      std::vector<int> va; va.push_back(1); va.push_back(4); va.push_back(9);
      std::vector<int> vb(va);

      TDOG_ASSERT_ARRAY_NEQ(va, vb, 3);
      TDOG_ASSERT_ARRAY_NEQ_MSG(vb, vb, 3, "assert_array_neq_msg(vector)");
    }

    TDOG_TEST_CASE(assert_throw)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      // Must fail
      TDOG_ASSERT_THROW( throw_exception(false), std::runtime_error );
      TDOG_ASSERT_THROW( throw_exception(true), int );
    }

    TDOG_TEST_CASE(assert_no_throw)
    {
      // Must fail
      TDOG_ASSERT_NO_THROW( throw_exception(true) );
    }

    TDOG_TEST_CASE(force_fail)
    {
      TDOG_TEST_FAIL("Test failure was forced");
    }

    TDOG_TEST_CASE(force_error)
    {
      TDOG_TEST_ERROR("Test error was forced");
    }

    TDOG_TEST_CASE(has_test_failed)
    {
      TDOG_SET_CONTINUE_ON_FAIL(true);

      TDOG_ASSERT(false);

      if (TDOG_HAS_TEST_FAILED())
      {
        TDOG_TEST_WARNING("This is failure warning");
      }
    }

    // negative_suite
    TDOG_CLOSE_SUITE
  }


  // ------------------------------
  // POSITIVE TESTS
  // ------------------------------
  TDOG_TEST_CASE(assert_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Look for expected strings in reports.
    // Will do this for all report types here, and
    // all prints share the same functionality, will
    // just check txt later.
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_HTML_VERBOSE);
    std::string htm = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_XML);
    std::string xml = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-cond")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("ASSERT"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_msg()"));
    TDOG_ASSERT_EQ(NPOS, htm.find("-cond")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, htm.find("ASSERT"));
    TDOG_ASSERT_NEQ(NPOS, htm.find("assert_msg()"));
    TDOG_ASSERT_EQ(NPOS, xml.find("-cond")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, xml.find("ASSERT"));
    TDOG_ASSERT_NEQ(NPOS, xml.find("assert_msg()"));
  }

  TDOG_TEST_CASE(assert_not_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_not_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_not) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_not_msg()"));
  }

  TDOG_TEST_CASE(assert_eq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_eq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(int)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(unsigned)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(string)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(wstring)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_neq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_neq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(int)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(unsigned)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(string)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(wstring)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_lt_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_lt_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_lt) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_lt_msg(37)"));
  }

  TDOG_TEST_CASE(assert_lte_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_lte_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_lte) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_lte_msg(37)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_lte_msg(36)"));
  }

  TDOG_TEST_CASE(assert_gt_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_gt_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_gt) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_gt_msg(35)"));
  }

  TDOG_TEST_CASE(assert_gte_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_gte_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_gte) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_gte_msg(35)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_gte_msg(36)"));
  }

  TDOG_TEST_CASE(assert_double_eq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_double_eq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_double_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_double_eq_msg()"));
  }

  TDOG_TEST_CASE(assert_double_neq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_double_neq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_double_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_double_neq_msg()"));
  }

  TDOG_TEST_CASE(assert_stric_eq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_stric_eq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_stric_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_stric_eq_msg()"));
  }

  TDOG_TEST_CASE(assert_stric_neq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_stric_neq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_stric_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_stric_neq_msg()"));
  }

  TDOG_TEST_CASE(assert_array_eq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_array_eq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_array_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_eq_msg(array)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_eq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_array_neq_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_array_neq_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_array_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-act")); // <- not found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_neq_msg(array)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_neq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_throw_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_throw_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_throw) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");
  }

  TDOG_TEST_CASE(assert_no_throw_positive)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_no_throw_positive.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(positive_suite::assert_no_throw) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 0) TDOG_TEST_FAIL("Assert test failed");
  }

  // ------------------------------
  // NEGATIVE TESTS
  // ------------------------------
  TDOG_TEST_CASE(assert_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Look for expected strings in reports.
    // Will do this for all report types here, and
    // all prints share the same functionality, will
    // just check txt later.
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_HTML_SUMMARY);
    std::string htm = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_XML);
    std::string xml = stm.str();

    TDOG_ASSERT_EQ(NPOS, txt.find("-cond")); // <- not found in summary
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_msg()"));
    TDOG_ASSERT_EQ(NPOS, htm.find("-cond")); // <- not found in summary
    TDOG_ASSERT_NEQ(NPOS, htm.find("assert_msg()"));
    TDOG_ASSERT_NEQ(NPOS, xml.find("-cond")); // <- xml are verbose
    TDOG_ASSERT_NEQ(NPOS, xml.find("assert_msg()"));

    // Also get verbose
    stm.str("");
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    txt = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_HTML_VERBOSE);
    htm = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_XML);
    xml = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("-cond")); // <- but found in verbose
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_msg()"));
    TDOG_ASSERT_NEQ(NPOS, htm.find("-cond")); // <- but found in verbose
    TDOG_ASSERT_NEQ(NPOS, htm.find("assert_msg()"));
    TDOG_ASSERT_NEQ(NPOS, xml.find("-cond")); // <- but found in verbose
    TDOG_ASSERT_NEQ(NPOS, xml.find("assert_msg()"));

  }

  TDOG_TEST_CASE(assert_not_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_not_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_not) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE); // verbose here
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("-cond")); // <- found
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_not_msg()"));
  }

  TDOG_TEST_CASE(assert_eq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_eq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE); // <- verbose here
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("-act")); // <- check in verbose
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(int)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(unsigned)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(string)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(wstring)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_eq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_neq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_neq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(int)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(unsigned)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(string)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(wstring)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_neq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_lt_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_lt_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_lt) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_lt_msg(36)"));
  }

  TDOG_TEST_CASE(assert_lte_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_lte_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_lte) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_lte_msg(35)"));
  }

  TDOG_TEST_CASE(assert_gt_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_gt_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_gt) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_gt_msg(36)"));
  }

  TDOG_TEST_CASE(assert_gte_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_gte_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_gte) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_gte_msg(37)"));
  }

  TDOG_TEST_CASE(assert_double_eq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_double_eq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_double_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_double_eq_msg()"));
  }

  TDOG_TEST_CASE(assert_double_neq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_double_neq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_double_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_double_neq_msg()"));
  }

  TDOG_TEST_CASE(assert_stric_eq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_stric_eq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_stric_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_stric_eq_msg()"));
  }

  TDOG_TEST_CASE(assert_stric_neq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_stric_neq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_stric_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_stric_neq_msg()"));
  }

  TDOG_TEST_CASE(assert_array_eq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_array_eq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_array_eq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_eq_msg(array)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_eq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_array_neq_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_array_neq_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_array_neq) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_neq_msg(array)"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("assert_array_neq_msg(vector)"));
  }

  TDOG_TEST_CASE(assert_throw_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_throw_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_throw) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");
  }

  TDOG_TEST_CASE(assert_no_throw_negative)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_no_throw_negative.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::assert_no_throw) ) );

    // Run and check result without relying ASSERT
    int rslt = tr.run();
    if (rslt != 1) TDOG_TEST_FAIL("Assert test failed");
  }

  TDOG_TEST_CASE(force_fail)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_force_fail.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::force_fail) ) );

    // Run and check result without relying ASSERT - throw exception instead
    int rslt = tr.run();
    if (rslt != 1) throw std::runtime_error("Assert test result failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("Test failure was forced"));
  }

  TDOG_TEST_CASE(force_error)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_force_error.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::force_error) ) );

    // Run and check result without relying ASSERT - throw exception instead
    int rslt = tr.run();
    if (rslt != 1) throw std::runtime_error("Assert test result failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("Test error was forced"));
  }

  TDOG_TEST_CASE(has_test_failed)
  {
    TDOG_SET_AUTHOR("Kuiper");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/assert_has_test_failed.txt");

    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(negative_suite::has_test_failed) ) );

    // Run and check result without relying ASSERT - throw exception instead
    int rslt = tr.run();
    if (rslt != 1) throw std::runtime_error("Assert test result failed");

    // Check for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("This is failure warning"));
  }

  // assert_suite
  TDOG_CLOSE_SUITE
}
//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
