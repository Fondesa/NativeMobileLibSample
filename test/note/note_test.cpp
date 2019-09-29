#include "note.hpp"
#include <gtest/gtest.h>

TEST(NoteTest, givenIdInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description");

    ASSERT_EQ(2, note.getId());
}

TEST(NoteTest, givenTitleInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description");

    ASSERT_EQ("dummy-title", note.getTitle());
}

TEST(NoteTest, givenDescriptionInConstructorWhenGetDescriptionIsInvokedThenDescriptionIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description");

    ASSERT_EQ("dummy-description", note.getDescription());
}

TEST(NoteTest, givenEqualFieldsWhenEqualityOperatorIsInvokedThenItReturnsTrue) {
    ASSERT_TRUE(Note(1, "a", "b") == Note(1, "a", "b"));
}

TEST(NoteTest, givenDifferentFieldsWhenEqualityOperatorIsInvokedThenItReturnsFalse) {
    EXPECT_FALSE(Note(1, "a", "b") == Note(2, "a", "b"));
    EXPECT_FALSE(Note(1, "a", "b") == Note(1, "c", "b"));
    EXPECT_FALSE(Note(1, "a", "b") == Note(1, "a", "c"));
}