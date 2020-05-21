#include "kmeans.h"
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;
using std::getline;


Sample::Sample(const Sample& sample)
{
	this->data.assign(sample.data.begin(), sample.data.end());
	this->type = sample.type;
	this->cluster_index = sample.cluster_index;
}

Sample::Sample()
{
	this->type = 0;
	this->cluster_index = 0;
}

KMeans::KMeans(const std::string& data_path, const int& cluster_count)
{
	this->_cluster_count = cluster_count;
	ifstream file(data_path);
	Sample sample;
	double value;
	const int column_count = 7;//有效数据列数，最后一列不要
	while (!file.eof()) {
		for (int index = 0; index < column_count; ++index) {
			file >> value;
			sample.data.push_back(value);
		}
		file >> value;
		sample.type = (int)value;
		_samples.push_back(sample);
		sample.data.clear();
	}
	file.close();
}

void KMeans::print_result() const
{
}

void KMeans::normalize() const
{
}

std::vector<Sample> KMeans::get_initial_center() const
{
	return std::vector<Sample>();
}

double KMeans::get_euclidian_distance(const Sample& lhs, const Sample& rhs)
{
	return 0.0;
}

std::vector<Sample> KMeans::get_center() const
{
	return std::vector<Sample>();
}

bool KMeans::is_finished(const double& delta) const
{
	return false;
}
