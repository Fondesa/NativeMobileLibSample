#include <gtest/gtest.h>
#include "note/incomplete_draft_exception.hpp"

TEST(IncompleteDraftExceptionTest,
     givenNewEmptyDraftWhenIncompleteDraftExceptionIsThrownThenMessageShowsBothEmptyFields) {
    auto draft = MutableDraft();
    auto exc = IncompleteDraftException(draft);
    auto expected = "The following fields of the new draft aren't set: title, description";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenNewDraftWithTitleWhenIncompleteDraftExceptionIsThrownThenMessageShowsDescriptionField) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");
    auto exc = IncompleteDraftException(draft);
    auto expected = "The following fields of the new draft aren't set: description";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenNewDraftWithDescriptionWhenIncompleteDraftExceptionIsThrownThenMessageShowsTitleField) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");
    auto exc = IncompleteDraftException(draft);
    auto expected = "The following fields of the new draft aren't set: title";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenNewDraftWithTitleAndDescriptionWhenIncompleteDraftExceptionIsThrownThenMessageShowsInvalidUsage) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");
    draft.updateDescription("dummy-description");
    auto exc = IncompleteDraftException(draft);
    auto expected = "Invalid usage: the new draft is complete.";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenExistingEmptyDraftWhenIncompleteDraftExceptionIsThrownThenMessageShowsBothEmptyFields) {
    auto draft = MutableDraft();
    auto exc = IncompleteDraftException(2, draft);
    auto expected = "The following fields of the existing draft with id 2 aren't set: title, description";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenExistingDraftWithTitleWhenIncompleteDraftExceptionIsThrownThenMessageShowsDescriptionField) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");
    auto exc = IncompleteDraftException(2, draft);
    auto expected = "The following fields of the existing draft with id 2 aren't set: description";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenExistingDraftWithDescriptionWhenIncompleteDraftExceptionIsThrownThenMessageShowsTitleField) {
    auto draft = MutableDraft();
    draft.updateDescription("dummy-description");
    auto exc = IncompleteDraftException(2, draft);
    auto expected = "The following fields of the existing draft with id 2 aren't set: title";

    ASSERT_STREQ(expected, exc.what());
}

TEST(IncompleteDraftExceptionTest,
     givenExistingDraftWithTitleAndDescriptionWhenIncompleteDraftExceptionIsThrownThenMessageShowsInvalidUsage) {
    auto draft = MutableDraft();
    draft.updateTitle("dummy-title");
    draft.updateDescription("dummy-description");
    auto exc = IncompleteDraftException(2, draft);
    auto expected = "Invalid usage: the existing draft with id 2 is complete.";

    ASSERT_STREQ(expected, exc.what());
}