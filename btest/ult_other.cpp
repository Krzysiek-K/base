
#include "stdafx.h"


struct st1 {
	int	a, b;

	st1(){}
	st1(int _a,int _b) : a(_a), b(_b) {}
};

struct st2 {
	int	a, b, c;

	st2(){}
	st2(int _a,int _b,int _c) : a(_a), b(_b), c(_c) {}
};


void ult_chunkfile()
{
	vector<st1>	v1;
	vector<st2>	v2;

	{
		v1.push_back(st1(1,2));
		v1.push_back(st1(3,4));
		v1.push_back(st1(5,6));
		v2.push_back(st2(4,5,6));
		v2.push_back(st2(14,15,16));
		v2.push_back(st2(24,25,26));

		ChunkFileBuilder fb;
		fb.add("foo",v1);
		fb.add("bar",v2);
		fb.Save("test.cf");
	}
	v1.clear();
	v2.clear();
	{
		ChunkFile f("test.cf");
		f.read("foo",v1);
		f.read("bar",v2);
		printf("Test 1A\n");	for(int i=0;i<v1.size();i++) printf(" %2d %2d\n",v1[i].a,v1[i].b);
		printf("Test 1B\n");	for(int i=0;i<v2.size();i++) printf(" %2d %2d %2d\n",v2[i].a,v2[i].b,v2[i].c);
	}
	v1.clear();
	v2.clear();
	{
		ChunkFile f("test.cf");
		f.read("foo",v2);
		f.read("bar",v1);
		printf("Test 2A\n");	for(int i=0;i<v1.size();i++) printf(" %2d %2d\n",v1[i].a,v1[i].b);
		printf("Test 2B\n");	for(int i=0;i<v2.size();i++) printf(" %2d %2d %2d\n",v2[i].a,v2[i].b,v2[i].c);
	}
}
