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

TEST(DraftTest, givenDraftWhenSetTitleIsInvokedThenTitleIsUpdated) {
    auto draft = Draft("dummy-title", "dummy-description");

    draft.setTitle("updated-title");

    ASSERT_EQ("updated-title", draft.getTitle());
}

TEST(DraftTest, givenDraftWhenSetDescriptionIsInvokedThenDescriptionIsUpdated) {
    auto draft = Draft("dummy-title", "dummy-description");

    draft.setDescription("updated-description");

    ASSERT_EQ("updated-description", draft.getDescription());
}