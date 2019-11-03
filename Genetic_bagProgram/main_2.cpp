#include <iostream>
#include <vector>
#include <algorithm>      //stable_sort()函数的头文件
#include <random>
#include<ctime>

using namespace std;

#define  BAG_MAX_VOLUME	   80   //背包最大容积
#define  BAG_MAX_WEIGHT    80	// 背包最大承重
#define  MAX_NUM		   20 // 最优结果连续出现MAX_NUM次后，停止迭代

//物品集合
const int value[32] = { 8, 9, 15, 6, 16, 9, 1, 4, 14, 9, 3, 7, 12, 4, 15, 5, 18, 5, 15, 4, 6, 2, 12, 14, 11, 9, 13, 13, 14, 13, 19, 4 };
const int volume[32] = { 11, 22, 12, 21, 21, 13, 1, 10, 13, 8, 6, 25, 13, 27, 12, 23, 12, 24, 23, 11, 6, 24, 28, 10, 20, 13, 25, 23, 5, 26, 30, 15 };
const int weight[32] = { 22, 15, 4, 5, 10, 19, 21, 20, 8, 13, 2, 3, 6, 17, 12, 5, 12, 4, 12, 21, 14, 23, 17, 15, 20, 22, 25, 0, 22, 15, 25, 13 };

//生成32位随机数
mt19937 Rand(time(NULL));

// 生成一个0-1之间的随机浮点数
float Rand_Float()
{
	return (Rand() % 100 / 100.0);
}

class Item// 单件物品定义
{
public:
	Item(int val, int vol, int wei) { value = val; volume = vol; weight = wei; }
	int volume;		//体积
	int weight;		// 重量
	int value;		// 价值
};

vector<Item> goods; //存放物品的容器

class Chromosome// 染色体
{
public:
	Chromosome(int goods_num) { fitness = 0; sum_weight = 0; sum_volume = 0; generation_id = 0; InitGene(goods_num); }
	Chromosome() { fitness = 0; sum_weight = 0; sum_volume = 0; generation_id = 0; gene.clear(); }
	void InitGene(int goods_num) { gene.assign(goods_num, 0); }  //初始化染色体
	int			fitness;		// 适应度
	int			sum_weight;		// 总重量
	int			sum_volume;		//总体积
	int			generation_id;  // 当前遗传代数
	vector<int>	gene;			// 存放基因的容器
};


bool ChromosomeSort(const Chromosome& one, const Chromosome& two)  //stable_sort函数中的排序模式
{
	return (one.fitness < two.fitness);
}

class GA// 遗传算法定义
{
public:
	GA(int gn, float cr, float vr, int goods_n) : m_group_num(gn), m_cross_rate(cr), m_varia_rate(vr), m_goods_num(goods_n)
	{
		Init_Goods();
	}

	void Init_Goods()// 初始化物品集合
	{
		cout << "*********物品集合***********" << endl;
		cout << "物品" << '\t' << "重量" << '\t' << "体积" << '\t' << "价值" << '\t' << endl;
		for (int i = 0; i < m_goods_num; i++)
		{
			m_goods.push_back(goods[i]);
			cout << i << '\t' << m_goods[i].weight << '\t' << m_goods[i].volume << '\t' << m_goods[i].value << endl;
		}
	}

	static float TotalFitness(const vector<Chromosome>& group)//总适应度
	{
		float sum = 0.0;
		for (size_t i = 0; i < group.size(); i++)
		{
			sum += group[i].fitness;
		}
		return sum;
	}

	void Operation()
	{
		int best_value_count = 0; //最优个体出现的次数
		int max_fitness = 0;      //当前最大适应度
		Init_Group();
		for (int i = 0; best_value_count < MAX_NUM; i++)
		{
			max_fitness = m_best_Chromosome.fitness;
			CalcFitness();         //适应度计算
			RecordOptimalChromosome(i);// 记录最优个体
			Select();
			Cross();
			Variation();

			if (m_best_Chromosome.fitness == max_fitness)
				best_value_count++;
			else
				best_value_count = 0;
		}
	}

	void PrintOptimal()// 显示最优个体
	{
		cout << endl;
		cout << "**************************最优解******************************" << endl;
		cout << "最优解代数" << "\t" << "总重量" << "\t" << "总体积" << "\t" << "总价值" << "\t" << "物品选择" << endl;
		cout << m_best_Chromosome.generation_id << "\t\t" << m_best_Chromosome.sum_weight << "\t" << m_best_Chromosome.sum_volume << "\t" << m_best_Chromosome.fitness << "\t";

		int count = 0;
		for (size_t i = 0; i < m_best_Chromosome.gene.size(); i++)
		{
			count++;
			if (m_best_Chromosome.gene[i] == 1)
				cout << count << " ";
		}
		cout << endl;
		cout << endl;
	}

	void Init_Group()// 初始化群体
	{
		m_best_Chromosome.InitGene(m_goods_num);

		int weight = 0, count = 0, volume = 0;

		for (int i = 0; i < m_group_num; i++)
		{
			m_group.push_back(Chromosome(m_goods_num));
			Chromosome& en = m_group.back();      //遗传群体容器中最上面一个染色体

			// 产生一个0.5-1.0倍的承重值
			float vir_capacity = ((Rand() % 50 + 50) / 100.0) * BAG_MAX_WEIGHT;

			// 初始化该个体基因中的每一位
			weight = 0;
			count = 0;
			while (weight <= vir_capacity)
			{
				int idx = Rand() % m_goods_num;

				// 连续3次都随机到基因中的同一个染色体，则该个体初始化结束
				if (count == 3) { break; }
				if (en.gene[idx]) { count++; continue; }

				en.gene[idx] = 1;
				weight += m_goods[idx].weight;
			}
		}
	}

	void CalcFitness()// 适应度计算
	{
		for (size_t i = 0; i < m_group.size(); i++)
		{
			Chromosome& en = m_group[i];
			int wei = 0, val = 0, vol = 0;
			for (size_t j = 0; j < en.gene.size(); j++)
			{
				if (en.gene[j])
				{
					wei += m_goods[j].weight;
					val += m_goods[j].value;
					vol += m_goods[j].volume;
				}
			}
			if (wei > BAG_MAX_WEIGHT) { en.fitness = 0; continue; }
			if (vol > BAG_MAX_VOLUME) { en.fitness = 0; continue; }

			en.fitness = val;
			en.sum_weight = wei;
			en.sum_volume = vol;
		}
	}

	void RecordOptimalChromosome(int gid)// 记录最优个体
	{
		stable_sort(m_group.begin(), m_group.end(), ChromosomeSort);   //对群体中的每一个个体的适应度从小到大排序
		Chromosome& en = m_group.back();                               //找出适应度最大的个体
		if (en.fitness > m_best_Chromosome.fitness)
		{
			m_best_Chromosome = en;
			m_best_Chromosome.generation_id = gid;
		}


		cout << gid << "\t\t" << en.sum_weight << "\t" << en.sum_volume << "\t" << en.fitness << "\t";
		int count = 0;
		for (size_t i = 0; i < en.gene.size(); i++)
		{
			count++;
			if (en.gene[i] == 1)
				cout << count << " ";
		}
		cout << endl;
	}

	void Select()// 选择
	{
		vector<Chromosome>  new_group;

		stable_sort(m_group.begin(), m_group.end(), ChromosomeSort);

		int src_group_num = m_group.size();

		// 保留群体中最优的前10%
		int reserve_num = (int)(src_group_num * 0.1);
		for (int i = 0; i < reserve_num; i++)
		{
			new_group.push_back(m_group.back());
			m_group.pop_back();
		}

		// 计算剩余个体的累积概率
		vector<float>  selected_rate;
		float sum_fitness = GA::TotalFitness(m_group);
		selected_rate.push_back(m_group[0].fitness / sum_fitness);   //
		for (size_t i = 1; i < m_group.size(); i++)
		{
			float cur_rate = selected_rate.back() + (m_group[i].fitness / sum_fitness);
			selected_rate.push_back(cur_rate);
		}

		// 利用轮赌法选择剩下的40%
		int left_num = (int)(src_group_num * 0.4);
		for (int i = 0; i < left_num; i++)
		{
			float rand_rate = Rand_Float();
			for (size_t idx = 0; idx < selected_rate.size(); idx++)
			{
				if (rand_rate <= selected_rate[idx])
				{
					new_group.push_back(m_group[idx]);
					break;
				}
			}
		}
		m_group.clear();
		m_group = new_group;// 新群体赋值
	}

	bool IsCross() { return (Rand_Float() <= m_cross_rate); }// 是否交叉	
	void Cross()// 交叉
	{
		size_t src_group_num = m_group.size();

		for (size_t i = 0; i < src_group_num - 1; i += 2)
		{
			Chromosome en1 = m_group[i];
			Chromosome en2 = m_group[i + 1];

			for (size_t j = 0; j < en1.gene.size(); j++)
			{
				if (IsCross())
				{
					int tmp = en1.gene[j];
					en1.gene[j] = en2.gene[j];
					en2.gene[j] = tmp;
				}
			}

			m_group.push_back(en1);
			m_group.push_back(en2);
		}
	}

	bool IsVariation() { return (Rand_Float() <= m_varia_rate); }// 是否变异
	void Variation()// 变异
	{
		for (size_t i = 0; i < m_group.size(); i++)
		{
			if (IsVariation())
			{
				Chromosome& en = m_group[i];
				for (size_t j = 0; j < en.gene.size(); j++)
				{
					if (IsVariation())
					{
						en.gene[j] = (en.gene[j] ? 0 : 1);
					}
				}
			}
		}
	}

private:

	int				    m_group_num;	// 群体数量
	float				m_cross_rate;	// 交叉概率
	float				m_varia_rate;	// 变异概率
	int				    m_goods_num;	// 物品总数
	Chromosome		    m_best_Chromosome;	// 最优个体
	vector<Chromosome>	m_group;	// 遗传 - 群体
	vector<Item>		m_goods;	// 总的物品集合

};

int main()
{
	for (int i = 0; i < 32; i++)
		goods.push_back(Item(value[i], volume[i], weight[i]));
	GA ga(1000, 0.5, 0.1, 32);
	cout << endl << "*************************迭代过程******************************" << endl;
	cout << "迭代次数" << "\t" << "总重量" << "\t" << "总体积" << "\t" << "总价值" << "\t" << "物品选择" << endl;
	ga.Operation();
	ga.PrintOptimal();

	return 0;
}