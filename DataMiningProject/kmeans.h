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

	int begin_clustering(const double& delta/*, std::vector<std::vector<Sample>>& centers*/);
	//��ʼ���࣬deltaΪ������ֵ�����������ı仯ֵС��delta������Ϊ������centersΪ��ѡ�ı���ÿ�ξ������ĵ����ݽṹ

	std::vector<std::vector<size_t>> get_result() const;
	//���������

	void min_max_normalize();
	//��_samples_data��������׼��

	std::vector<Sample> get_samples() const { return _samples; }

private:
	std::vector<size_t> get_initial_centers_index() const;
	//��ȡ��ʼ�������ģ����Ϊ��ʼ����������_samples�е��±꣬����Ϊ_cluster_count

	size_t get_next_initial_center_index(const std::vector<double>& distances) const;
	//�������о�������distance��ʹ�����̷������һ����ʼ�������ĵ��±�

	double get_euclidian_distance(const Sample& lhs, const Sample& rhs) const;
	//�������������㣬���㲢����ŷ����þ��룬ע�ⷵ�ص���ƽ������Ҫ�Լ�����

	std::vector<Sample> get_center() const;
	//���������µľ������ģ����Ϊ�µľ������ģ�����Ϊ_cluster_count

	bool is_finished(const std::vector<Sample>& prev, const std::vector<Sample>& post, const double& delta) const;
	//���µľ�������post��ɵ�����prev���С�ڵ���delta������Ϊ��������ֹͣ�����������������

private:
	std::vector<std::vector<double>> _samples_data;//�������������ڼ���Ĳ��֣���һάΪ����ά�ȣ��ڶ�άΪ��������������ڷ�ά�Ȳ��м���
	std::vector<Sample> _samples;//��������������
	int _cluster_count;
};