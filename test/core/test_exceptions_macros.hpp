#pragma once

#include <gtest/gtest.h>
#include <string>
#include "core/exception_macros.hpp"

#ifdef EXCEPTIONS_ENABLED
#define ASSERT_LIB_THROW(statement, expectedException) \
    ASSERT_THROW(statement, expectedException)
#define EXPECT_LIB_THROW(statement, expectedException) \
    EXPECT_THROW(statement, expectedException)
#else
#define ASSERT_LIB_THROW(statement, expectedException) \
    ASSERT_EXIT(statement, ::testing::ExitedWithCode(1), ".*")
#define EXPECT_LIB_THROW(statement, expectedException) \
    EXPECT_EXIT(statement, ::testing::ExitedWithCode(1), ".*")
#endif
