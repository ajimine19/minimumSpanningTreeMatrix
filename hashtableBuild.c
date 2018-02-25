#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct hasher//created a struct to save things in a linklist
{
    int hashTable[1000];//created a table to hold everything
    int coll;//coll counter
    
};

struct hasher * insertFunc(int mod, int value, struct hasher * list);

int main( int argc, char * argv[] )
{
    if(argc<3)
    {
        printf("Ah you messed up yeah. But its all good I make plenty mistakes too");
    }
    //made all the variable to be set
    int col =0;
    int mod= atoi( argv[1] );//saved the value in command arguement to mod
    int table[argc-2];//then we took in the total arguements for the hash table
    int i =0;
    
    for(i = 0; i < argc-2; i++)
    {
        
        table[i]=atoi(argv[i+2]);//puts all the values in the table
        
        //printf("%d\n",table[i]);
    }
    
//insertHASH
    struct hasher * list = malloc(sizeof(struct hasher));//malloc data
    int d;
    for(d=0;d<(argc-2);d++)
    {
        list->hashTable[d] = -100;//makes all the values to be -100 to start off
        
    }
    list->coll =0;
    int k;
    for(k=0; k<argc-2 ;k++)
    {
        list = insertFunc(mod,table[k],list);//next save list to the insert function
        
    }
    printf("%d Collisions\n",list->coll);
    printf("{ ");
    int j;
    for(j=0;j<mod;j++)
    {
        printf("%d:%d",j,list->hashTable[j]);//prints the values out how it is given in the example
        if(j != mod-1)
        {
            printf(", ");
        }
    }
    printf(" }\n");
    
}

struct hasher * insertFunc(int mod, int value, struct hasher * list)//the insert function goes through the hashing with mod. also we count the number of collisions
{
    //printf("%d \n",list->coll);
    int index = value%mod;
    if(list->hashTable[index] != -100)//as long as it is still -100 then it doesnt  mean there was a collision
    {
        list->coll = list->coll + 1;
    }
    
    list->hashTable[index] = value;//the value is saved
    
    return list;
    
    
}

//inputValue modhashTableSize
