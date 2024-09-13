#include <gtest/gtest.h>
#include "Util.h"  // Assuming ArrayVec is implemented in this header

// A simple non-primitive type for testing
struct TestObject {
    int value;
    TestObject(int val) : value(val) {}
    bool operator==(const TestObject& other) const { return value == other.value; }
};

// Test pushing elements to the ArrayVec (primitive type)
TEST(ArrayVecTest, PushPrimitive) {
    ArrayVec<uint32_t, 4> vec;
    vec.push(1);
    ASSERT_EQ(vec[0], 1);
    vec.push(2);
    ASSERT_EQ(vec[1], 2);

    ASSERT_EQ(vec.len(), 2);
}

// Test popping elements from the ArrayVec (primitive type)
TEST(ArrayVecTest, PopPrimitive) {
    ArrayVec<uint32_t, 4> vec;
    vec.push(10);
    vec.push(20);
    vec.pop();  // Removes the last element
    ASSERT_EQ(vec.back(), 10);
    ASSERT_EQ(vec.len(), 1);

    vec.pop();  // Now empty
    ASSERT_EQ(vec.len(), 0);
}

// Test pushing up to the capacity (primitive type)
TEST(ArrayVecTest, PushToCapacity) {
    ArrayVec<uint32_t, 4> vec;
    vec.push(1);
    vec.push(2);
    vec.push(3);
    vec.push(4);

    ASSERT_EQ(vec.len(), 4);
    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 3);
    ASSERT_EQ(vec[3], 4);
}

// Test pushing beyond the capacity without range checks
TEST(ArrayVecTest, PushBeyondCapacityCrashes) {
    ArrayVec<uint32_t, 4> vec;
    vec.push(1);
    vec.push(2);
    vec.push(3);
    vec.push(4);
    EXPECT_DEATH(vec.push(5), "");
}

// Test popping from an empty ArrayVec (primitive type)
TEST(ArrayVecTest, PopEmptyCrashes) {
    ArrayVec<uint32_t, 4> vec;
    vec.pop();
}

// Test pushing non-primitive types (class/struct)
TEST(ArrayVecTest, PushNonPrimitive) {
    ArrayVec<TestObject, 4> vec;
    vec.push(TestObject(100));
    vec.push(TestObject(200));

    ASSERT_EQ(vec[0], TestObject(100));
    ASSERT_EQ(vec[1], TestObject(200));
    ASSERT_EQ(vec.len(), 2);
}

// Test popping non-primitive types (class/struct)
TEST(ArrayVecTest, PopNonPrimitive) {
    ArrayVec<TestObject, 4> vec;
    vec.push(TestObject(300));
    vec.push(TestObject(400));

    vec.pop();  // Removes the last element
    ASSERT_EQ(vec.back(), TestObject(300));
    ASSERT_EQ(vec.len(), 1);

    vec.pop();  // Now empty
    ASSERT_EQ(vec.len(), 0);
}

// Test clearing ArrayVec with non-primitive types
TEST(ArrayVecTest, ClearNonPrimitive) {
    ArrayVec<TestObject, 4> vec;
    vec.push(TestObject(500));
    vec.push(TestObject(600));
    vec.clear();
}

// Test copying an ArrayVec with non-primitive types
TEST(ArrayVecTest, CopyNonPrimitive) {
    ArrayVec<TestObject, 4> vec;
    vec.push(TestObject(700));
    vec.push(TestObject(800));

    ArrayVec<TestObject, 4> copy = vec;
    ASSERT_EQ(copy.len(), 2);
    ASSERT_EQ(copy[0], TestObject(700));
    ASSERT_EQ(copy[1], TestObject(800));
}

// Test assignment operator with non-primitive types
TEST(ArrayVecTest, AssignmentNonPrimitive) {
    ArrayVec<TestObject, 4> vec1;
    vec1.push(TestObject(900));
    vec1.push(TestObject(1000));

    ArrayVec<TestObject, 4> vec2;
    vec2 = vec1;

    ASSERT_EQ(vec2.len(), 2);
    ASSERT_EQ(vec2[0], TestObject(900));
    ASSERT_EQ(vec2[1], TestObject(1000));
}