#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

void reverseFileContents(const std::string& inputFileName,
                         const std::string& outputFileName)
{
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open "
                  << inputFileName
                  << " for reading."
                  << std::endl;
        return;
    }

    std::string fileContents(
        (std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());

    inputFile.close();

    std::reverse(fileContents.begin(), fileContents.end());

    std::ofstream outputFile(outputFileName);

    if (!outputFile.is_open())
    {
        std::cerr << "Error: Could not open "
                  << outputFileName
                  << " for writing."
                  << std::endl;
        return;
    }

    outputFile << fileContents;
    outputFile.close();

    std::cout << "Reversed file created successfully: "
              << outputFileName
              << std::endl;
}

int main()
{
    const std::string originalFile = "CSC450_CT5_mod5.txt";
    const std::string reversedFile = "CSC450-mod5-reverse.txt";

    std::string userInput;

    std::cout << "Enter text to append to the file: ";
    std::getline(std::cin, userInput);

    std::ofstream appendFile(originalFile, std::ios::app);

    if (!appendFile.is_open())
    {
        std::cerr << "Error: Could not open "
                  << originalFile
                  << " for appending."
                  << std::endl;

        return 1;
    }

    appendFile << std::endl << userInput;
    appendFile.close();

    std::cout << "Input appended successfully to "
              << originalFile
              << std::endl;

    reverseFileContents(originalFile, reversedFile);

    return 0;
}
