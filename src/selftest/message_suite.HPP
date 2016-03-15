//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : message_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_MESSAGE_SUITE_H
#define TDOG_MESSAGE_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test print macros and warning.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(message_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    TDOG_TEST_CASE(print)
    {
      TDOG_PRINT("PRINT<none>");
      TDOG_PRINT("This is a\nmulti-line message");
    }

    TDOG_TEST_CASE(printf_bool)
    {
      TDOG_PRINTF("PRINT<bool> %b", true);
      TDOG_PRINTF("PRINT<bool> %b", false);
    }

    TDOG_TEST_CASE(printf_int)
    {
      TDOG_PRINTF("PRINTF<int> %d, HEX: 0x%08X, OCT:0%o", 34283);
    }

    TDOG_TEST_CASE(printf_str)
    {
      std::string ts = "std::string";
      TDOG_PRINTF("PRINTF<str> %s", "hello world");
      TDOG_PRINTF("PRINTF<str> %s", ts);
    }

    TDOG_TEST_CASE(printf_wstr)
    {
      std::wstring ws = L"std::wstring";
      TDOG_PRINTF("PRINTF<wstr> %s", L"hello world");
      TDOG_PRINTF("PRINTF<wstr> %s", ws);
    }

    TDOG_TEST_CASE(printf_kstr)
    {
      TDOG_PRINTF("PRINTF<kstr> %k", L"hello\x09world");
    }

    TDOG_TEST_CASE(printf_char)
    {
      // Make sure no const casting problems
      char* ncs = const_cast<char*>("char*");
      const char *ccs = "const char*";
      wchar_t* nws = const_cast<wchar_t*>(L"wchar_t*");
      const wchar_t* cws = L"const wchar_t*";
      TDOG_PRINTF("PRINTF<str> %s", ncs);
      TDOG_PRINTF("PRINTF<str> %s", ccs);
      TDOG_PRINTF("PRINTF<str> %s", nws);
      TDOG_PRINTF("PRINTF<str> %s", cws);

      // Null string pointers
      TDOG_PRINTF("PRINTF<str> %s", (const char*)0);
      TDOG_PRINTF("PRINTF<wstr> %s", (const wchar_t*)0);
    }

    TDOG_TEST_CASE(printf_double)
    {
      TDOG_PRINTF("PRINTF<double> %f", 3.142);
    }

    TDOG_TEST_CASE(printf_pointer)
    {
      // Class pointers
      TDOG_PRINTF("PRINTF<class> %u", this);
      TDOG_PRINTF("PRINTF<class> %g", this);
      TDOG_PRINTF("PRINTF<class> %g", (std::vector<int>*)0); // <- null pointer
    }

    TDOG_TEST_CASE(printf_typed)
    {
      std::size_t TEST_SZ = 2001;
      enum test_t {VAL0, VAL1};

      // Must cast
      TDOG_PRINTF("PRINTF<enum> %g", static_cast<int>(VAL1));
      TDOG_PRINTF("PRINTF<size> %g", static_cast<int>(TEST_SZ));
    }

    TDOG_TEST_CASE(printf_unknown)
    {
      // Unknown type
      std::vector<int> temp;
      TDOG_PRINTF("PRINTF<class> %g", temp);
    }

    TDOG_TEST_CASE(print_warn)
    {
      TDOG_ASSERT(true);
      TDOG_TEST_WARNING("This is a warning");
      TDOG_ASSERT(true);
    }

    // Internal suite
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(print)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_print.txt");

    // Register tests with local runner
    tr.register_test( TDOG_GET_TCPTR(internal_suite::print) );

    TDOG_ASSERT_EQ(0, tr.run());

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

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINT<none>"));
    TDOG_ASSERT_NEQ(NPOS, htm.find("PRINT&lt;none&gt;"));
    TDOG_ASSERT_NEQ(NPOS, xml.find("PRINT&lt;none&gt;"));
  }

  TDOG_TEST_CASE(printf_bool)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_bool.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_bool) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINT<bool> true"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINT<bool> false"));
  }

  TDOG_TEST_CASE(printf_int)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_int.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_int) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<int> 34283, HEX: 0x000085EB, OCT:0102753"));
  }

  TDOG_TEST_CASE(printf_str)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_str.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_str) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> hello world"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> std::string"));
  }

  TDOG_TEST_CASE(printf_wstr)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_wstr.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_wstr) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<wstr> hello world"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<wstr> std::wstring"));
  }

  TDOG_TEST_CASE(printf_kstr)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_kstr.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_kstr) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);

    std::string txt = stm.str();
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<kstr> hello\\tworld"));
  }

  TDOG_TEST_CASE(printf_char)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_char.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_char) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> char*"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> const char*"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> wchar_t*"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> const wchar_t*"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<str> <nullptr>"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<wstr> <nullptr>"));
  }

  TDOG_TEST_CASE(printf_double)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_double.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_double) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<double> 3.142"));
  }

  TDOG_TEST_CASE(printf_pointer)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_pointer.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_pointer) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<class> addr:")); // <- we can't check pointer value
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<class> <nullptr>"));
  }

  TDOG_TEST_CASE(printf_typed)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_typed.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_typed) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<enum> 1"));
    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<size> 2001"));
  }

  TDOG_TEST_CASE(printf_unknown)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_printf_unknown.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::printf_unknown) );

    TDOG_ASSERT_EQ(0, tr.run());

    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string txt = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("PRINTF<class> <unknown type>"));
  }

  TDOG_TEST_CASE(warnings)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    tdog::runner tr(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/message_warnings.txt");
    tr.register_test( TDOG_GET_TCPTR(internal_suite::print_warn) );

    TDOG_ASSERT_EQ(0, tr.run());

    TDOG_ASSERT_EQ(1, tr.statistic_count(tdog::CNT_WARNINGS));

    // Check all reports. Should exist in summary type.
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_SUMMARY);
    std::string txt = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_HTML_VERBOSE);
    std::string htm = stm.str();

    stm.str("");
    tr.generate_report(stm, tdog::RS_XML);
    std::string xml = stm.str();

    TDOG_ASSERT_NEQ(NPOS, txt.find("This is a warning"));
    TDOG_ASSERT_NEQ(NPOS, htm.find("This is a warning"));
    TDOG_ASSERT_NEQ(NPOS, xml.find("This is a warning"));
  }

  TDOG_TEST_CASE(print_warn)
  {
    TDOG_SET_AUTHOR("Andy Thomas");
    TDOG_TEST_WARNING("This is an example warning - the test should pass");
  }

  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
