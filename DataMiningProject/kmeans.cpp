#include "kmeans.h"
#include <fstream>
#include <algorithm>
#include <ctime>

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
	const int column_count = 7;//��Ч�������������һ�в�Ҫ
	_samples_data.resize(column_count);
	while (!file.eof()) {
		for (int index = 0; index < column_count; ++index) {
			file >> value;
			sample.data.push_back(value);
			_samples_data.at(index).push_back(value);
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

void KMeans::min_max_normalize()
{
	for (vector<double>& data : _samples_data) {
		double min_value = *std::min_element(data.begin(), data.end());
		double max_value = *std::max_element(data.begin(), data.end());
		double ratio = min_value / max_value;//ѭ������ʽ���ᣬ��֪���������᲻���Զ��Ż�
		for (double& value : data) {
			value = (value - min_value) / ratio;
		}
	}
}

std::vector<Sample> KMeans::get_initial_center() const
{
	std::vector<Sample> result;
	srand((int)time(nullptr));
	int index = rand() % _samples.size();
	result.push_back(_samples.at(index));//��һ�������������ѡȡ
	for (int i = 1; i < _cluster_count; i++) {//ʣ�µľ�����������ѡ�����һ��������Զ�ĵ�

	}
	return result;
}

double KMeans::get_euclidian_distance(const Sample& lhs, const Sample& rhs)
{
	double result = 0.0;

	return result;
}

std::vector<Sample> KMeans::get_center() const
{
	return std::vector<Sample>();
}

bool KMeans::is_finished(const double& delta) const
{
	return false;
}
