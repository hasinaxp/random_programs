#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//searching in array (without branches)
int linearSearch(const vector<int>& arr, int key)
{
	int pos = -1;
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i] == key)
		{
			pos = i;
			break;
		}
	}
	return pos;
}

int binarySearch(const vector<int> &arr, int key, int start = 0, int end = 0)
{
	int mid = (start + end) / 2;
	if (arr[mid] == key) return mid;
	else if (mid == start) return -1;
	else if (arr[mid] > key) 
	{
		return binarySearch(arr, key, start, mid - 1);
	}
	else
	{
		return binarySearch(arr, key, mid, end);
	}
}


//searching in tree (with branches)
int depthFirstSearch(const vector<int>& tree, int key, int root = 0)
{
	if (tree[root] == key) return root;
	int lc = (root + 1) * 2 -1;
	int rc = (root + 1) * 2;
	int res = -1;
	if (lc < tree.size())
		res = depthFirstSearch(tree, key, lc);
	if (res != -1) return res;
	if (rc < tree.size())
		res = depthFirstSearch(tree, key, rc);
	if (res != -1) return res;
	else return -1;
}

int breathFirstSearch(const vector<int>& tree, int key, int root = 0)
{
	if (root == 0 && tree[root] == key) return root;
	int lc = (root + 1) * 2 -1;
	int rc = (root + 1) * 2;
	
	if (lc < tree.size() && tree[lc] == key) return lc;
	if (rc < tree.size() && tree[rc] == key) return rc;

	int res = -1;
	if (lc < tree.size())
		res = breathFirstSearch(tree, key, lc);
	if (res != -1) return res;
	if (rc < tree.size())
		res = breathFirstSearch(tree, key, rc);
	if (res != -1) return res;
	else return -1;

}

int HuristicSearchBST(const vector<int>& tree, int key, int root = 0)
{
	if (tree[root] == key) return root;
	int lc = (root + 1) * 2 -1;
	int rc = (root + 1) * 2;
	if (tree[root] > key && lc < tree.size())
		return HuristicSearchBST(tree, key, lc);
	if (tree[root] < key && rc < tree.size())
		return HuristicSearchBST(tree, key, rc);
	return -1;
}



int main()
{
	//searching through non sorted data
	//vector<int> arr = {10, 5, 3, 7, 4, 9, 2, 8};
	//cout << linearSearch(arr, 7) << endl;
	//cout << linearSearch(arr, 1) << endl;

	//searching through sorted data
	//vector<int> arr = { 2,3,4,5,6,7,8 };
	//cout << binarySearch(arr, 7, 0, arr.size()-1) << endl;
	//cout << binarySearch(arr, 1, 0, arr.size()-1) << endl;

	vector<int> arr = { 6,3,8,2,4,7,9 };
	for (int i : arr) cout << i << ", ";
	cout << endl;
	cout << HuristicSearchBST(arr, 7) << endl;
	cout << HuristicSearchBST(arr, 1) << endl;
}

