#include <stdio.h>
#include <ctype.h>
#include <string.h>


int anagram_checker(char s1[], char s2[])
{
	int hash_1[256] = {0}, hash_2[256] = {0};

	int len_1 = strlen(s1), len_2 = strlen(s2);
	// Trivial Case
	if(len_1 != len_2)
		return 0;

	// Update hash table
	int i;
	for(;i<len_1;++i)
	{
		++hash_1[(int)s1[i]];
		++hash_2[(int)s2[i]];
	}

	i=0;
	for(;i<256;++i)
		if(hash_1[i] != hash_2[i])
			return 0;
	return 1;
}

int main() {
	char string1[100], string2[100];

	scanf("%s",string1);
	scanf("%s",string2);

	if (anagram_checker(string1, string2) == 1)
		printf("Given Strings are anagrams\n");
	else
		printf("Given Strings are not anagrams\n");

	return 0;
}
