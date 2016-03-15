//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : test_list.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TEST_LIST_H
#define TDOG_TEST_LIST_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <string>
#include <vector>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

 // Forward declarations
class basic_test;

//---------------------------------------------------------------------------
// CLASS test_list
//---------------------------------------------------------------------------
class test_list
{
  private:

  class sort_functor
  {
    public:
    bool full_sort;
    bool operator()(const basic_test* a, const basic_test* b) const;
  };

  public:

  // Test list storage. Objects
  // will be deleted on destruction.
  std::vector<basic_test*> storage;

  void sort(bool full);
  basic_test* find(const std::string& name, bool precise) const;

  // Convenience
  inline void clear() { storage.clear(); } // <- does not destroy objects
  inline basic_test* add(basic_test* ptr) { storage.push_back(ptr); return ptr; }
  inline basic_test* get(std::size_t n) const { return storage[n]; }
  inline std::size_t size() const { return storage.size(); }

};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

