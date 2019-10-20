#include "note/mutable_draft.hpp"
#include <gtest/gtest.h>
#include "core/compat_bad_optional_access_exception.hpp"
#include "note/incomplete_draft_exception.hpp"
#include "core/test_exceptions_macros.hpp"

TEST(MutableDraftTest, givenMutableDraftWithoutTitleWhenRequireTitleIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();

    ASSERT_LIB_THROW(draft.requireTitle(), CompatBadOptionalAccessException);
}

TEST(MutableDraftTest, givenMutableDraftWithTitleWhenRequireTitleIsInvokedThenTitleIsReturned) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_EQ("dummy-title", draft.requireTitle());
}

TEST(MutableDraftTest, givenMutableDraftWithoutDescriptionWhenRequireDescriptionIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();

    ASSERT_LIB_THROW(draft.requireDescription(), CompatBadOptionalAccessException);
}

TEST(MutableDraftTest, givenMutableDraftWithDescriptioneWhenRequireDescriptionIsInvokedThenDescriptionIsReturned) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");

    ASSERT_EQ("dummy-description", draft.requireDescription());
}

TEST(MutableDraftTest, givenTitleUpdatedMultipleTimesWhenRequireTitleIsInvokedThenLastTitleIsReturned) {
    auto draft = MutableDraft();
    draft.updateTitle("first");
    draft.updateTitle("second");
    draft.updateTitle("third");

    ASSERT_EQ("third", draft.requireTitle());
}

TEST(MutableDraftTest,
     givenDescriptionUpdatedMultipleTimesWhenRequireDescriptionIsInvokedThenLastDescriptionIsReturned) {
    auto draft = MutableDraft();
    draft.updateDescription("first");
    draft.updateDescription("second");
    draft.updateDescription("third");

    ASSERT_EQ("third", draft.requireDescription());
}

TEST(MutableDraftTest, givenEmptyDraftWhenHasTitleIsInvokedThenItReturnsFalse) {
    auto draft = MutableDraft();

    ASSERT_FALSE(draft.hasTitle());
}

TEST(MutableDraftTest, givenDraftWithTitleWhenHasTitleIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_TRUE(draft.hasTitle());
}

TEST(MutableDraftTest, givenEmptyDraftWhenHasDescriptionIsInvokedThenItReturnsFalse) {
    auto draft = MutableDraft();

    ASSERT_FALSE(draft.hasDescription());
}

TEST(MutableDraftTest, givenDraftWithDescriptionWhenHasDescriptionIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");

    ASSERT_TRUE(draft.hasDescription());
}

TEST(MutableDraftTest, givenEmptyDraftWhenIsIncompleteIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();

    ASSERT_TRUE(draft.isIncomplete());
}

TEST(MutableDraftTest, givenDraftWithoutTitleWhenIsIncompleteIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");

    ASSERT_TRUE(draft.isIncomplete());
}

TEST(MutableDraftTest, givenDraftWithoutDescriptionWhenIsIncompleteIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_TRUE(draft.isIncomplete());
}

TEST(MutableDraftTest, givenDraftWithTitleAndDescriptionWhenIsIncompleteIsInvokedThenItReturnsTrue) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");
    draft.updateDescription("dummy-description");

    ASSERT_FALSE(draft.isIncomplete());
}

TEST(MutableDraftTest, givenEmptyDraftWhenToDraftIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();

    ASSERT_LIB_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithoutTitleWhenToDraftIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");

    ASSERT_LIB_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithoutDescriptionWhenToDraftIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_LIB_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithTitleAndDescriptionWhenToDraftIsInvokedThenCompleteDraftIsReturned) {
    auto mutableDraft = MutableDraft();
    mutableDraft.updateTitle("dummy-title");
    mutableDraft.updateDescription("dummy-description");

    auto draft = mutableDraft.toDraft();

    ASSERT_EQ("dummy-title", draft.getTitle());
    ASSERT_EQ("dummy-description", draft.getDescription());
}

TEST(MutableDraftTest, givenEqualFieldsWhenEqualityOperatorIsInvokedThenItReturnsTrue) {
    auto first = MutableDraft();
    auto second = MutableDraft();

    EXPECT_TRUE(first == second);

    first.updateTitle("dummy-title");
    second.updateTitle("dummy-title");

    EXPECT_TRUE(first == second);

    first.updateDescription("dummy-description");
    second.updateDescription("dummy-description");

    EXPECT_TRUE(first == second);
}

TEST(MutableDraftTest, givenDifferentFieldsWhenEqualityOperatorIsInvokedThenItReturnsFalse) {
    auto first = MutableDraft();
    auto second = MutableDraft();

    first.updateTitle("dummy-title");

    EXPECT_FALSE(first == second);

    second.updateDescription("dummy-description");

    EXPECT_FALSE(first == second);

    first.updateDescription("dummy-different-description");

    EXPECT_FALSE(first == second);

    second.updateTitle("dummy-different-title");

    EXPECT_FALSE(first == second);

    second.updateTitle("dummy-title");

    EXPECT_FALSE(first == second);
}
