#include "database_exception.hpp"
#include <gtest/gtest.h>

TEST(DatabaseExceptionTest, givenMsgWhenWhatIsInvokedThenMsgIsReturned) {
    std::string msg = "A dummy message.";
    auto exc = Db::Exception(msg);

    EXPECT_EQ(msg, exc.what());
}
