//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : test_fixture.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TEST_FIXTURE_H
#define TDOG_TEST_FIXTURE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "basic_test.hpp"

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
namespace tdog {

//---------------------------------------------------------------------------
// CLASS test_fixture
//---------------------------------------------------------------------------
template <class CONTYPE>
class test_fixture : public basic_test
{
  private:

  // Allowed to create new, but not copy.
  test_fixture(test_fixture const&);
  test_fixture& operator=(test_fixture const&);

  protected:

  // Implement virtual method
  virtual void _run_unprotected()
  {
    // This method wraps the run implementation.
    // This must be defined in header to allow generic template
    // type. The basic_test wraps this to catch any exceptions.
    bool setup_ok = false;

    CONTYPE container(&m_helper);

    try
    {
      // FIXTURE SETUP
      setup_ok = container.setup();
      m_helper.assert(setup_ok, true, "Fixture setup()", 0);
      if (!setup_ok) m_helper.raise_error("Fixture setup() failed", 0);
    }
    catch(...)
    {
      m_helper.assert(false, true, "Fixture setup()", 0);
      throw; // <- rethrow for protected run, will set error state
    }

    // MAIN RUN
    if (setup_ok)
    {
      try
      {
        // Main run
        container.tdog_impl();
      }
      catch(...)
      {
        // Ensure teardown in the case of
        // an implementation error, and re-throw
        bool teardown_ok = true;
        try { container.teardown(); } catch(...) { teardown_ok = false; }
        m_helper.assert(teardown_ok, true, "Fixture teardown()", 0);
        throw;
      }

      // FIXTURE TEARDOWN
      try
      {
        container.teardown();
        m_helper.assert(true, true, "Fixture teardown()", 0);
      }
      catch(...)
      {
        m_helper.assert(false, true, "Fixture teardown()", 0);
        throw;
      }
    }
  }

  public:

  // Construction
  test_fixture(const std::string& tname, // <- test name
    test_type_t ttype,  // <- test type
    const std::string& fname,   // <- filename
    int lnum,   // <- line number
    const std::string& utype = std::string(),   // <- user typename
    const std::string& rtype = std::string())   // <- repeated test typename
    : basic_test(tname, ttype, fname, lnum, utype, rtype) {}
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
