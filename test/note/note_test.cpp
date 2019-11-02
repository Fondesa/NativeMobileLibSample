#include <gtest/gtest.h>
#include "core/include_macros.hpp"
#include AMALGAMATION(note.hpp)

TEST(NoteTest, givenIdInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", 1572694125);

    ASSERT_EQ(2, note.getId());
}

TEST(NoteTest, givenTitleInConstructorWhenGetTitleIsInvokedThenTitleIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", 1572694125);

    ASSERT_EQ("dummy-title", note.getTitle());
}

TEST(NoteTest, givenDescriptionInConstructorWhenGetDescriptionIsInvokedThenDescriptionIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", 1572694125);

    ASSERT_EQ("dummy-description", note.getDescription());
}

TEST(NoteTest, givenLastUpdateDateInConstructorWhenGetLastUpdateDateIsInvokedThenLastUpdateDateIsReturned) {
    auto note = Note(2, "dummy-title", "dummy-description", 1572694125);

    ASSERT_EQ(1572694125, note.getLastUpdateTime());
}

TEST(NoteTest, givenEqualFieldsWhenEqualityOperatorIsInvokedThenItReturnsTrue) {
    ASSERT_TRUE(Note(1, "a", "b", 1572694125) == Note(1, "a", "b", 1572694125));
}

TEST(NoteTest, givenDifferentFieldsWhenEqualityOperatorIsInvokedThenItReturnsFalse) {
    EXPECT_FALSE(Note(1, "a", "b", 1572694125) == Note(2, "a", "b", 1572694125));
    EXPECT_FALSE(Note(1, "a", "b", 1572694125) == Note(1, "c", "b", 1572694125));
    EXPECT_FALSE(Note(1, "a", "b", 1572694125) == Note(1, "a", "c", 1572694125));
    EXPECT_FALSE(Note(1, "a", "b", 1572694125) == Note(1, "a", "b", 1572694124));
}