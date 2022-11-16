#include "post.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
bool compare(tuple<double, vector<int>> a, tuple<double, vector<int>> b){
	if(get<0>(a) > get<0>(b))
		return 1;
	if(get<0>(a) == get<0>(b))
		if(get<1>(a).size() > get<1>(b).size())
			return 1;

	return 0;
}
void post_proc(const char* name, const char* name2){
	char* token;
	FILE *out2 = fopen(name2, "w");
	FILE *in = fopen(name, "r");
	if(in == NULL ){
		in = fopen(name, "w");
		printf("in");
		fprintf(in, "\n");
		fprintf(out2, "0\n");
		return;
	}
	FILE *out = fopen(".tmp.open", "w");
	FILE *in2 = fopen(".tmp.tmp.cudaout", "r");
	char b[300] = {0};
	int count;
	int t;
	int cnt;
	double d;
	vector<tuple<double, vector<int>>> buf;
	vector<int> tmp;
	while(fgets(b, 300, in) ){
		count = 0;
		for(int i = 0; i < 300; i++){
			if(b[i] == ')')
				break;
			if(b[i] == ' ')
				count ++;
		}
		token = strtok(b, " ");
		
		while(token != NULL){
			if(!sscanf(token, "%d", &t))
				break;
			tmp.push_back(t);
			token = strtok(NULL, " ");
		}
		

		//printf("%s\n", token);
		sscanf(token, "(%lf)", &d);
		buf.push_back(make_tuple(d, tmp));
		tmp.clear();
		memset(b, 300, sizeof(char));
	}
	sort(buf.begin(), buf.end(), compare);

	// Write
	for(auto &vec : buf){
		memset(b, 300, sizeof(char));
		cnt = 0;
		cnt = sprintf(b, "%.1lf\t{", get<0>(vec));
		for(auto &item : get<1>(vec)){
			cnt += sprintf(b + cnt, "%d,", item);
		}
		cnt = sprintf(b + cnt -1 , "}\n");
		fprintf(out, "%s", b);
	}
	
	fclose(in);
	fclose(out);

	remove(name);				/* Remove old file */
	rename(".tmp.open", name);		/* Rename tmp to original */
	
	/**************************************************
	 * Out file 2
	 * 1. total number of items
	 * 2. each pruning 
	 * end
	 * ************************************************/
	int length[500] = {0};
	int be, index;
	fprintf(out2, "%ld\n", buf.size());  		/* Total num of items*/

	for(auto&a : buf){			/* Generate number after pruning*/
		length[get<1>(a).size()] += 1;
	}
	for(int i = 1; i < 500; i++){
		if(fscanf(in2, "%d\t%d\n", &index, &be) == 0)
			break;
		if(index != i)		/* Mismatch -> no further iteration*/
			break;
		fprintf(out2, "%d\t%d\t%d\n", i, be, length[i]);
	}

	fclose(in2);
	fclose(out2);
	return;
}


