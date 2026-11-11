#include <iostream>
#include "g2p_engine.h"
#include "synthesizer.h"
#include "text_processor.h"  // Include our new C++ header
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    // Create instances
    Dictionary m_voiceDict;
    Synthesizer m_engine;

    // Check if command line arguments are provided
    if (argc < 2) {
        std::cout << "Speech Synthesizer - Type text to speak (type 'quit' to exit)" << std::endl;

        while (true) {
            std::string text_input;
            std::cout << "\nEnter text to speak: ";
            std::getline(std::cin, text_input);

            if (text_input.empty() || text_input == "quit" || text_input == "exit") {
                std::cout << "Goodbye!" << std::endl;
                break;
            }

            // Preprocess the text using our C++ version
            std::string processed_text = preprocess_text(text_input);

            std::cout << "Input text: \"" << text_input << "\"" << std::endl;
            std::cout << "Processed Text: \"" << processed_text << "\"" << std::endl;

            m_engine.speak(processed_text, &m_voiceDict, 1.5f);
        }
    } else {
        // Process command line arguments
        std::string text_input = "";
        for (int i = 1; i < argc; ++i) {
            if (i > 1) text_input += " ";
            text_input += argv[i];
        }

        // Preprocess the text using our C++ version
        std::string processed_text = preprocess_text(text_input);

        std::cout << "Input text: \"" << text_input << "\"" << std::endl;
        std::cout << "Processed Text: \"" << processed_text << "\"" << std::endl;

        m_engine.speak(processed_text, &m_voiceDict, 1.5f);
    }

    return 0;
}
