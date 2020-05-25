#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "intal.h"

static void trim(char* str)
{
	int m = strlen(str);
    int i = 0;
	while (str[i]=='0')
        i++;
	for(int j = i; j<m; j++)	
		str[j-i] = str[j];
	str[m-i] = '\0';
}

static void heapify(char** arr, int n, int i)
{
	char* temp;
	int root = i, r = 2*i+1, l = 2*i+2;
	if(l < n && intal_compare(arr[l], arr[root])==1)
		root = l;
	if(r < n && intal_compare(arr[r], arr[root])==1)
		root = r;
	if(root!=i)
	{
		temp = arr[root];
		arr[root] = arr[i]; 
		arr[i] = temp;
		heapify(arr, n, root);
	}
}

static char* repeat_subtract(char* intal1, char* intal2)
{
	char* s = intal2;
	char* temp;
	char* res = (char*)calloc(1001, sizeof(char));
	strcpy(res,intal1);
	while(intal_compare(res,s)!=-1)
	{						
		temp = intal_diff(res,s);
		strcpy(res,temp);
		free(temp);		
	}
	return res;
}

int intal_compare(const char* intal1, const char* intal2)
{
    int len1 = strlen(intal1), len2 = strlen(intal2);
    if (len1 > len2)
        return 1;
    else if (len2 > len1)
        return -1;
    else
    {
        int i = 0;
        while (i<len1 && intal1[i]==intal2[i])
            i++;
        if (i==len1)
            return 0;
        else
        {
            if (intal1[i] > intal2[i])
                return 1;
            else
                return -1;
        }
    }
}

char* intal_add(const char* intal1, const char* intal2)
{
    char *sum = (char*)calloc(1001,sizeof(char));
    int len1 = strlen(intal1), len2 = strlen(intal2);
    int i = len1-1, j = len2-1, pos = 1000;
    int n1, n2, carry = 0;
    while(i>=0 && j>=0)
    {
        n1 = intal1[i]-'0';
        n2 = intal2[j]-'0';
        sum[pos] = (n1+n2+carry)%10+'0';
        carry = (n1+n2+carry)/10;
        pos--;
        i--;
        j--;
    }
    if (i!=j)
    {
        if(i<0)
        {
            while(j>=0)
            {
                n2 = intal2[j]-'0';
                sum[pos] = (n2+carry)%10+'0';
                carry = (n2+carry)/10;
                pos--;
                j--;
            }
        }
        else
        {
            while(i>=0)
            {
                n1 = intal1[i]-'0';
                sum[pos] = (n1+carry)%10+'0';
                carry = (n1+carry)/10;
                pos--;
                i--;
            }
        }
    }
    if(carry)
        sum[pos] = carry+'0';
    else
        pos++;

    for(int i=0;i<(1000-pos+1);i++)
        sum[i] = sum[pos+i];
	sum[1000-pos+1] = '\0';

    return sum;
}

char* intal_diff(const char* intal1, const char* intal2) 
{
    char* zero = (char*)calloc(2,sizeof(char));
    int cmp = intal_compare(intal1, intal2);
	if(cmp==0)
    {
	    strcpy(zero, "0");
        return zero;
    }
	free(zero);

	int m = strlen(intal1), n = strlen(intal2);
	int maxlen = m>n?m:n;
	int k = maxlen-1;
	
	char* difference = (char*)calloc((maxlen+1),sizeof(int));
	char* temp1 = (char*)calloc(m,sizeof(char));
	char* temp2 = (char*)calloc(n,sizeof(char));
	strcpy(temp1, intal1);
	strcpy(temp2, intal2);
	difference[maxlen]='\0';

	if(cmp==1) 
	{	
		int i, j;
		for(i=m-1, j=n-1; i>=0 && j>=0; --i, --j)
		{
			if(temp1[i] >= temp2[j])
				difference[k--] = temp1[i] - temp2[j] + '0';
			else
			{
				if(temp1[i-1]!='0') 
					temp1[i-1]-=1;
				else
				{
					int key=i-1;
					while(temp1[key]=='0')
					{
						temp1[key]='9';
						key--;
					}
					temp1[key]-=1;
				}
				difference[k--] = (10 + (temp1[i]-'0')) - (temp2[j] -'0') + '0';
			}
		}
		if(i!=-1)
		{
			for(int k=i; k>=0; k--)
				difference[k] = temp1[k];
		}
		free(temp1);
		free(temp2);
		trim(difference);
		return difference;
	}
	else 
	{
		int i, j;
		for(i=n-1, j=m-1; i>=0 && j>=0; --i, --j)
		{
			if(temp2[i] >= temp1[j])
				difference[k--] = temp2[i] - temp1[j] + '0';
			else
			{
				if(temp2[i-1]!='0') 
					temp2[i-1]-=1;
				else
				{
					int key=i-1;
					while(temp2[key]=='0')
					{
						temp2[key]='9';
						key--;
					}
					temp2[key]-=1;
				}
				difference[k--] = (10 + (temp2[i]-'0')) - (temp1[j] -'0') + '0';
			}
		}
		if(i!=-1)
		{
			for(int k=i; k>=0; k--)
				difference[k] = temp2[k];
		}
		free(temp1);
		free(temp2);
		trim(difference);
		return difference;
	}
}

char* intal_multiply(const char* intal1, const char* intal2)
{

	if(!strcmp(intal1,"0") || !strcmp(intal2,"0"))
	{
		char *res = (char*)calloc(2, sizeof(char));
		strcpy(res, "0");
		return res;
	}
	char* temp = (char*)calloc(1001, sizeof(char));
	int m = strlen(intal1), n = strlen(intal2);
	char* result = (char*)calloc(m+n+1, sizeof(char));

	for(int i=0; i<1000; ++i)
		temp[i]='0';
	temp[1000]='\0';
	strcpy(result, "0");

	int k = m+n-1, carry=0;
	for(int i=n-1; i>=0; --i)
	{
		k = m;
		for(int j=m-1; j>=0; --j)
		{
			int val = (intal1[j]-'0')*(intal2[i]-'0')+carry;
			temp[k+i] = (char)(val%10 + '0');
			k--;
			carry = val/10;
		}
		temp[i] = carry+'0';
		temp[m+n] = '\0';
		char* sum = intal_add(result, temp);
		strcpy(result, sum);
		free(sum);

		for(int i=0; i<1001; ++i)
			temp[i]='0';
		carry=0;
	}

	free(temp);
    trim(result);
	return result;
}

char* intal_pow(const char* intal1, unsigned int n)
{
    char* result = (char*)calloc(1001, sizeof(char));
	char* val = (char*)calloc(1001, sizeof(char));
	result[1000] = '\0';
	val[1000] = '\0';

	char* temp;
	strcpy(val, intal1);
	strcpy(result, "1");

    while (n > 0) { 
        if (n & 1)  
        {
			temp = intal_multiply(result, val);
			strcpy(result, temp);
			free(temp);
		}
        n = n >> 1;
		if(n!=0)
		{
			temp = intal_multiply(val, val); 
			strcpy(val, temp);
			free(temp);
		}
    } 
	free(val);
	return result;
} 

char* intal_fibonacci(unsigned int n)
{
	char* a = (char*)calloc(1001, sizeof(char));
	char* b = (char*)calloc(1001, sizeof(char));
	strcpy(a, "0");
	strcpy(b, "1");
	if(n==0)
	{
		free(b);
		return a;
	}
	if(n==1)
	{
		free(a);
		return b;
	}
	char* c;
	char* final = (char*)calloc(1001, sizeof(char));
	for(int i=2; i<=n; i++)
	{
		c = intal_add(a, b);
		strcpy(a, b);
		strcpy(b, c);
		free(c);
	}
	strcpy(final, b);	 
	free(a);
	free(b);
	return final;
}

char* intal_factorial(unsigned int n)
{
	char* result = (char*)calloc(1001, sizeof(char));
	char* counter = (char*)calloc(1001, sizeof(char));
	strcpy(result, "1");
	strcpy(counter, "1");
	char* temp;
	char* temp1;

	if(n==1 || n==0)
		return result;

	for(int i=0; i<n-1; i++)
	{
		temp = intal_add(counter, "1");
		strcpy(counter, temp);
		temp1 = intal_multiply(result, counter);
		strcpy(result, temp1);
		free(temp1);
		free(temp);
	}
	free(counter);
	return result;		
}

char* intal_bincoeff(unsigned int n, unsigned int k)
{ 
	if(k > n-k)
		k = n-k; 
	char** arr = (char**)malloc(sizeof(char*)*(k+1));

	for(int i=0; i < k+1; i++)
	{
		arr[i] = (char*)calloc(1001, sizeof(char));
		strcpy(arr[i], "0");
	}
	strcpy(arr[0], "1"); 

	char* temp;
	for (int i=1; i <= n; i++) 
	{
		int j = i<k ?i:k;
		for (;j > 0; j--) 
		{
			temp = intal_add(arr[j], arr[j-1]); 
			strcpy(arr[j], temp);
			free(temp);
		}
	}

	char* final = (char*)calloc(1001, sizeof(char));
	strcpy(final, arr[k]);

	for(int i=0; i<k+1;i++)
		free(arr[i]);
	free(arr);
	return final;
} 

int intal_max(char **arr, int n)
{
	char* max = (char*)calloc(1001, sizeof(char));
	strcpy(max, arr[0]);
	int pos = 0;
	for(int i=1; i<n; i++)
	{
		int cmp = intal_compare(max, arr[i]);
		if(cmp==-1)
		{
			strcpy(max, arr[i]);
			pos= i;
		}
	}
	free(max);
	return pos;
}

int intal_min(char **arr, int n)
{
	char* min = (char*)calloc(1001, sizeof(char));
	strcpy(min, arr[0]);
	int pos = 0;
	for(int i=1; i<n; i++)
	{
		int cmp = intal_compare(min, arr[i]);
		if(cmp==1)
		{
			strcpy(min, arr[i]);
			pos = i;
		}
	}
	free(min);
	return pos;
}

int intal_search(char **arr, int n, const char* key)
{
	for(int i=0; i<n; i++)
	{
		if(!strcmp(arr[i], key))
			return i;
	}
	return -1;
}

int intal_binsearch(char **arr, int n, const char* key)
{
	int start = 0, end = n-1, mid, pos = -1;
	while(start <= end)
	{	
		mid = (start+end)/2;
		if(intal_compare(key, arr[mid]) == 0)
		{
			pos = mid;
			end = mid-1;
		}
		else if(intal_compare(key, arr[mid]) == 1)
			start = mid+1;
		else
			end = mid-1;
	}
	return pos;
}

void intal_sort(char **arr, int n)
{
	char* temp;
	for(int i=n/2-1; i>=0; i--)
		heapify(arr, n, i);
	for(int i=n-1; i>0; i--)
	{
		temp = arr[0];
		arr[0] = arr[i]; 
		arr[i] = temp;
		heapify(arr, i, 0);
	}
}

char* intal_mod(const char* intal1, const char* intal2)
{
	char* temp1 = (char*)calloc(1001, sizeof(char));
	char* temp2 = (char*)calloc(1001, sizeof(char));
	strcpy(temp1,intal1);		
	strcpy(temp2,intal2);

	char* mod = (char*)calloc(1001, sizeof(char));	
	mod[0] = '0';		

	if(!strcmp(intal1,"0"))
	{
		free(temp1);
		free(temp2);
	    return mod;
	}

	char *p1,*p2;
	for(int i = 0;i<strlen(intal1);i++)	
	{	
		char* res1 = (char*)calloc(1001, sizeof(char));
		char* res2 = (char*)calloc(1001, sizeof(char));
		char* t = (char*)calloc(2, sizeof(char));

		t[0] = intal1[i];		
		strcpy(res1,mod);	

		if(strcmp(res1,"0"))
			strcat(res1,"0");	

		p1 = intal_add(res1,t);
		strcpy(res2,p1);
		p2 = repeat_subtract(res2,temp2);
		strcpy(mod,p2);

		free(p1);
		free(p2);
		free(res1);
		free(res2);
		free(t);

		if(!strcmp(mod,temp2))
			strcpy(mod,"0");
	}
	free(temp1);	
	free(temp2);	
	return mod;	
}

char* intal_gcd(const char* intal1, const char* intal2)
{	
    char* temp1 = (char*)calloc(1001, sizeof(char));
	char* temp2 = (char*)calloc(1001, sizeof(char));

    strcpy(temp1,intal1);
	strcpy(temp2,intal2);	
	
	char* temp;
	while(strcmp(temp2,"0"))
	{
		temp = intal_mod(temp1,temp2);	
		strcpy(temp1,temp2);		
		strcpy(temp2,temp);		
		free(temp);	
	}
	free(temp2);
	return temp1;
}

