#pragma once

#include <string>
#include <vector>


class Sample {
public:
	Sample(const Sample& sample);
	Sample();
public:
	std::vector<double> data;//样本点数据
	int type;//样本来源种类
	int cluster_index;//属于哪一个聚类，取值范围为1 - cluster_count
};

class KMeans {
public:
	KMeans(const std::string& data_path, const int& cluster_count);
	//第一个参数为数据路径，第二个参数为想要聚类的簇数

	void print_result() const;
	//输出聚类结果

private:
	void min_max_normalize();
	//对_samples_data进行离差标准化

	std::vector<Sample> get_initial_center() const;
	//获取初始聚类中心，结果为初始聚类中心，数量为_cluster_count

	size_t get_next_initial_center(const std::vector<double>& distances) const;
	//根据已有距离数据distance，使用轮盘法获得下一个初始聚类中心的下标

	double get_euclidian_distance(const Sample& lhs, const Sample& rhs) const;
	//传入两个样本点，计算并返回欧几里得距离，注意返回的是平方，需要自己开方

	std::vector<Sample> get_center() const;
	//迭代计算新的聚类中心，结果为新的聚类中心，数量为_cluster_count

	bool is_finished(std::vector<Sample> prev, std::vector<Sample> post, const double& delta) const;
	//若新的聚类中心post与旧的中心prev差距小于等于delta，则视为已收敛，停止迭代，否则继续迭代

private:
	std::vector<std::vector<double>> _samples_data;//样本数据中用于计算的部分，第一维为样本维度，第二维为样本点个数。便于分维度并行计算
	std::vector<Sample> _samples;//保存所有样本点
	int _cluster_count;
};