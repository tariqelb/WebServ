#include <iostream>
#include <fstream>

int main() {
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Generate 20,000 lines with numbers from 1 to 20,000
    for (int i = 1; i <= 20000; ++i) {
        outputFile << i << std::endl;
    }

    outputFile.close();
    std::cout << "Text file 'output.txt' generated successfully." << std::endl;

    return 0;
}