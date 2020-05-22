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
}

int KMeans::begin_clustering(const double& delta/*, vector<vector<Sample>>& centers*/)
{
	vector<size_t> initial_centers_index = get_initial_centers_index();
	vector<Sample> post_center;
	for (const size_t& index : initial_centers_index)
		post_center.push_back(_samples.at(index));
	//centers.push_back(post_center);
	vector<double> distance(_cluster_count, 0);
	vector<Sample> prev_center;
	int count = 1;
	do {
		for (Sample& s : _samples) {
			int center_index = 0;
			for (double& value : distance) {
				value = get_euclidian_distance(s, post_center.at(center_index++));
			}
			vector<double>::iterator min_iterator = std::min_element(distance.begin(), distance.end());
			center_index = 1;
			for (vector<double>::const_iterator it = distance.cbegin(); it != min_iterator; ++it, ++center_index);
			s.cluster_index = center_index;
		}
		prev_center = post_center;
		post_center = get_center();
		//centers.push_back(post_center);
		count++;
	} while (!is_finished(prev_center, post_center, delta));
	return count;
}

vector<vector<size_t>> KMeans::get_result() const
{
	vector<vector<size_t>> result(_cluster_count, vector<size_t>());
	size_t sample_count = 1;
	for (const Sample& s : _samples) {
		result.at(s.cluster_index - 1).push_back(sample_count++);
	}
	return result;
}

void KMeans::min_max_normalize()
{
	int index = 0;
	int column_count = 0;
	for (vector<double>& data : _samples_data) {
		double min_value = *std::min_element(data.begin(), data.end());
		double max_value = *std::max_element(data.begin(), data.end());
		double ratio = max_value - min_value;//循环不变式外提，不知道编译器会不会自动优化
		for (double& value : data) {
			_samples[index++].data[column_count] = (value - min_value) / ratio;
		}
		++column_count;
		index = 0;
	}
}

vector<size_t> KMeans::get_initial_centers_index() const
{
	vector<size_t> result;
	int _samples_count = _samples.size();
	srand((int)time(nullptr));
	int index = rand() % _samples_count;
	result.push_back(index);//第一个聚类中心随机选取
	vector<double> distances(_samples_count, 10000.0);//保存每个点与聚类中心的距离
	for (int center_count = 1; center_count < _cluster_count; center_count++) {//剩下的聚类中心优先选择与之前中心最远的点。若有多个中心，则取最近的一个中心
		for (int point_count = 0; point_count < _samples_count; point_count++) {
			distances[point_count] = std::min
			(get_euclidian_distance(_samples.at(result.at(center_count - 1)), _samples.at(point_count)), 
				distances.at(point_count));
		}
		index = get_next_initial_center_index(distances);
		result.push_back(index);
	}
	return result;
}

size_t KMeans::get_next_initial_center_index(const vector<double>& distances) const
{
	double sum = 0.0;
	for (const double& value : distances)
		sum += value;
	vector<double> probabilities(distances.size(), 0);//计算每个点能被选为中心的概率
	int index = 0;
	for (double& value : probabilities) {
		value = distances.at(index++) / sum;
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
			result += (*lhs_it - *rhs_it) * (*lhs_it - *rhs_it);
		}
	}
	return sqrt(result);
}

vector<Sample> KMeans::get_center() const
{
	 vector<Sample> result(_cluster_count);
	 if (_samples.size() > 0) {
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
	 }
	 return result;
}

bool KMeans::is_finished(const vector<Sample>& prev, const vector<Sample>& post, const double& delta) const
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
