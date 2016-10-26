//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : test_case.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TEST_CASE_H
#define TDOG_TEST_CASE_H

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
// CLASS test_case
//---------------------------------------------------------------------------
template <class CONTYPE>
class test_case : public basic_test
{
  private:

  // Allowed to create new, but not copy.
  test_case(test_case const&);
  test_case& operator=(test_case const&);

  protected:

  // Implement virtual method
  virtual void _run_unprotected()
  {
    // This method wraps the run implementation.
    // This must be defined in header to allow generic template
    // type. The basic_test wraps this to catch any exceptions.
    CONTYPE container(&m_helper);

    // Main run
    container.tdog_impl();
  }

  public:

  // Construction
  test_case(const std::string& tname, // <- test name
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
