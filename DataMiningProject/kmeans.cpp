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
	min_max_normalize();
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
	int _samples_count = _samples.size();
	srand((int)time(nullptr));
	int index = rand() % _samples_count;
	result.push_back(_samples.at(index));//��һ�������������ѡȡ
	vector<double> distances(DBL_MAX, _samples_count);//����ÿ������������ĵľ���
	for (int center_count = 1; center_count < _cluster_count; center_count++) {//ʣ�µľ�����������ѡ����֮ǰ������Զ�ĵ㡣���ж�����ģ���ȡ�����һ������
		for (int point_count = 0; point_count < _samples_count; point_count++) {
			double distance = std::min(get_euclidian_distance(result.at(center_count), _samples.at(point_count)), distances[point_count]);
		}
		index = get_next_initial_center(distances);
		result.push_back(_samples.at(index));
	}
	return result;
}

size_t KMeans::get_next_initial_center(const vector<double>& distances) const
{
	double sum = 0.0;
	for (const double& value : distances)
		sum += value;
	vector<double> probabilities(0, distances.size());//����ÿ�����ܱ�ѡΪ���ĵĸ���
	for (double& value : probabilities) {
		value = value / sum;
	}
	double target = rand() / double(RAND_MAX);//0-1�����
	sum = 0.0;
	size_t result = 0;
	size_t length = probabilities.size();
	for (; result < length; ++result) {
		sum += probabilities[result];
		if (sum > target) {
			return result;
		}
	}
	return distances.size() - 1;
}

double KMeans::get_euclidian_distance(const Sample& lhs, const Sample& rhs) const
{
	double result = 0.0;
	if (lhs.data.size() == rhs.data.size()) {
		for (vector<double>::const_iterator lhs_it = lhs.data.cbegin(), rhs_it = rhs.data.cbegin();
			lhs_it != lhs.data.cend(); ++lhs_it, ++rhs_it) {
			result += pow((*lhs_it) - (*rhs_it), 2);
		}
	}
	return result;
}

std::vector<Sample> KMeans::get_center() const
{
	 vector<Sample> result(_cluster_count);
	 size_t column_count = _samples.at(0).data.size();
	 int index = 1;
	 for (Sample& s : result) {//type����ֵ�����洢�ж��ٸ����Ըþ�������Ϊ�������
		 s.data.resize(column_count);
		 for (double& v : s.data)
			 v = 0.0;
		 s.cluster_index = index++;
	 }
	 for (const Sample& sample : _samples) {//����sample��cluster_indexֵ���������ۼӵ�result��Ӧλ��
		 for (size_t i = 0; i < column_count; ++i) {
			 result[sample.cluster_index - 1].data[i] += sample.data[i];
		 }
		 ++result[sample.cluster_index - 1].type;
	 }
	 for (Sample& s : result) {//ȡƽ��ֵ��Ϊ����
		 for (double& value : s.data) {
			 value = value / s.type;
		 }
		 s.type = 0;//���typeֵ
	 }
	 return result;
}

bool KMeans::is_finished(vector<Sample> prev, vector<Sample> post, const double& delta) const
{
	bool result = true;
	if (prev.size() == post.size() && prev.size() > 0) {
		for (size_t i = 0, point_count = prev.size(); i < point_count; i++) {
			for (size_t j = 0, column_count = prev.at(0).data.size(); j < column_count; j++) {
				if (abs((prev.at(i).data.at(j) - post.at(i).data.at(j))) > delta) {
					result = false;
					break;
				}
			}
		}
	}
	else {
		result = false;//��С���ԣ�û�н��еı�Ҫ
	}
	return result;
}
