//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : repeated_suite.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_REPEATED_SUITE_H
#define TDOG_REPEATED_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Tests repeated tests with both virtual base type and template test.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
TDOG_SUITE(repeated_suite)
{
  // For convenience
  const std::size_t NPOS = std::string::npos;

  TDOG_EXPLICIT_SUITE(internal_suite)
  {
    // ---------------------------------
    // TEST FUNCTIONS AND TYPES
    // ---------------------------------
    // Template based test functions
    template <class T>
    T init_case(const char* s, bool lower) {
      // Accepts only narrow string as input,
      // but result will be specified type
      std::string temp(s);
      T rslt(temp.size(), 0);
      unsigned int c;
      for(std::size_t n = 0; n < temp.size(); ++n) {
        c = temp[n];
        if (lower && c > 0x40 && c < 0x5B) c += 0x20; // <- lower
        else if (!lower && c > 0x60 && c < 0x7B) c -= 0x20; // <- upper
        rslt[n] = static_cast<typename T::value_type>(c);
      }
      return rslt;
    }
    template <typename T> bool insensitive_compare(T s1, T s2) {
      // This is the function we want to test
      bool rslt = (s1 == s2);
      if (!rslt && s1.size() == s2.size())
      {
        rslt = true;
        wchar_t c1, c2;
        for(std::size_t n = 0; n < s1.size(); ++n) {
          c1 = s1[n]; c2 = s2[n];
          if (c1 >= 'A' && c1 <= 'Z') c1 += 0x20;
          if (c2 >= 'A' && c2 <= 'Z') c2 += 0x20;
          if (c1 != c2) return false;
        }
      }
      return rslt;
    }

    // Type for repeated fixture
    class base_obj
    {
    protected:
      int seed;
    public:
      const std::string type_name;
      base_obj(int s, const std::string& t) // <- non default
        : seed(s), type_name(t) {}
      virtual int counter() = 0; // <- pure virtual
    };

    class inc_obj : public base_obj
    {
    public:
      inc_obj(int s = 1000) // <- default here
        : base_obj(s, "inc_obj") {}
      int counter() { return seed++; }
    };

    class dec_obj : public base_obj
    {
    public:
      dec_obj(int s = 1000) // <- default here
        : base_obj(s, "test_dec") {}
      int counter() { return seed--; }
    };

    // ---------------------------------
    // TEST CASES
    // ---------------------------------
    TDOG_DEFINE_REPEATED(template_test)
    {
      USER_TYPE lowc = init_case<USER_TYPE>("Hello World", true);
      USER_TYPE uppc = init_case<USER_TYPE>("Hello World", false);

      // Test these different
      TDOG_ASSERT_NEQ(lowc, uppc);

      // Test comparison
      TDOG_ASSERT( insensitive_compare(lowc, uppc) );

      // Negative test
      lowc = init_case<USER_TYPE>("Hello World", true);
      uppc = init_case<USER_TYPE>("Goodbye World", false);

      TDOG_ASSERT_NOT( insensitive_compare(lowc, uppc) );
    }

    // Instantiate tests
    TDOG_TEST_REPEATED(narrow_test, template_test, std::string); // <- narrow
    TDOG_TEST_REPEATED(wide_test, template_test, std::wstring);  // <- wide

    // Define test
    TDOG_DEFINE_REPEATED(baseobj_test)
    {
      USER_TYPE t;

      TDOG_PRINTF("Type name: %s", t.type_name);
      int last = t.counter();
      TDOG_ASSERT_NEQ(last, t.counter());
    }

    // Instantiate tests
    TDOG_TEST_REPEATED(inc_test, baseobj_test, inc_obj);
    TDOG_TEST_REPEATED(dec_test, baseobj_test, dec_obj);

    TDOG_SUITE(nested_suite)
    {
      // Ensure we can instantiate in different suites
      TDOG_TEST_REPEATED(narrow_test, template_test, std::string);
      TDOG_TEST_REPEATED(wide_test, template_test, std::wstring);

      TDOG_TEST_REPEATED(inc_test, baseobj_test, inc_obj);
      TDOG_TEST_REPEATED(dec_test, baseobj_test, dec_obj);

      // nested_suite
      TDOG_CLOSE_SUITE
    }

    // internal_suite
    TDOG_CLOSE_SUITE
  }

  TDOG_TEST_CASE(repeated_baseobj)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/repeated_baseobj.txt");

    // Register tests
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::inc_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::dec_test) ) );

    // Run, no error
    TDOG_ASSERT_EQ(0, tr.run("*"));

    // Grab a copy of the report and look for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string s = stm.str();

    // Look for the type strings in the output
    // We should see this in verbose reports
    TDOG_ASSERT_NEQ(NPOS, s.find("REPEATED"));
    TDOG_ASSERT_NEQ(NPOS, s.find("baseobj_test"));
  }

  TDOG_TEST_CASE(repeated_template)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/repeated_template.txt");

    // Register tests
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::narrow_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::wide_test) ) );

    // Run, no error
    TDOG_ASSERT_EQ(0, tr.run("*"));

    // Grab a copy of the report and look for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string s = stm.str();

    // Look for the type strings in the output
    // We should see this in verbose reports
    TDOG_ASSERT_NEQ(NPOS, s.find("REPEATED"));
    TDOG_ASSERT_NEQ(NPOS, s.find("template_test"));
  }

  TDOG_TEST_CASE(repeated_cross_suite)
  {
    TDOG_SET_AUTHOR("Andy Thomas");

    tdog::runner tr;
    tr.set_default_report(tdog::RS_NONE);
    tr.add_report(tdog::RS_TEXT_VERBOSE, "./reports/internal/repeated_cross_suite.txt");

    // Register tests (in different suite)
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::narrow_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::wide_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::inc_test) ) );
    TDOG_ASSERT( tr.register_test( TDOG_GET_TCPTR(internal_suite::nested_suite::dec_test) ) );

    // Run, no error
    TDOG_ASSERT_EQ(0, tr.run("*"));

    // Grab a copy of the report and look for expected strings
    std::stringstream stm;
    tr.generate_report(stm, tdog::RS_TEXT_VERBOSE);
    std::string s = stm.str();

    // Look for the type strings in the output
    // We should see this in verbose reports
    TDOG_ASSERT_NEQ(NPOS, s.find("REPEATED"));
    TDOG_ASSERT_NEQ(NPOS, s.find("template_test"));
    TDOG_ASSERT_NEQ(NPOS, s.find("baseobj_test"));
  }

  // repeated_suite
  TDOG_CLOSE_SUITE
}
//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
