#include "core/include_macros.hpp"
#include <gtest/gtest.h>
#include AMALGAMATION(database_exception.hpp)

TEST(DatabaseExceptionTest, givenMsgWhenWhatIsInvokedThenMsgIsReturned) {
    std::string msg = "A dummy message.";
    auto exc = Db::Exception(msg);

    EXPECT_EQ(msg, exc.what());
}
