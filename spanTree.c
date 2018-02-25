#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_EDGES 4 //we defined the edges 4 in the variable NUM_EDGES
//created the struct

//The following code produces a minimum spanning tree matrix that takes in an input file and print the new matrix in an output. We created a batch file that takes in every name and value and saves it in a series of nodes with a hash table. After, prim function is called to sort out the matrix and come out with mst. Inside of prim there is a minimum function that replaces the value if it the min is less than the value. printing uses nest for loops to through all the variables to switch from 0 to a edgeweight value if neccessary. 

////////////////////////////////////////////////////////////////////////////////////
struct node
{
    char name; // A, B, C
    int value; // weights of the current node
    int stage; // 1 white, 2 gray, 3 black
    struct node * treeEdge; //node from which this node is part of the tree
    struct node * edges[NUM_EDGES]; // possible edges to other nodes... start at NULL
    int edgeWeights[NUM_EDGES]; // weights for each possible edge
};
////////////////////////////////////////////////////////////////////////////////////

struct node * newNode( char initName );
int primLogParser(char * inputPath, char * output);
void printGraphToTerminal( struct node * graph[], int size );
int hashIndex( char a );
struct node * minFunc(struct node * table[]);
void primerFunc( struct node * table[] , int target, char * output );
void printTree( struct node * graph[], int size, char * output );

////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char * args[] )
{
    return primLogParser( args[1], args[2] );//takes in the command line arguement
    
}

////////////////////////////////////////////////////////////////////////////////////

int primLogParser(char * inputPath, char * output)//takes in the input and the output
{
    /////////Variables//////////
    struct node * listOfNodes[26] ={NULL} ;
    char headerNodes[10] = {'!'};
    int headerIndex = 0, iter;
    char buff[100];
    char * token;
    
    FILE * fpi = fopen( inputPath, "r" );//opens the file
    
    if( fpi == NULL )
    {
        printf( "Input Path not found, %s", inputPath );
        return -1;
    }
    
    fscanf( fpi, "%s", buff );//scans the first line
    
    token = strtok( buff, "," );
    
    while( token != NULL )//collects the info of the first line (nodes,g,a,b,c,h)
    {
        char nodeName = token[0];
        
        token = strtok( NULL, "," );//starts at g
        //printf( "%c\n\n", token[0] );
        
        if( token != NULL )
        {
            headerNodes[ headerIndex ] = token[0];//saves the token into the headerNodes array
            listOfNodes[ hashIndex( token[0] ) ] = newNode( token[0] );//inside the linklist their is a hash table and now a newNode made for g,a,b,c,h is created
            headerIndex++;//iterates until NULL is reached
        }
    }
    
    while( fscanf( fpi, "%s", buff ) != EOF )//takes in the next line
    {
        token = strtok( buff, "," );
        char nodeRowName = token[0];//takes in the first Node Name of the first row
        headerIndex = 0;//we start the header Index at 0
        while( token != NULL )
        {
            token = strtok( NULL, "," );//takes in the char after the ","
            if( token != NULL )
            {
                int weight = atoi( token );//saves the int to weight
                if( weight > 0 )
                {
                    struct node * pointFrom = listOfNodes[ hashIndex(nodeRowName) ];
                    struct node * pointTo =  listOfNodes[ hashIndex( headerNodes[ headerIndex ] ) ];
                    for( iter=0; iter<NUM_EDGES; iter++ )
                    {
                        if( pointFrom->edges[iter] == NULL )//checks if the edges is empty
                        {
                            pointFrom->edges[iter] = pointTo; //then fills the point from edge with int
                            pointFrom->edgeWeights[iter] = weight; //then fills the weight of the tree
                            break;
                        }
                    }
                }
                headerIndex++;
            }
        }
    }
    primerFunc( listOfNodes, hashIndex( headerNodes[ 0] ), output);//call the primerFunction
    
    fclose( fpi );
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////

void printGraphToTerminal( struct node * graph[], int size )//print terminal to see original matrix
{
    printf("\n");
    int iterGraph,iterEdges;
    for( iterGraph=0; iterGraph<size; iterGraph++ )
    {
        if( graph[iterGraph] != NULL)
        {
            for(iterEdges=0; iterEdges<NUM_EDGES; iterEdges++)
            {
                if( graph[iterGraph]->edges[iterEdges] != NULL )
                {
                    printf("%c->%c:%d   ",graph[iterGraph]->name,graph[iterGraph]->edges[iterEdges]->name,graph[iterGraph]->edgeWeights[iterEdges]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////

struct node * newNode( char initName )//created a new node for the list of values
{
    /////////Variables//////////
    struct node * temp = malloc( sizeof( struct node ) );
    
    temp->name = initName;//set the names to the node
    temp->value = -1;//set -1 to all the values
    temp->stage = 0;//Set the stages to 0 for the first stage of not being touch
    
    temp->treeEdge = NULL;//all the edges are null
    
    int i;
    for(i=0; i<NUM_EDGES; i++)//goes through all the adjacent edges and makes them 0
    {
        temp->edges[i] = NULL;
        temp->edgeWeights[i] = -1;
    }
    
    return temp;
}

////////////////////////////////////////////////////////////////////////////////////

//only all lower case or upper alpha letters
int hashIndex( char a )//created a hash function to establish a hash table
{
    return a%26;//hash table to take in the 26 alaphabets
}

////////////////////////////////////////////////////////////////////////////////////

void primerFunc( struct node * table[] , int target,char * output)//prime function that
{
    /////////Variables//////////
    struct node * u;//created u nodes to hold the minfunction of table
    struct node * v;
    struct node * r = table[target]; //r to hold table entirely
    r->value=0;//set the value to 0
    
    while( (u=minFunc(table))!=NULL )//as long as the minimal is some number
    {
        int i;
        for(i=0; i<NUM_EDGES ;i++)
        {
            v = u->edges[i];//first we have v point to u edges
            
            if(v!=NULL)//so as long as they are not parent edges
            {
                //printf("|v=%c| u=%c v-value=%d, v-Stage=%d", v->name,u->name,v->value,v->stage);
                if(v->stage != 3 && (u->edgeWeights[i] < v->value || v->value== -1))//we check if the stage is not 3 or complete and the edgeweaights are less than the value, if so it continues, and makes sure nothing is -1 so we dont compare everytingt to -1
                {
                    //printf(" yes");
                    v->treeEdge = u;//we make the v tree edge u
                    v->value = u->edgeWeights[i];//the value is now switched
                    v->stage = 2;// and changes to 2 since it got hit
                }
                //printf("\n");
            }
        }
        
        u->stage = 3;//finishes to stage 3 to show it all got hit
        //printf("u=%c\n",u->name);
    }
    
    printf("\n\tYour MATRIX has been printed in: %s\n\n", output);
    
    
    printTree(table,26, output);//prints the final matrix in the putput
    
    
    
}

////////////////////////////////////////////////////////////////////////////////////

struct node * minFunc(struct node * table[])//function used to switch the minimal values and allow the treeedges to come from a certain edge
{
    /////////Variables//////////
    int i;
    struct node * minPoint = NULL;
    
    int min=100000;//make the min start at a hgih number
    for(i=0;i<26;i++)//goes through a loop to  check all
    {
        if(table[i] != NULL)//if there is something inside
        {
            if(table[i]->stage != 3)//not stage 3 since that would mean it is already completed
            {
                if((min > table[i]->value) && table[i]->value !=-1)//finally checks if the value is less than the min
                {
                    min = table[i]->value;//if so the min will change and be saved to the new value
                    minPoint = table[i];//then the table will be saved in midpoint
                    
                }
            }
            
        }
    }
    return minPoint;//return the node that hols all the new minimums
}



////////////////////////////////////////////////////////////////////////////////////

void printTree( struct node * graph[], int size, char * output )
{
    /////////Variables//////////
    FILE * fpi = fopen( output, "w" );//opens the file
    
    if( fpi == NULL )
    {
        fprintf( fpi, "error opening");
    }
    
    fprintf(fpi, "Nodes");
    int t;
    for(t=0; t< size; t++)
    {
        if(graph[t]!=NULL)
        {
            fprintf(fpi,",%c",graph[t]->name);//prints the name of the matrix in order of how the input is in.
        }
    }
    fprintf(fpi, "\n");
    
    int var = 0;
    struct node * rowNode;
    struct node * colNode;
    int row,col,edg;
    for( row=0; row<size; row++ )//checks the rows
    {
        if( graph[row] != NULL)
        {
            rowNode= graph[row];
            fprintf(fpi,"%c",rowNode->name);
            for(col=0;col<size;col++)//checks the colls
            {
                if(graph[col] != NULL)
                {
                    colNode= graph[col];
                    var =0;
                    for(edg=0; edg<NUM_EDGES;edg++)//then we go through all the surrounding edges
                    {
                        if( rowNode-> edges[edg] != NULL )
                        {
                            if(rowNode-> edges[edg]->name == colNode-> name)//compare the names of each and if they are equal
                            {
                                var = rowNode->edgeWeights[edg];//then the var now becomes the edge weight or the changes 0 to the number of the min value that has been touched
                                
                            }
                        }
                    }
                    fprintf(fpi,",%d",var);//print var with a value if it is connected to the parent or with 0
                }
                
            }
            
            //printf("|%c|  ",graph[iterGraph]->name);
            fprintf(fpi,"\n");
        }
    }
    
    fprintf(fpi,"\n\n\n\t(╯°□°)╯\n");
    fprintf(fpi,"\tHAVE A GREAT DAY\n");
    
    fclose( fpi );//close the function
    
}
