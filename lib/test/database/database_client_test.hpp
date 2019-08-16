#pragma once

#include <gtest/gtest.h>

class DatabaseClientTest : public ::testing::Test {
   protected:
    void TearDown() override;
};