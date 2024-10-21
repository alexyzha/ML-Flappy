#include <vector>
#include <string>
#include <fstream>

template<typename T>
class GAHandler {
public:
    GAHandler();
    virutal void simulate(std::string ofile = "") = 0;
    virutal void generate_new() = 0;
    virutal void print_fittest() = 0;
private:
    std::vector<T*> current_population;
    T make_chromosome() = 0;
};