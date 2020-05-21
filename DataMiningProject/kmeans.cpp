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
	const int column_count = 7;//有效数据列数，最后一列不要
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
		double ratio = min_value / max_value;//循环不变式外提，不知道编译器会不会自动优化
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
	result.push_back(_samples.at(index));//第一个聚类中心随机选取
	vector<double> distances(DBL_MAX, _samples_count);//保存每个点与聚类中心的距离
	for (int center_count = 1; center_count < _cluster_count; center_count++) {//剩下的聚类中心优先选择与之前中心最远的点。若有多个中心，则取最近的一个中心
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
	vector<double> probabilities(0, distances.size());//计算每个点能被选为中心的概率
	for (double& value : probabilities) {
		value = value / sum;
	}
	double target = rand() / double(RAND_MAX);//0-1随机数
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
	 for (Sample& s : result) {//type类型值用来存储有多少个点以该聚类中心为最近中心
		 s.data.resize(column_count);
		 for (double& v : s.data)
			 v = 0.0;
		 s.cluster_index = index++;
	 }
	 for (const Sample& sample : _samples) {//根据sample的cluster_index值，将数据累加到result相应位置
		 for (size_t i = 0; i < column_count; ++i) {
			 result[sample.cluster_index - 1].data[i] += sample.data[i];
		 }
		 ++result[sample.cluster_index - 1].type;
	 }
	 for (Sample& s : result) {//取平均值作为中心
		 for (double& value : s.data) {
			 value = value / s.type;
		 }
		 s.type = 0;//清空type值
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
		result = false;//大小不对，没有进行的必要
	}
	return result;
}
