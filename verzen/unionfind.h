#include <vector>
using std::vector;

#include <list>
using std::list;

#include <iostream>
using std::cout;
using std::endl;


class Unionfind {
	public:
		Unionfind(int aantalk) {
			vertegenwoordigers = vector<int>(aantalk);
			leden_op_v = vector<list<int> >(aantalk);
			grootte = vector<int>(aantalk, 1);

			for(int i = 0; i < vertegenwoordigers.size(); i++) {
				vertegenwoordigers[i] = i;
				leden_op_v[i] = list<int>();
				leden_op_v[i].insert(leden_op_v[i].end(), i);
			}
		}

		void uni(int knoop1, int knoop2);
		int find(int knoop);


	private:
		vector<int> vertegenwoordigers;
		vector<list<int> > leden_op_v;
		vector<int> grootte;
};

void Unionfind::uni(int knoop1, int knoop2) {
	if(grootte[vertegenwoordigers[knoop2]] > grootte[vertegenwoordigers[knoop1]]) {
		std::cout << "jupla" << std::endl;
		std::swap(knoop1, knoop2);
	}
	grootte[vertegenwoordigers[knoop1]] += grootte[vertegenwoordigers[knoop2]];


	int v = vertegenwoordigers[knoop2];

	list<int>::iterator it = leden_op_v[v].begin();

	while(it !=  leden_op_v[v].end()) {
		vertegenwoordigers[*it] = vertegenwoordigers[knoop1];
		it++;
	}

	leden_op_v[vertegenwoordigers[knoop1]].merge(leden_op_v[v]);
}

int Unionfind::find(int knoop) {
	return vertegenwoordigers[knoop];
}