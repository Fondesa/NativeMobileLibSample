#pragma once

#include <gtest/gtest.h>

class SQLiteCursorTest : public ::testing::Test {
   protected:
    void SetUp() override;

    void TearDown() override;

    void insertRecord(std::string colText, double colDouble, int colInt, bool colBool);

    std::shared_ptr<Db::Cursor> selectAll();

    std::shared_ptr<Db::Database> db;
};