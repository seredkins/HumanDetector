#include <cstdlib>
#include "model.h"
#include "liblinear-1.8/linear.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

Model::Model() {
	classifier = nullptr;
}

Model::~Model() {
	if (classifier)
		free_and_destroy_model(&classifier);
}

void Model::learn(const std::vector<std::vector<double>> &features,
	          const std::vector<int> &labels)
{
	int f_n = features[0].size();
	int i_n = features.size();
	
	struct problem prob;
	prob.l = i_n;
	prob.bias = -1;
	prob.n = f_n;
	prob.y = Malloc(int, i_n);
	prob.x = Malloc(struct feature_node *, i_n);
	
	for (int i = 0; i < i_n; ++i) {
		prob.y[i] = labels[i];
		prob.x[i] = Malloc(struct feature_node, f_n + 1);
		prob.x[i][f_n].index = -1;
		for (int j = 0; j < f_n; ++j) {
			prob.x[i][j].index = j + 1;
			prob.x[i][j].value = features[i][j];
		}
	}

	struct parameter param;
	param.solver_type = L2R_L2LOSS_SVC_DUAL;
	param.C = 0.1;
	param.eps = 1e-4;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	classifier = train(&prob, &param);
	destroy_param(&param);
	free(prob.y);
	for (int i = 0; i < i_n; ++i)
		free(prob.x[i]);
	free(prob.x);
}

std::vector<std::pair<int, double>>
Model::predict(const std::vector<std::vector<double> > &features)
{
	std::vector<std::pair<int, double>> result;
	for (const std::vector<double> &f: features) {
		struct feature_node *x = Malloc(struct feature_node, f.size() + 1);
        for (int i = 0; i < int(f.size()); ++i) {
			x[i].index = i + 1;
			x[i].value = f[i];
		}
		x[f.size()].index = -1;
		double est;
		int label = predict_values(classifier, x, &est);
		result.emplace_back(label, est);
		free(x);
	}
	return result;
}

void Model::save_file(std::string filename) {
    save_model(filename.c_str(), classifier);
}

void Model::load_file(std::string filename) {
    classifier = load_model(filename.c_str());
}
