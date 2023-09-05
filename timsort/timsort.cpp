#include <iostream>
#include <vector>
#include <random>
using namespace std;

struct runs
{
	int first;
	int size = 0;
};

void generaterandom(vector<int>& a, int size)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, 20000);
	a.reserve(size);
	for (int i = 0; i < size; i++)
	{
		a.push_back(distribution(generator));
	}
}

int getminrun(int n)
{
	int r = 0;
	while (n >= 64)
	{
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

void reverse(vector<int>& a, int first, int size)
{
	int last = first + size - 1;
	for (int i = first; i < first + (size / 2); i++)
	{
		int tmp = a[i];
		a[i] = a[last - i + first];
		a[last - i + first] = tmp;
	}
}

bool incnotstrict(int a, int b)
{
	if (a <= b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool decstrict(int a, int b)
{
	if (a > b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void insertionsort(vector<int>& a, int first, int size)
{
	for (int i = first + 1; i < size + first; i++)
	{
		int tmp = a[i];
		int j = i - 1;
		for (j; j >= first && a[j] > tmp; j--)
		{
			a[j + 1] = a[j];
		}
		a[j + 1] = tmp;
	}
}

int binarysearch(int value, vector<int>& a, int first, int size)
{
	if (size == 0)
	{
		return -1;
	}
	int mid = first + size / 2;
	if (value == a[mid])
	{
		return mid;
	}
	else
	{
		if (value < a[mid])
		{
			if ((mid - 1) < 0 || value > a[mid - 1])
			{
				return mid;
			}
			return binarysearch(value, a, first, size / 2);
		}
		else
		{
			if (value > a[mid])
			{
				if ((mid + 1) >= a.size() || value < a[mid + 1])
				{
					return mid + 1;
				}
				return binarysearch(value, a, mid + 1, size / 2);
			}
		}
	}
}

vector<runs> divide(vector<int>& a)
{
	int minrun = getminrun(a.size());
	int cur = 0;
	bool (*f)(int, int) = nullptr;
	vector<runs> res;
	runs run;
	for (int pos = 0; pos < a.size();)
	{
		int i = pos;
		for (i; i < a.size(); i++)
		{
			if (i == pos)
			{
				run.first = i;
				run.size = 1;
				continue;
			}
			if (i == pos + 1)
			{
				run.size++;
				if (incnotstrict(a[run.first], a[run.first + run.size - 1]))
				{
					f = incnotstrict;
				}
				if (decstrict(a[run.first], a[run.first + run.size - 1]))
				{
					f = decstrict;
				}
				continue;
			}
			if (f(a[run.first + run.size - 1], a[i]))
			{
				run.size++;
			}
			else
			{
				break;
			}
		}
		if (f == decstrict)
		{
			reverse(a, run.first, run.size);
		}
		if (run.size < minrun)
		{
			run.size = __min(minrun, a.size() - run.first);
		}
		insertionsort(a, run.first, run.size);
		res.push_back(run);
		pos = run.first + run.size;
	}
	return res;
}

void mergefunc(vector<int>&a, runs run1, runs run2)
{
	int min_gallop = 7;
	pair<int , int> last;
	runs min = (run1.size < run2.size ? run1 : run2);
	runs max = (min.first == run1.first ? run2 : run1);
	vector<int> tmpmin;
	vector<int> tmpmax;
	tmpmin.reserve(min.size);
	for (int i = min.first; i < min.first + min.size; i++)
	{
		tmpmin.push_back(a[i]);
	}
	tmpmax.reserve(max.size);
	for (int i = max.first; i < max.first + max.size; i++)
	{
		tmpmax.push_back(a[i]);
	}
	int i = 0;
	int j = 0;
	int k = __min(run1.first, run2.first);
	for (;i < tmpmax.size() && j < tmpmin.size(); k++)
	{
		if (tmpmax[i] < tmpmin[j])
		{
			a[k] = tmpmax[i];
			if (last.first != 1)
			{
				last.first = 1;
				last.second = 0;
			}
			last.second++;
			i++;
			if (last.second == min_gallop)
			{
				int until = binarysearch(tmpmin[j], tmpmax, i, tmpmax.size() - i);
				for (i; i < until; i++)
				{
					k++;
					a[k] = tmpmax[i];
				}
			}
		}
		else
		{
			a[k] = tmpmin[j];
			if (last.first != 2)
			{
				last.first = 2;
				last.second = 0;
			}
			last.second++;
			j++;
			if (last.second == min_gallop)
			{
				int until = binarysearch(tmpmax[i], tmpmin, j, tmpmin.size() - j);
				for (j; j < until; j++)
				{
					k++;
					a[k] = tmpmin[j];
				}
			}
		}
	}
	for (; i < tmpmax.size(); k++)
	{
		a[k] = tmpmax[i];
		i++;
	}
	for (; j < tmpmin.size(); k++)
	{
		a[k] = tmpmin[j];
		j++;
	}
}

bool issorted(vector<int> a)
{
	for (int i = 1; i < a.size(); i++)
	{
		if (!(a[i - 1] <= a[i]))
		{
			return false;
		}
	}
	return true;
}

void merge(vector<int>& a, vector<runs> run)
{
	vector<runs> stk;
	vector<runs>::reverse_iterator itx;
	vector<runs>::reverse_iterator ity;
	vector<runs>::reverse_iterator itz;

	for (int i = 0; i < run.size(); i++)
	{
		stk.push_back({ run[i].first, run[i].size });
		if (stk.size() > 2)
		{
			itx = stk.rbegin();
			ity = itx++;
			itz = ity++;
			while (stk.size() > 2 && !(((*itx).size > (*ity).size + (*itz).size) && ((*ity).size > (*itz).size)))
			{
				vector<runs>::reverse_iterator itmin = ((*itx).size < (*itz).size ? itx : itz);
				mergefunc(a, *ity, *itmin);
				(*ity).first = __min((*ity).first, (*itmin).first);
				(*ity).size += (*itmin).size;
				stk.erase(next(itmin).base());
			}
		}
	}
	while (stk.size() > 1)
	{
		itx = stk.rbegin();
		ity = itx++;
		mergefunc(a, *itx, *ity);
		(*ity).first = __min((*ity).first, (*itx).first);
		(*ity).size += (*itx).size;
		stk.erase(next(itx).base());
	}
}

void timsort(vector<int>& a)
{
	vector<runs> res = divide(a);
	merge(a, res);
}

int main()
{
	vector<int> a;
	int num;
	std::cout << "enter number of elements" << endl;
	std::cin >> num;
	generaterandom(a, num);
	for (int i = 0; i < a.size(); i++)
	{
		if (i % 10 == 0)
		{
			cout << endl;
		}
		std::cout << a[i] << "\t";
	}
	std::cout << endl;
	timsort(a);
	for (int i = 0; i < a.size(); i++)
	{
		std::cout << i << ". " << a[i] << endl;
	}
	cout << "sorted: " << boolalpha << issorted(a);
}