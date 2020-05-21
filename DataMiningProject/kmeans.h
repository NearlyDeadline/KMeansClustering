#pragma once

#include <string>
#include <vector>


class Sample {
public:
	Sample(const Sample& sample);
	Sample();
public:
	std::vector<double> data;//����������
	int type;//������Դ����
	int cluster_index;//������һ�����࣬ȡֵ��ΧΪ1 - cluster_count
};

class KMeans {
public:
	KMeans(const std::string& data_path, const int& cluster_count);
	//��һ������Ϊ����·�����ڶ�������Ϊ��Ҫ����Ĵ���

	void print_result() const;
	//���������

private:
	void min_max_normalize();
	//��_samples_data��������׼��

	std::vector<Sample> get_initial_center() const;
	//��ȡ��ʼ�������ģ����Ϊ��ʼ�������ģ�����Ϊ_cluster_count

	size_t get_next_initial_center(const std::vector<double>& distances) const;
	//�������о�������distance��ʹ�����̷������һ����ʼ�������ĵ��±�

	double get_euclidian_distance(const Sample& lhs, const Sample& rhs) const;
	//�������������㣬���㲢����ŷ����þ��룬ע�ⷵ�ص���ƽ������Ҫ�Լ�����

	std::vector<Sample> get_center() const;
	//���������µľ������ģ����Ϊ�µľ������ģ�����Ϊ_cluster_count

	bool is_finished(std::vector<Sample> prev, std::vector<Sample> post, const double& delta) const;
	//���µľ�������post��ɵ�����prev���С�ڵ���delta������Ϊ��������ֹͣ�����������������

private:
	std::vector<std::vector<double>> _samples_data;//�������������ڼ���Ĳ��֣���һάΪ����ά�ȣ��ڶ�άΪ��������������ڷ�ά�Ȳ��м���
	std::vector<Sample> _samples;//��������������
	int _cluster_count;
};