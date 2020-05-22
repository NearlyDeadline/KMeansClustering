// DataMiningProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//删除kmean.h, kmeans.cpp, DataMiningProject.cpp的所有被注释掉的代码，即可在控制台上输出聚类中心不断收敛的过程
#include "kmeans.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

int main()
{
    KMeans kmeans("seeds_dataset.txt", 3);
    kmeans.min_max_normalize();
    //vector<vector<Sample>> centers;
    int iterate_count =  kmeans.begin_clustering(0.0001/*, centers*/);
    /*cout.width(18);
    cout.fill('0');
    cout.precision(16);
    for (int i = 0; i < iterate_count; ++i) {
        cout << "迭代次数：" << i + 1 << "\n";
        for (const Sample& s : centers.at(i)) {
            for (const double& value : s.data)
                cout << value << " ";
            cout << "\n";
        }
    }*/
    vector<vector<size_t>> result = kmeans.get_result();
    int result_index = 1;
    cout << "迭代次数：" << iterate_count << "\n";
    for (auto i : result) {
        cout << "C" << result_index << "= { ";
        for (auto j : i)
            cout << j << " ";
        cout << "}\n{C" << result_index << "} = " << i.size() << endl;
        ++result_index;
    }
    return 0;
}
