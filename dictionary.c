// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 15061;//USING PRIME NUM: 15061
int wordsAdded = 0;
// Hash table
node *table[N];


/**
--allocate memory for a new node
node *n = malloc(sizeof(node));
strcpy(n->word, "Hello");
n->next = NULL;
**/


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //hash word as "hashed"
    //access linked list at index [hashed]
    //traverse linked list, looking for that word. use strcasecmp()
    //return true if found, false if not

    char word_stzd[strlen(word)];
    strcpy(word_stzd, word);

    //make sure the word is in lowercase
    for (int i = 0; word_stzd[i] != '\0'; i++)
    {
        word_stzd[i] = tolower(word_stzd[i]);
    }

    int index = hash(word_stzd);
    if (table[index] != NULL) //if there is a linked list at the index
    {
        for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
        {
            if (strcasecmp(cursor->word, word_stzd) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *s)
{
    //input: word with alphanum chars and possibly apostraphes
    //output: numerical index between 0 and N-1, inclusive (where N is size of hash table)

    //HASH FUNCTION CREDIT TO: Gabriel Staples
    //https://stackoverflow.com/questions/7666509/hash-function-for-string
    unsigned int hashval;

    for (hashval = 0; *s != '\0'; s++)
    {
        if (isalpha(*s))
        {
            hashval = *s + 31 * hashval;
        }
    }
    return hashval % N;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //load dictionary into data-structure (hash-table)
    /**
    -open dictionary file
    -Read strings from file one at a time
    -Create new node for each word
    -Hash word to obtain hash value (apple = "a") -- implemented in hash()
    -insert node into hash table at that location
    **/

    for (int i = 0; i < N; i++) //for size of hash table
    {
        table[i] = NULL;//set hash tables elements to null
    }

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));//create new node

        if (n == NULL) //returns null if not enough memory
        {
            return false;
        }

        strcpy(n->word, word);//copy word into node's word property
        n->next = NULL;//init its next ptr to null

        int index = hash(word);

        if (table[index] != NULL) //if there is already a node at the index
        {
            n->next = table[index];//set new node's ptr to the first node
        }

        table[index] = n;//set the pointer at the index to be the new node
        //node insertion complete

        wordsAdded++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //printf("%i\n",wordsAdded);
    return wordsAdded;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //free the memory from the hash table

    //iterate through table
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *cursor = table[i];

            while (cursor != NULL)
            {
                node *tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }

        }

    }

    return true;
}
