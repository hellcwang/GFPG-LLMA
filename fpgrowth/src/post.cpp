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
void post_proc(const char* name){
	char* token;
	FILE *in = fopen(name, "r");
	FILE *out = fopen(".tmp.open", "w");
	char b[300] = {0};
	char* eptr, *ptr;
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
		ptr = b;
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
	//sort(buf.begin(), buf.end(), compare);

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

	remove(name);
	rename(".tmp.open", name);
	return;
}


