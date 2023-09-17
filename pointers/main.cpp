#include <gtest/gtest.h>
#include <cinttypes>

int32_t twice(int32_t value) {
    return value * 2;
}

int32_t add_two(int32_t value) {
    return value + 2;
}

void apply(int32_t* array, size_t size, int32_t (*modifier)(int32_t)) {
    for (int i = 0; i < size; ++i) {
        array[i] = modifier(array[i]);
    }
}

void my_memcpy(void* dest, const void* src, size_t number_of_bytes) {
    uint8_t* dest_bytes = (uint8_t*)dest;
    const uint8_t* src_bytes = (const uint8_t*)src;

    for (size_t i = 0; i < number_of_bytes; ++i) {
        dest_bytes[i] = src_bytes[i];
    }
}

TEST(POINTER, PRIMITIVE) {
    int value = 5;
    int* ptr = &value;
    EXPECT_EQ(value, *ptr);
}

TEST(POINTER, ARITHMETIC) {
    uint32_t* ptr = (uint32_t*)0x00000000;
    uint32_t* next_ptr = (uint32_t*)0x00000004;
    uint32_t* next_next_ptr = ptr + 2;

    EXPECT_EQ(ptr + 1, next_ptr);
    EXPECT_EQ(++next_ptr, (uint32_t*)0x00000008);
    EXPECT_EQ(next_next_ptr - ptr, 2);
}

TEST(POINTER, ARRAY) {
    int arr[3] = {1, 2, 3};
    EXPECT_EQ(*arr, 1);
    EXPECT_EQ(*(arr + 1), 2);
    EXPECT_EQ(*(arr + 2), 3);
    EXPECT_EQ(*(arr + 2), arr[2]);
}

TEST(POINTER, FUNCTION) {
    int32_t (*modifier)(int32_t);

    modifier = twice;
    EXPECT_EQ(modifier(8), 16);

    modifier = add_two;
    EXPECT_EQ(modifier(4), 6);
}

TEST(POINTER, FUNCTION_2) {
    int array[] = {1, 2, 3};

    apply(array, 3, twice);
    EXPECT_EQ(array[0], 2);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 6);
}

TEST(POINTER, MY_MEMCPY) {
    int32_t src[3] = {1, 2, 3};
    int32_t* dest = new int32_t[3];
    
    my_memcpy(dest, src, sizeof(int32_t) * 3);

    EXPECT_EQ(dest[0], 1);
    EXPECT_EQ(dest[1], 2);
    EXPECT_EQ(dest[2], 3);

    delete[] dest;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
