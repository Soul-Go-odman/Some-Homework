#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <random>
#include <cassert>

using namespace std;

vector<vector<int>> datas = {  // goods datas
		/*value volume weight*/
		{8,  11, 22},
		{9,  22, 15},
		{15, 12, 4},
		{6,  21, 5},
		{16, 21, 10},
		{9,  13, 19},
		{1,  1,  21},
		{4,  10, 20},

		{14, 13, 8},
		{9,  8,  13},
		{3,  6,  2},
		{7,  25, 3},
		{12, 13, 6},
		{4,  27, 17},
		{15, 12, 12},
		{5,  23, 5},

		{18, 12, 12},
		{5,  24, 4},
		{15, 23, 12},
		{4,  11, 21},
		{6,  6,  14},
		{2,  24, 23},
		{12, 28, 17},
		{14, 10, 15},

		{11, 20, 20},
		{9,  13, 22},
		{13, 25, 25},
		{13, 23, 0},
		{14, 5,  22},
		{13, 26, 15},
		{19, 30, 25},
		{4,  15, 13},
};
/*------------------Parameters------------------*/
const int GOODS_COUNT = int(datas.size());
int ITERATOR = 20;
const long POPULATION_SIZE = 60;

const int VOLUME_LIMIT = 80;
const int WEIGHT_LIMIT = 80;
/*--------------------------------------------*/


class Goods {
public:
	Goods() : _value(0), _volume(0), _weight(0) {}

	int get_value() { return _value; }
	int get_volume() { return _volume; }
	int get_weight() { return _weight; }

	void set_value(int value) { _value = value; }
	void set_volume(int volume) { _volume = volume; }
	void set_weight(int weight) { _weight = weight; }

private:
	int _value;
	int _volume;
	int _weight;
};


class GA {  /*  Genetic Algorithm  */
public:
	GA(const int goods_count, const long population_size, const int iterator, const vector<Goods>& task) :
		_chromosome_size(goods_count),
		_population_size(population_size),
		_iterator(iterator),
		_task(task) {

		int _sum_volume = 0, _sum_weight = 0, _sum_value = 0;
		double pGene = 0;  // 随机生成的概率 0~1
		const double pThreshold = 0.6;  // 概率阈值 大于该阈值置为1，否则为0

		Chromosome _chromosome[60];
		// Chromosome _chromosome[_population_size]; // Can't use in VC, but gcc
		for (long i = 0; i < _population_size; ++i) {
			_chromosome[i].set_num_gene(_chromosome_size);
			_sum_value = 0, _sum_volume = 0, _sum_weight = 0;
			for (int j = 0; j < _chromosome_size; j++) {
				pGene = abs((double(rand()) / (double(RAND_MAX) + 1)));
				_chromosome[i].set_gene(((pGene > pThreshold) ? 1 : 0));
				
				if (_chromosome[i].get_gene_item(j) == 1) {
					_sum_volume += _task[j].get_volume();
					_sum_weight += _task[j].get_weight();
					_sum_value += _task[j].get_value();
				}
			}
			
			if ((_sum_volume > VOLUME_LIMIT) || (_sum_weight > WEIGHT_LIMIT)) {
				_chromosome[i].empty_genes();
				i--;
				continue;
			}

			//cout << i << "  " << _sum_value << "  " << _sum_volume << "  " << _sum_weight << "  ";
			//for (int j = 0; j < _chromosome_size; j++) {
			//	cout << _chromosome[i].get_gene_item(j);
			//	if ((j + 1) % 32 == 0) cout << endl;
			//}
			_population.push_back(_chromosome[i]);
		}
	}

	double pCROSS = 0.8;  // 交叉概率
	double pMUTATION = 0.1;  // 变异概率

	class Chromosome {
	public:
		Chromosome() : _nums_gene(0), _fitness(0), selectProbability(0) {}

		double selectProbability;

		void set_num_gene(int chromosone_size) { _nums_gene = chromosone_size; }
		void empty_genes() { vector<int>().swap(_gene); }

		void set_gene(int gene) { _gene.push_back(gene); }
		vector<int> get_gene() { return _gene; }

		int get_gene_item(int index) { return _gene[index]; }
		void set_gene_item(int index, int vaule) { _gene[index] = vaule; }

		void set_fitness(int fitness) { _fitness = fitness; }
		int get_fitness() { return _fitness; }

		void set_constraint(int volume, int weight) {
			vector<int>().swap(_constraint);  // empty
			_constraint.push_back(volume);
			_constraint.push_back(weight);
		}
		int get_constraint(string element) { 
			if (element == "volume") {
				return _constraint[0];
			}
			else if (element == "weight") {
				return _constraint[1];
			}
			else {
				assert("Wrong Parameter!");
			}
		}

	private:
		int _nums_gene;
		vector<int> _gene;

		int _fitness;
		vector<int> _constraint;  // 约束：sum_Volume and sum_Weight
	};

	void fitness(vector<Chromosome>& population, vector<Goods>& _task) {
		unsigned int value_sum = 0, volume_sum = 0, weight_sum = 0;
		int gene_index = 0;
		for (auto& chromosome : population) {
			value_sum = 0, volume_sum = 0, weight_sum = 0;
			gene_index = 0;
			for (auto& gene : chromosome.get_gene()) {
				if (gene) {
					value_sum += _task[gene_index].get_value();
					volume_sum += _task[gene_index].get_volume();
					weight_sum += _task[gene_index].get_weight();
				}
				gene_index++;
			}

			/* Calculate fitness */
			chromosome.set_fitness(value_sum);
			chromosome.set_constraint(volume_sum, weight_sum);
		}
	}

	void filter(vector<Chromosome>& population) {

		long sum_fitness = 0;  // 计算概率值
		for (auto& chromosome : population) {
			sum_fitness += chromosome.get_fitness();
		}
		for (auto& chromosome : population) {
			/* sum of probability is 1. */
			chromosome.selectProbability = double(chromosome.get_fitness()) / double(sum_fitness);
		}

		double dropout;
		double cumulativeProbability;  // 累计概率
		long offset;

		long num_dropout = long(0.4 * population.size());
		for (long i = 0; i < num_dropout; ++i) {
			dropout = abs((double(rand()) / (double(RAND_MAX) + 1)));  //随机选中的概率 0~1
			cumulativeProbability = 0;
			offset = 0;
			for (auto& chromosome : population) {
				cumulativeProbability += chromosome.selectProbability;
				if ((chromosome.get_constraint("volume") > VOLUME_LIMIT) || 
					(chromosome.get_constraint("weight") > WEIGHT_LIMIT) || 
					(chromosome.get_fitness() < 0)) {
					population.erase(population.begin() + offset);
					break;
				}
				if (cumulativeProbability >= dropout) {
					if (chromosome.get_fitness() > 80) {  //如果该个体的适应度(fitness)大于80则不进行筛选直接进入下一轮
						i--;
						break;
					}

					population.erase(population.begin() + offset);
					break;
				}
				offset++;
			}
		}
	}

	void cross(vector<Chromosome>& population) {
		int first = -1;
		double corssProbability;
		int index = 0;
		int nGeneExchange = 0;  // 基因交换次数 1~3
		int gene_index = 0;  // 交换基因的索引
		int gene_temp = 0;  //  交换基因临时变量

		vector<Chromosome> tempPopulation;  //TODO

		for (auto& chromosome : population) {
			corssProbability = abs((double(rand()) / (double(RAND_MAX) + 1)));
			if (corssProbability < pCROSS) {
				if (chromosome.get_fitness() > 80) {  //如果该个体的适应度(fitness)大于80则该染色体保留副本
					tempPopulation.push_back(chromosome);
				}

				if (first < 0) first = index;
				else {
					nGeneExchange = int((rand() % 3) + 1);
					for (int i = 0; i < nGeneExchange; i++) {
						gene_index = int(rand() % chromosome.get_gene().size());
						gene_temp = population[first].get_gene_item(gene_index);
						population[first].set_gene_item(gene_index, population[index].get_gene_item(gene_index));
						population[index].set_gene_item(gene_index, gene_temp);
					}
					first = -1;
				}
			}
			index++;
		}

		for (auto& chromosome : tempPopulation) {
			population.push_back(chromosome);
		}
	}

	void mutation(vector<Chromosome>& population) {
		double mutationProbability;  // 基因变异概率
		int nMutation;  // 基因变异的数量
		int mutationGene;  // 变异的基因

		vector<Chromosome> tempPopulation;  // TODO

		for (auto& chromosome : population) {

			mutationProbability = abs((double(rand()) / (double(RAND_MAX) + 1)));
			if (mutationProbability < pMUTATION) {
				if (chromosome.get_fitness() > 80) {  //如果该个体的适应度(fitness)大于80则该染色体保留副本
					tempPopulation.push_back(chromosome);
				}

				nMutation = rand() % chromosome.get_gene().size() + 1;
				for (int i = 0; i < nMutation; i++) {
					mutationGene = rand() % chromosome.get_gene().size();
					chromosome.set_gene_item(mutationGene,
						int(1 - chromosome.get_gene_item(mutationGene)));
				}
			}
		}

		for (auto& chromosome : tempPopulation) {
			population.push_back(chromosome);
		}
	}

	Chromosome get_best(vector<Chromosome>& population) {
		Chromosome best;
		for (auto& chromosome : population) {
			//if ((chromosome.get_constraint("volume") < 81) && (chromosome.get_constraint("weight") < 81)) {
				if (chromosome.get_fitness() > best.get_fitness()) {
					best = chromosome;
				}
			//}
		}
		return best;
	}

	void print_result(Chromosome best, int index=-1) {
		if (index >= 0) {
			cout << right << setw(4) << index << " => ";
		}

		cout << "value: " << left << setw(6) << best.get_fitness();
		cout << "volume: " << left << setw(6) << best.get_constraint("volume");
		cout << "weight: " << left << setw(6) << best.get_constraint("weight");
		cout << "chromosome: ";
			int i = 1;
			for (auto& gene : best.get_gene()) {
				cout << gene;
				if (i % 8 == 0) cout << ' ';
				i++;
			}
			cout << endl;
	}

	void test(Chromosome test) {
		int _value = test.get_fitness();
		int _volume = test.get_constraint("volume");
		int _weight = test.get_constraint("weight");
	
		int sumValue = 0, sumVolume = 0, sumWeight = 0;
	
		int gene;
		for (int i = 0; i < test.get_gene().size(); i++) {
			gene = test.get_gene_item(i);
			if (gene) {
				sumValue += _task[i].get_value();
				sumVolume += _task[i].get_volume();
				sumWeight += _task[i].get_weight();
			}
		}
		cout << _value << "  " << _volume << "  " << _weight << endl;
		cout << sumValue << "  " << sumVolume << "  " << sumWeight << endl;
	}

	void run() {
		Chromosome best;
		for (int i = 0; i < _iterator; i++) {
			fitness(_population, _task);
			filter(_population);
			best = get_best(_population);
			print_result(best, i);
			Bestes.push_back(best);
			cross(_population);
			mutation(_population);
		}
		cout << "The best one" << endl;
		print_result(get_best(Bestes));
		//test(get_best(Bestes));

	}

private:
	int _iterator;          // 迭代次数
	int _chromosome_size;   // 物体总数
	long _population_size;  // 种群数量
	vector<Chromosome> _population; 
	vector<Goods> _task;
	vector<Chromosome> Bestes;
};



int main() {

	/*-----------------Get 32 goods---------------*/
	Goods goods[32];
	// Goods goods[GOODS_COUNT];  // Can't use in VC, but gcc
	vector<Goods> task;
	int goods_index = 0;
	for (auto& data : datas) {
		goods[goods_index].set_value(data[0]);
		goods[goods_index].set_volume(data[1]);
		goods[goods_index].set_weight(data[2]);
		task.push_back(goods[goods_index]);
		goods_index++;
	}
	//int i = 0;
	//for (auto& t : task) {
	//	cout << i << "  " << t.get_value() << "  " << t.get_volume() << "  " << t.get_weight() << endl;
	//	i++;
	//}
	/*--------------------------------------------*/

	GA genetic(GOODS_COUNT, POPULATION_SIZE, ITERATOR, task);
	genetic.run();

	return 0;
}