#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <cctype>

const std::string DATA_FILE = "database.txt";

// Database class for file-based storage
class FileDatabase {
private:
    std::map<std::string, std::set<int>> data;
    
    // Load data from file
    void loadFromFile() {
        std::ifstream file(DATA_FILE);
        if (!file) return; // File doesn't exist yet
        
        data.clear();
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string index;
            int value;
            if (iss >> index >> value) {
                data[index].insert(value);
            }
        }
        file.close();
    }
    
    // Save data to file
    void saveToFile() {
        std::ofstream file(DATA_FILE);
        if (!file) {
            std::cerr << "Error: Cannot write to database file" << std::endl;
            return;
        }
        
        for (const auto& entry : data) {
            const std::string& index = entry.first;
            for (int value : entry.second) {
                file << index << " " << value << std::endl;
            }
        }
        file.close();
    }

public:
    FileDatabase() {
        loadFromFile();
    }
    
    ~FileDatabase() {
        saveToFile();
    }
    
    void insert(const std::string& index, int value) {
        data[index].insert(value);
    }
    
    void remove(const std::string& index, int value) {
        auto it = data.find(index);
        if (it != data.end()) {
            it->second.erase(value);
            if (it->second.empty()) {
                data.erase(it);
            }
        }
    }
    
    std::vector<int> find(const std::string& index) {
        std::vector<int> result;
        auto it = data.find(index);
        if (it != data.end()) {
            result.assign(it->second.begin(), it->second.end());
            std::sort(result.begin(), result.end());
        }
        return result;
    }
};

// Parse command line
std::vector<std::string> parseCommand(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    FileDatabase db;
    
    int n;
    std::cin >> n;
    std::cin.ignore(); // Ignore newline after n
    
    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);
        
        std::vector<std::string> tokens = parseCommand(line);
        
        if (tokens.empty()) continue;
        
        if (tokens[0] == "insert" && tokens.size() == 3) {
            std::string index = tokens[1];
            int value = std::stoi(tokens[2]);
            db.insert(index, value);
        }
        else if (tokens[0] == "delete" && tokens.size() == 3) {
            std::string index = tokens[1];
            int value = std::stoi(tokens[2]);
            db.remove(index, value);
        }
        else if (tokens[0] == "find" && tokens.size() == 2) {
            std::string index = tokens[1];
            std::vector<int> results = db.find(index);
            
            if (results.empty()) {
                std::cout << "null" << std::endl;
            } else {
                for (size_t j = 0; j < results.size(); j++) {
                    if (j > 0) std::cout << " ";
                    std::cout << results[j];
                }
                std::cout << std::endl;
            }
        }
    }
    
    return 0;
}