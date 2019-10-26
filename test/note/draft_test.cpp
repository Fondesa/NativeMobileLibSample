#include <gtest/gtest.h>
#include "core/include_macros.hpp"
#include AMALGAMATION(draft.hpp)

TEST(DraftTest, givenTitleInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto draft = Draft("dummy-title", "dummy-description");

    ASSERT_EQ("dummy-title", draft.getTitle());
}

TEST(DraftTest, givenDescriptionInConstructorWhenGetDescriptionIsInvokedThenDescriptionIsReturned) {
    auto draft = Draft("dummy-title", "dummy-description");

    ASSERT_EQ("dummy-description", draft.getDescription());
}

TEST(DraftTest, givenEqualFieldsWhenEqualityOperatorIsInvokedThenItReturnsTrue) {
    ASSERT_TRUE(Draft("a", "b") == Draft("a", "b"));
}

TEST(DraftTest, givenDifferentFieldsWhenEqualityOperatorIsInvokedThenItReturnsFalse) {
    EXPECT_FALSE(Draft("a", "b") == Draft("c", "b"));
    EXPECT_FALSE(Draft("a", "b") == Draft("a", "c"));
}