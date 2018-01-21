#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

class Model {
	struct model *classifier;

public:
	Model();
	~Model();
	void learn(const std::vector<std::vector<double>>&, const std::vector<int>&);
	std::vector<std::pair<int, double>> predict(const std::vector<std::vector<double>>&);
    void load_file(std::string);
    void save_file(std::string);
};

#endif // MODEL_H
