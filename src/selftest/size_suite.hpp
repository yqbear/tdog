//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : size_suite.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_SIZE_SUITE_H
#define TDOG_SIZE_SUITE_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <tdog.hpp>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Test integer sizes.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

TDOG_SUITE(size_suite)
{
  TDOG_TEST_CASE(test_sizes)
  {
    TDOG_SET_AUTHOR("Kuiper");

    TDOG_ASSERT_EQ(1, static_cast<int>(sizeof(tdog::i8_t)));
    TDOG_ASSERT_EQ(1, static_cast<int>(sizeof(tdog::u8_t)));
    TDOG_ASSERT_EQ(2, static_cast<int>(sizeof(tdog::i16_t)));
    TDOG_ASSERT_EQ(2, static_cast<int>(sizeof(tdog::u16_t)));
    TDOG_ASSERT_EQ(4, static_cast<int>(sizeof(tdog::i32_t)));
    TDOG_ASSERT_EQ(4, static_cast<int>(sizeof(tdog::u32_t)));
    TDOG_ASSERT_EQ(8, static_cast<int>(sizeof(tdog::i64_t)));
    TDOG_ASSERT_EQ(8, static_cast<int>(sizeof(tdog::u64_t)));
    TDOG_ASSERT_GTE(8, static_cast<int>(sizeof(tdog::xint_t)));
  }

  // size_suite
  TDOG_CLOSE_SUITE
}

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
