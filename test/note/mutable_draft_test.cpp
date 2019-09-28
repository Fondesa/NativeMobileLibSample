#include "note/mutable_draft.hpp"
#include <gtest/gtest.h>
#include <core/compat_bad_optional_access_exception.hpp>
#include <note/incomplete_draft_exception.hpp>

TEST(MutableDraftTest, givenMutableDraftWithoutTitleWhenRequireTitleIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();

    ASSERT_THROW(draft.requireTitle(), CompatBadOptionalAccessException);
}

TEST(MutableDraftTest, givenMutableDraftWithTitleWhenRequireTitleIsInvokedThenTitleIsReturned) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_EQ("dummy-title", draft.requireTitle());
}

TEST(MutableDraftTest, givenMutableDraftWithoutDescriptionWhenRequireDescriptionIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();

    ASSERT_THROW(draft.requireDescription(), CompatBadOptionalAccessException);
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

TEST(MutableDraftTest, givenDescriptionUpdatedMultipleTimesWhenRequireDescriptionIsInvokedThenLastDescriptionIsReturned) {
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

    ASSERT_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithoutTitleWhenToDraftIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");

    ASSERT_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithoutDescriptionWhenToDraftIsInvokedThenExceptionIsThrown) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");

    ASSERT_THROW(draft.toDraft(), IncompleteDraftException);
}

TEST(MutableDraftTest, givenDraftWithTitleAndDescriptionWhenToDraftIsInvokedThenCompleteDraftIsReturned) {
    auto mutableDraft = MutableDraft();
    mutableDraft.updateTitle("dummy-title");
    mutableDraft.updateDescription("dummy-description");

    auto draft = mutableDraft.toDraft();

    ASSERT_EQ("dummy-title", draft.getTitle());
    ASSERT_EQ("dummy-description", draft.getDescription());
}