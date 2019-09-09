#include "core/compat_bad_optional_access_exception.hpp"
#include <gtest/gtest.h>

TEST(CompatBadOptionalAccessExceptionTest, whenWhatIsInvokedThenDefaultMsgIsReturned) {
    auto exc = CompatBadOptionalAccessException();

    EXPECT_STREQ("Bad optional access.", exc.what());
}
