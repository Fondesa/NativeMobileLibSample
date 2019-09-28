#include "draft.hpp"
#include <gtest/gtest.h>

TEST(DraftTest, givenTitleInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto draft = Draft("dummy-title", "dummy-description");

    ASSERT_EQ("dummy-title", draft.getTitle());
}

TEST(DraftTest, givenDescriptionInConstructorWhenGetDescriptionIsInvokedThenDescriptionIsReturned) {
    auto draft = Draft("dummy-title", "dummy-description");

    ASSERT_EQ("dummy-description", draft.getDescription());
}
