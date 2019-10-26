#include <gtest/gtest.h>
#include "core/include_macros.hpp"
#include AMALGAMATION(note.hpp)

TEST(NoteTest, givenIdInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", "2019-10-26T10:03:38Z");

    ASSERT_EQ(2, note.getId());
}

TEST(NoteTest, givenTitleInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", "2019-10-26T10:03:38Z");

    ASSERT_EQ("dummy-title", note.getTitle());
}

TEST(NoteTest, givenDescriptionInConstructorWhenGetDescriptionIsInvokedThenDescriptionIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", "2019-10-26T10:03:38Z");

    ASSERT_EQ("dummy-description", note.getDescription());
}

TEST(NoteTest, givenLastUpdateDateInConstructorWhenGetLastUpdateDateIsInvokedThenLastUpdateDateIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", "2019-10-26T10:03:38Z");

    ASSERT_EQ("2019-10-26T10:03:38Z", note.getLastUpdateDate());
}

TEST(NoteTest, givenEqualFieldsWhenEqualityOperatorIsInvokedThenItReturnsTrue) {
    ASSERT_TRUE(Note(1, "a", "b", "2019-10-26T10:03:38Z") == Note(1, "a", "b", "2019-10-26T10:03:38Z"));
}

TEST(NoteTest, givenDifferentFieldsWhenEqualityOperatorIsInvokedThenItReturnsFalse) {
    EXPECT_FALSE(Note(1, "a", "b", "2019-10-26T10:03:38Z") == Note(2, "a", "b", "2019-10-26T10:03:38Z"));
    EXPECT_FALSE(Note(1, "a", "b", "2019-10-26T10:03:38Z") == Note(1, "c", "b", "2019-10-26T10:03:38Z"));
    EXPECT_FALSE(Note(1, "a", "b", "2019-10-26T10:03:38Z") == Note(1, "a", "c", "2019-10-26T10:03:38Z"));
    EXPECT_FALSE(Note(1, "a", "b", "2019-10-26T10:03:38Z") == Note(1, "a", "b", "2019-10-26T10:00:00Z"));
}