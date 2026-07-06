#ifndef INVERSIONCOUNTER_HPP
#define INVERSIONCOUNTER_HPP

#include<bits/stdc++.h>
using namespace std;

class InversionCounter{
    public:
    static int countInversions(vector<int>& arr){
        vector<int> temp(arr.size());
        return mergeSort(arr,temp,0,arr.size()-1);
    }
    private:
    static int mergeSort(vector<int>& arr,vector<int>&temp,int left,int right){
        int invCount=0;
        if(left<right){
            int mid=(left+right)/2;
            invCount+=mergeSort(arr,temp,left,mid);
            invCount+=mergeSort(arr,temp,mid+1,right);
            invCount+=merger(arr,temp,left,mid,right);
        }
        return invCount;
    }
    static int merger(vector<int>&arr,vector<int>&temp,int left,int mid,int right){
        int i=left;
        int j=mid+1;
        int k=left;
        int invCount=0;
        while(i<=mid && j<=right){
            if(arr[i]<=arr[j]){
                temp[k++]=arr[i++];
            }
            else{
                temp[k++]=arr[j++];
                invCount+=(mid+1-i);
            }
        }
        while(i<=mid){
            temp[k++]=arr[i++];
        }
        while(j<=right){
            temp[k++]=arr[j++];
        }
        for(int x=left;x<=right;x++){
            arr[x]=temp[x];
        }
        return invCount;
    }
};

#endif