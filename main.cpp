#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

struct Route {
    std::string start;
    std::string end;
    std::string day;
    std::string time;
    double price;
};

std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::string cleanLine(const std::string& line) {
    std::string result = trim(line);
    if (!result.empty() && result.back() == '\r') {
        result.pop_back(); // Remove trailing \r
    }
    return result;
}

bool isValidRoute(const std::vector<std::string>& fields) {
    if (fields.size() != 5) return false;

    try {
        std::stod(fields[4]);
    } catch (...) {
        return false;
    }

    const std::string& time = fields[3];
    if (time.size() != 5 || time[2] != ':' ||
        !std::isdigit(time[0]) || !std::isdigit(time[1]) ||
        !std::isdigit(time[3]) || !std::isdigit(time[4])) {
        return false;
    }

    return true;
}

void loadRoutes(const std::string& filename, std::vector<Route>& routes, const std::string& errorFile) {
    std::ifstream file(filename);
    std::ofstream errFile(errorFile);
    std::string line;

    while (std::getline(file, line)) {
        line = cleanLine(line); // Clean the line
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        std::string segment;
        std::vector<std::string> fields;

        while (std::getline(iss, segment, ',')) {
            fields.push_back(trim(segment));
        }

        if (isValidRoute(fields)) {
            routes.push_back({fields[0], fields[1], fields[2], fields[3], std::stod(fields[4])});
        } else {
            errFile << line << std::endl; // Use std::endl to avoid \r\n mismatch
        }
    }
    file.close();
    errFile.close();
}

void printRoutes(const std::vector<Route>& routes) {
    std::cout << "result:\n";
    bool first = true;
    for (const auto& route : routes) {
        if (!first) {
            std::cout << std::endl;
        }
        first = false;

        std::cout << route.start << " " << route.end << " " << route.day << " "
                  << route.time << " " << std::fixed << std::setprecision(2)
                  << route.price;
    }
    std::cout << std::endl;
}

void queryA(const std::vector<Route>& routes, const std::string& start, const std::string& end) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.start == start && route.end == end) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryB(const std::vector<Route>& routes, const std::string& day) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.day == day) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryC(const std::vector<Route>& routes, double maxPrice) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.price <= maxPrice) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryD(const std::string& errorFile) {
    std::ifstream file(errorFile);
    std::string line;

    std::cout << "result:" << std::endl;

    bool first = true;
    while (std::getline(file, line)) {
        line = cleanLine(line); // Clean the line
        if (line.empty()) continue;

        

        std::cout << line << std::endl; // Use std::endl for consistent line endings
    }
    
}

int main() {
    const std::string dbFile = "db.csv";
    const std::string errFile = "err.txt";
    std::vector<Route> routes;

    loadRoutes(dbFile, routes, errFile);

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "a") {
            std::string start, end;
            std::cin >> start >> end;
            std::cin.ignore();
            queryA(routes, start, end);
        } else if (command == "b") {
            std::string day;
            std::cin >> day;
            std::cin.ignore();
            queryB(routes, day);
        } else if (command == "c") {
            double maxPrice;
            std::cin >> maxPrice;
            std::cin.ignore();
            queryC(routes, maxPrice);
        } else if (command == "d") {
            queryD(errFile);
        } else if (command == "e") {
            break;
        }
    }

    return 0;
}
