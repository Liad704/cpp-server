#include <binary_functions.hpp>

int image_data_to_img(std::vector<char>& image_data) {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::string executable_path = std::string(buffer);
    std::string directory_path = executable_path.substr(0, executable_path.find_last_of("\\/"));
    //not working
    // Append file name to the path
    std::string file_path = directory_path + "\\image.jpg";

    std::ofstream outfile(file_path, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(image_data.data(), image_data.size());
        outfile.close();
        std::cout << "Image file 'image.jpg' created successfully.\n";
        return 0;
    } else {
        std::cerr << "Error opening file 'image.jpg' for writing.\n";
        return 1;
    }
}


