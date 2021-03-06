#ifndef __TEST_DATA_READER_HPP__
#define __TEST_DATA_READER_HPP__

enum ReadType {
    SHAPE = 0,
    START_END = 1,
    ADJACENCY_LIST = 2,
    PATH_LENGTH = 3,
    PATH_POINTS = 4
};

/*
 * Reads, constructs, and returns the specified
 * type of data from the file of the given name.
 */
void* read_test_data(const ReadType type, const std::string& name, const size_t size);

#endif // #ifndef __TEST_DATA_READER_HPP__