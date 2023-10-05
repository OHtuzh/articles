#include <iostream>
#include <cstring>
#include <fstream>
#include <limits>
#include <cinttypes>
#include <charconv>
#include <system_error>

struct Arguments {
    char* filename;
    char* target_name;
};

Arguments ParseArgs(int argc, char** argv) {
    Arguments args;
    if (argc != 3) {
        std::cerr << "Not correct number of aruments" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (strncmp(argv[1], "--target=", 9) == 0) {
        args.target_name = argv[1] + 9;
        args.filename = argv[2];
    } else if (strncmp(argv[2], "--target=", 9) == 0) {
        args.filename = argv[1];
        args.target_name = argv[2] + 9;
    } else {
        std::cerr << "There's no argument '--target'" << std::endl;
        exit(EXIT_FAILURE);
    }

    return args;
}

bool TryParseUInt64T(const char* str, uint64_t& dest) {
    const char* str_end = str + strlen(str);
    auto parse_result = std::from_chars(str, str_end, dest);

    if (parse_result.ec == std::errc::invalid_argument) {
        std::cerr << "Invalid config: " << str << " is not a number" << std::endl;
        return false;
    }
    if (parse_result.ec == std::errc::result_out_of_range) {
        std::cerr << "Invalid config: " << str << " is too big" << std::endl;
        return false;
    }
    if (parse_result.ec == std::errc()) {
        if (parse_result.ptr != str_end) {
            std::cerr << "Invalid config: " << str << " is not a number" << std::endl;
            return false;
        }
        return true;
    }
    // actually it is std::unreachable();
    return false;
}

int Run(const Arguments& args) {
    std::ifstream fin(args.filename);

    if (!fin) {
        std::cerr << "There's no such file: " << args.filename << std::endl;
        return EXIT_FAILURE;
    }

    // думаю разумно считать, что строки больше 8192 символов тут быть не может)
    const size_t kBufferSize = 8192;
    char buffer[kBufferSize];

    const size_t kTargetNameLength = strlen(args.target_name);
    uint64_t min_price = std::numeric_limits<uint64_t>::max();
    bool found = false;

    while (fin.getline(buffer, kBufferSize)) {
        if (strncmp(buffer, args.target_name, kTargetNameLength) == 0) {
            found = true;
            uint64_t price;
            if (!TryParseUInt64T(buffer + kTargetNameLength + 1, price)) {
                return EXIT_FAILURE;
            }
            min_price = std::min(min_price, price);
        }
    }

    if (found) {
        std::cout << min_price << std::endl;
    } else {
        std::cout << "There's no such good!" << std::endl;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    Arguments args = ParseArgs(argc, argv);
    return Run(args);
}
