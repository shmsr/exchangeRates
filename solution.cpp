#include<bits/stdc++.h>

#define fastIO ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;

class Solution 
{
	private:
		int dimension; /* dimension of the matrix */
		vector<vector<float>> exgMatrix, cost; /* exchange and cost matrix */
		vector<vector<int>> path; /* stores path */
		map<pair<string, string>, int> mapper; /* maps KRAKEN, BTC to a numeric value */
		map<int, pair<string, string>> swap_mapper; /* maps numerix value to KRAKEN, BTC (swap version of mapper above) */
		map<pair<int, int>, float> coord; /* stores the weights give, KRAKEN, BTC -> KRAKEN, USD = value */
		map<pair<int, int>, vector<int>> bestPath; /* stores the bestPath vector for key value (i,j) */
		void tracer(int i, int j); /* traces the path and stores in bestPath vector */
		void tracePaths(void); /* invokes the tracer to find all the bestPath in a problem */
		void formExchangeGraph(void); /* form the exchange graph matrix, intitial */
	public:
		void costMatrix(void); /* get final cost matrix */
		void floydWarshall(void); /* floyd marshall  algorithm */
		void exchangeRateRequest(string src_exc, string src_cur, string dest_exc, string dest_cur); /* exchange_request */
		void priceUpdate(string exchange, string src, string dest, float ff, float bf); /* price_update */
};

void Solution::tracer(int i, int j)
{
	/* get the best path vector */
	int x = i; int y = j;
	bestPath[make_pair(x,y)].emplace_back(x);
	while(i != j)
	{
		i = path[i][j];
		bestPath[make_pair(x,y)].emplace_back(i);
	}
}

void Solution::tracePaths(void)
{
	bestPath.clear();
	for (int i = 0; i < dimension; i++) 
	{
		for (int j = 0; j < dimension; j++) 
		{
			if (j != i && path[i][j] != -1) tracer(i,j);
		}
	}
}

void Solution::formExchangeGraph(void)
{
	dimension = mapper.size();
	exgMatrix.resize(dimension, vector<float>(dimension));
	cost.resize(dimension, vector<float>(dimension));
	path.resize(dimension, vector<int>(dimension));
	for(int i=0; i<dimension; i++)
	{
		for(int j=0; j<dimension; j++)
		{
			if(i == j) exgMatrix[i][i] = 0; /* KRAKEN, BTC -> KRAKEN, BTC = 0 */
			else if(swap_mapper[i].second == swap_mapper[j].second) exgMatrix[i][j] = 1.0; /* KRAKEN, BTC -> GBAX, BTC = 1 */	
			else if(coord.find(make_pair(i,j)) != coord.end()) exgMatrix[i][j] = coord[make_pair(i,j)]; /* fill in the weight of the edges given from price updates */
			else exgMatrix[i][j] = 0; /* As algorithm is not working for FLT_MAX, using 0 as rates as intially 0 as mentioned in PDF */
		}
	}
}

void Solution::costMatrix(void)
{
	for(int i=0; i<dimension; i++)
	{
		for(int j=0; j<dimension; j++)
		{
			cout<<cost[i][j]<<" ";
		}
		cout<<endl;
	}
}

void Solution::floydWarshall(void)
{
	/* form the adjacency matrix first from the price updates */
	formExchangeGraph();
	for(int i = 0; i < dimension; i++) 
	{
		for(int j = 0; j < dimension; j++) 
		{
			cost[i][j] = exgMatrix[i][j];
			path[i][j] = j;
		}
	}
	/* k iterations that is check for each entry (intermediate nodes) */
	for(int k = 0; k < dimension; k++) 
	{
		for(int i = 0; i < dimension; i++) 
		{
			for(int j = 0; j < dimension; j++) 
			{
				/* floyd marshall, cost calcuation at kth iteration (k as intermediate) */
				if((cost[i][k] * cost[k][j]) > cost[i][j]) 
				{
					cost[i][j] = cost[i][k] * cost[k][j];
					path[i][j] = path[i][k];
				}
			}
		}
	}
	tracePaths();
}

void Solution::exchangeRateRequest(string src_exc, string src_cur, string dest_exc, string dest_cur)
{
	/* eg, x = 0 as (KRAKEN, BTC is 0) */
	int x = mapper[make_pair(src_exc, src_cur)];
	/* eg, y = 1 as (KRAKEN, USD is 1) */
	int y = mapper[make_pair(dest_exc, dest_cur)];
	/* get the best path vector */
	auto vec = bestPath[make_pair(x,y)];
	cout<<"BEST_RATES_BEGIN "<<src_exc<<" "<<src_cur<<" "<<dest_exc<<" "<<dest_cur<<" "<<cost[x][y]<<endl;
	for(auto &i: vec) cout<<swap_mapper[i].first<<", "<<swap_mapper[i].second<<"\n";
	cout<<"BEST_RATES_END"<<endl;
}

void Solution::priceUpdate(string exchange, string src, string dest, float ff, float bf)
{
	/* if entry is already there and just the forward and backward factor have changed */
	if((mapper.find(make_pair(exchange, src)) != mapper.end()) && (mapper.find(make_pair(exchange, dest)) != mapper.end()))
	{
		/* updates coord only, ie forward and backward factors if entries already exists */
		coord[make_pair(mapper[make_pair(exchange, src)], mapper[make_pair(exchange, dest)])] = ff;
		coord[make_pair(mapper[make_pair(exchange, dest)], mapper[make_pair(exchange, src)])] = bf;
	}
	else
	{
		/* mapper[KRAKEN, BTC] = 0 */
		mapper[make_pair(exchange, src)] = mapper.size();
		/* mapper[KRAKEN, USD] = 1 */
		mapper[make_pair(exchange, dest)] = mapper.size();
		/* swap_mapper[0] = KRAKEN, BTC */
		swap_mapper[mapper[make_pair(exchange, src)]] = make_pair(exchange, src);
		/* swap_mapper[1] = KRAKEN, USD */
		swap_mapper[mapper[make_pair(exchange, dest)]] = make_pair(exchange, dest);
		/* coord[0,1] = forward_factor */
		coord[make_pair(mapper[make_pair(exchange, src)], mapper[make_pair(exchange, dest)])] = ff;
		/* coord[1,0] = backward_factor */
		coord[make_pair(mapper[make_pair(exchange, dest)], mapper[make_pair(exchange, src)])] = bf;
	}
}

int main()
{
	fastIO
	bool flag;
	string temp;
	Solution solution;
	while(true)
	{
		getline(cin, temp);
		istringstream iss(temp);
		vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>()); /* breaks string into tokens and storing them in vector of strings */
		if(results.size() == 6) 
		{
			flag = false;
			/* price update */
			solution.priceUpdate(results[1], results[2], results[3], stof(results[4]), stof(results[5]));
		}
		else if(results.size() == 5)
		{
			/* if price is updated, run the floydWarshall again, otherwise if we just providing exchange_reqs now, we don't need to run floydWarshall again and again because graph remains same */
			if(flag == false)
			{
				/* as price updates are there, new edges are added. so, run again */
				solution.floydWarshall();
				flag = true;
			}
			/* exchange request */
			solution.exchangeRateRequest(results[1], results[2], results[3], results[4]);
		}
		else exit(0);
	}
}