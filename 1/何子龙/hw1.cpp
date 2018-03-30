//author��hezilong
//This is a parallel program using openMP
//which calculates all prime numbers from 2 to n  
#include <iostream>
#include <omp.h>
#include <cmath> 
#include <algorithm>
#include <memory.h>
#include <vector>
#include <time.h>
#include<windows.h>
using namespace std;

int main(){
	int n;
	cout << "������n��С��";
	cin >> n;
	int k = ceil(sqrt(n)/2);//�������߳�����
	cout <<"�߳�����" << k <<endl; 
	int interval = n/k == 0? n/k : n/k + 1;//�����С 
	cout <<"�����С:" <<interval<<endl; 
	int sqrtofn = floor(sqrt(n));
	bool isprime[n+1];
	double time[k];
	vector<int> prime;
#pragma omp parallel for
#ifndef _OPENMP
    fprintf(stderr, "OpenMP not supported");
#endif
	for(int i = 0; i < k; i++){//ע�����ѡ��Ҫ�� -fopenmp
	    //cout<<i<<endl;//����i�����֪��ʵ�ֲ���/���� 
	    LARGE_INTEGER nFreq; 
	    LARGE_INTEGER nBeginTime;  
		LARGE_INTEGER nEndTime;  
		QueryPerformanceFrequency(&nFreq);  
		QueryPerformanceCounter(&nBeginTime);
		int begin = i * interval+1;
		int end = min((i+1) * interval, n);
		for(int j = begin; j <= end; j++){
			isprime[j] = true;
			/*cout << j <<':'<< isprime[j] << endl;*/
		}
		if(begin == 1){
			for(int j = 2; j <= sqrtofn; j++){
				if(isprime[j]){
					#pragma omp critical
					{
						prime.push_back(j);
					}
					int t = j;
					while(t <= end){
						t += j;
						isprime[t] = false;
					}
				}
			}
			#pragma omp critical
			{
				prime.push_back(0);
			}
		}
		else{
			int checksize = 0;
			while(1){
				while(prime.size() == checksize){
				}
				int num = prime[checksize];
				checksize++;
				if(num == 0) break;
				int j = begin % num == 0 ? begin : begin + num - begin % num;//startingpoint
				while(j <= end){
					isprime[j] = false;
					j += num;
				}
			}
		}
		QueryPerformanceCounter(&nEndTime);
		time[i] = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000;
	}
	cout << "primes found:";
	cout << 2;
	for(int i = 3; i <= n; i++){
		//cout << i <<':'<< isprime[i] << endl;
		if(isprime[i]){
			cout <<','<< i;
		}
	}
	cout << endl;
	double maxt = 0;
	for(int i = 0; i < k; i++){
		cout << "thread" <<i <<':'<< time[i] <<"ms"<< endl;
		maxt = max(maxt, time[i]);
	}
	cout<<"�ʱ�䣻"<<maxt<<"ms";
	return 0;
} 
