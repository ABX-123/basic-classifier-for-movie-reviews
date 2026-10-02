#pragma once

#include <string>
#include <utility>

class SentimentAnalyzer {
public:
    SentimentAnalyzer();
    ~SentimentAnalyzer();

    std::pair<std::string, double> analyze(const std::string& text);

private:
    void* analyzeFunction_;
    void* sentimentModule_;
};
