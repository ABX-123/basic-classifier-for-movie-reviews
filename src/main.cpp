#include "SentimentAnalyzer.h"
#include <iostream>
#include <string>

int main() {
    try {
        SentimentAnalyzer analyzer;

        std::cout << "Simple C++ / Python Sentiment Analyzer\n"
                  << "Type 'quit' to exit.\n\n";

        std::string text;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, text) || text == "quit") {
                break;
            }

            auto [sentiment, confidence] = analyzer.analyze(text);
            
            std::cout << "Sentiment: " << sentiment << "\n";
            std::cout << "Confidence: " << confidence << "\n\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
