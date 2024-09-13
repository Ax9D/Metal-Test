#include <cstdio>
#include <Util.h>
#include <vector>

std::string read_to_string(const std::string &path) {
    FILE* file = fopen(path.c_str(), "r");

    if(file == nullptr) {
        printf("Error: %s\n", strerror(errno));
        std::exit(-1);
    }

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<char> bytes(size, 0);

    int result = fread(bytes.data(), bytes.size(), 1, file);

    std::string text = std::string(bytes.begin(), bytes.end());

    fclose(file);

    return text;
}

