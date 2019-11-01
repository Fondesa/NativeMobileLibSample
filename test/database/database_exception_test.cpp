#include <gtest/gtest.h>
#include "database/database_exception.hpp"

TEST(DatabaseExceptionTest, givenMsgWhenWhatIsInvokedThenMsgIsReturned) {
    std::string msg = "A dummy message.";
    auto exc = Db::Exception(msg);

    EXPECT_EQ(msg, exc.what());
}
