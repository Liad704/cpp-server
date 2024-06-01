#include <binary_functions.hpp>

int image_data_to_img(std::vector<char>& image_data) {
    // Example binary data stored in a vector of chars (replace with your actual image data)
    // Name of the output file
    const std::string filename = "output_image.png"; // Change the extension accordingly

    // Open file in binary mode
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);

    // Check if the file was opened successfully
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return 1;
    }

    // Write the binary data to the file
    outFile.write(image_data.data(), image_data.size());
    // Close the file
    outFile.close();

    std::cout << "Image saved successfully to " << filename << std::endl;

    return 0;
}


