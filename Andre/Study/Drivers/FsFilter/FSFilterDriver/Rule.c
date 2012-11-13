#include "FsFilter.h"

/*********************** Structures *****************************/
// typedef struct _RULE_ENTRY 
// {
// 	WCHAR		name [FILE_NAME_LENGTH];
// 	ULONG		length;
// 	struct _RULE_ENTRY * pNext;
// } RULE_ENTRY, *PRULE_ENTRY;
// 
PRULE_ENTRY g_pFirst = NULL;
/********************** Processing rule *****************************/
BOOLEAN InitRule (PWCHAR pBuffer)
{
	PRULE_ENTRY currEntry = NULL;
	PRULE_ENTRY prevEntry = NULL;
	USHORT numberOfWords = 0;
	USHORT numberOfSymbols = 0;
	ULONG ruleIterator = 0;
	ULONG i = 0;

	numberOfWords = (ULONG) pBuffer [ruleIterator++];

	for (; numberOfWords; --numberOfWords, ruleIterator += numberOfSymbols)
	{
		numberOfSymbols = (USHORT) pBuffer [ruleIterator++];
		currEntry = (RULE_ENTRY *) ExAllocatePool (NonPagedPool, sizeof(RULE_ENTRY));
		if (currEntry) // Memory allocated
		{	
			currEntry->length = numberOfSymbols;
			memcpy (currEntry->name, pBuffer + ruleIterator, numberOfSymbols * sizeof(WCHAR));
			for ( ; i < currEntry->length; ++i )
			{
				currEntry->name[i] = RtlUpcaseUnicodeChar( currEntry->name[i]);
			}
			if (prevEntry)
				prevEntry->pNext = currEntry;
			else
				g_pFirst = currEntry;
			prevEntry = currEntry;
		}
		else // Memory don't allocated
		{
			if (prevEntry) // Not first word
			{	
				prevEntry->pNext = NULL;
				RemoveRule();			
			}
			else // First word
			{
				g_pFirst = NULL;				
			}
			return FALSE;
		} // END: Memory don't allocated		
	} // END for (words)
	currEntry->pNext = NULL; // Last entry
	return TRUE;
}

void RemoveRule ()
{
	PRULE_ENTRY currEntry = NULL;
	PRULE_ENTRY nextEntry = NULL;

	currEntry = g_pFirst;
	g_pFirst = NULL;
	while (currEntry)
	{
		nextEntry = currEntry->pNext;
		ExFreePool (currEntry);
		currEntry = nextEntry;
	}
}

BOOLEAN CheckEqual (PWCHAR fileName)
{
	PRULE_ENTRY hidingFile = g_pFirst;
	size_t nameLength = wcslen(fileName);
	while (hidingFile)
	{
		if (nameLength == hidingFile->length && !memcmp(fileName, hidingFile->name, hidingFile->length * sizeof(WCHAR)))
			return TRUE;

		hidingFile = hidingFile->pNext;
	}
	return FALSE;
}

ULONG RuleSerializerLength ()
{
	PRULE_ENTRY currEntry = g_pFirst;
	ULONG length = 2; // one symbol for number of words, and one for '\0'
	while (currEntry)
	{
		length += 1 + currEntry->length; //one symbol for number of symbols
		currEntry = currEntry->pNext;
	}
	return length;
}

BOOLEAN AssembleRule (PWCHAR outBuffer, ULONG outBufLength)
{
 	PRULE_ENTRY currEntry = g_pFirst;
// 	PRULE_ENTRY prevEntry = NULL;
	USHORT numberOfWords = 0;
//	ULONG numberOfSymbols = 0;
 	ULONG ruleIterator = 0;
// 	ULONG i = 0;
	//outBuffer [ruleIterator++] = '0';

	if (/* outBufLength < g_ruleLength || */ !currEntry)
		return FALSE;
	
	while (currEntry /* && ruleIterator <= g_ruleLength */)
	{
		//++numberOfWords;
		//outBuffer [ruleIterator++] = currEntry->length;
		memcpy (outBuffer + ruleIterator, currEntry->name, currEntry->length * sizeof(WCHAR));
		ruleIterator += currEntry->length ;
		outBuffer[ruleIterator++] = ';';
	//	if (ruleIterator > g_ruleLength)
		currEntry = currEntry->pNext;
	}
	outBuffer [ruleIterator] = '\0';

	return TRUE;
}