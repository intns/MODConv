#include <MOD.hpp>
#include <filesystem>
#include <iostream>
#include <util/tokeniser.hpp>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "MODConv by Axiot\nUsage: MODConv.exe [commands file]";
        return EXIT_FAILURE;
    }

    std::ifstream cmdStream(argv[1], std::ios::in | std::ios::binary);
    const u32 cfSz = static_cast<u32>(std::filesystem::file_size(argv[1]));
    std::string fileStr(cfSz, '\0');
    cmdStream.read(fileStr.data(), cfSz);
    util::tokeniser tokeniser(fileStr);

    MOD modFile;
    while (!tokeniser.isEnd()) {
        const std::string& token = tokeniser.next();

        if (token == "load") {
            const std::string& filename = tokeniser.next();
            util::fstream_reader reader;
            reader.open(filename, std::ios_base::binary);
            if (!reader.is_open()) {
                std::cerr << "Unable to open " << filename << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << "Loading " << filename << std::endl;
            reader.endianness() = util::fstream_reader::Endianness::Big;
            reader.seekg(0, std::ios_base::beg);
            modFile.read(reader);
            reader.close();
        } else if (token == "write") {
            const std::string& filename = tokeniser.next();
            util::fstream_writer writer(util::fstream_writer::Endianness::Big);
            writer.open(filename, std::ios_base::binary);
            if (!writer.is_open()) {
                std::cerr << "Unable to open " << filename << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << std::endl << "Writing " << filename << std::endl;
            modFile.write(writer);
            writer.close();
        } else if (token == "close") {
            std::cout << std::endl;
            modFile.reset();
        }
    }

    ((void)std::getchar());

    return EXIT_SUCCESS;
}