/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

unsigned int hash(int userID){
    int prime=0; //we have a prime value in which we store an integer of the primes_g
    
    for(int i=0;i<hashtable_size;i++){  
        //we want to find the prime that each time is bigger than both maxID and maxUsers 
        if(primes_g[i]>max_id && primes_g[i]>max_users)
            prime=primes_g[i] * userID;
    }
    unsigned int hash_value=((userID*max_id+max_users))%hashtable_size; //than we have the type of universal hashing function in which we return it 
    return hash_value; //that is the final hash function
}

void init(){ //we initialize the hashtable in main using this function
    user_hashtable_p =malloc(sizeof(user_t)*hashtable_size);
    for(int i=0;i<hashtable_size;i++)
        user_hashtable_p[i]=NULL;
        
}
    


/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID){
	int registered_=0; //this counts the number of registered users
    user_t* NewNode=(user_t*)malloc(sizeof(user_t)); //initialization of the user
    NewNode->userID=userID;
    NewNode->next=NULL;
    NewNode->history=NULL;

    int index=hash(userID); //we use the hash function to store it in the hash table
    
        if(user_hashtable_p[index]==NULL){ //if the head is empty that means the list is empty 
            user_hashtable_p[index]=NewNode;   //make the newuser the newHead of this chain-list
        }
        else{ //if not 
             if(user_hashtable_p[index]->next==NULL)//check if the next node from the head is empty
                user_hashtable_p[index]->next=NewNode; //and make it next
             else{ //else search the node which is empty so as to put the user accordingly
                user_t* current=user_hashtable_p[index];
                while(current->next!=NULL)
                {
                    current=current->next;
                }
                current->next=NewNode;
            }
        }
    registered_++; //we increment the registered
    if(registered_==primes_g[index]){ //if the registered users are the same number as the particular number in the prime_g array we chose 
        user_hashtable_p=realloc(user_hashtable_p,primes_g[index++]); //extend the memory by adding just the next integer in the primes_g
        hashtable_size = primes_g[index];
    }
    printf("R<%u>\n",userID);
    printf(" Chain <%d> of Users: \n",index);
    user_t* current=user_hashtable_p[index];
    while(current!=NULL){
        printf("\t <%d>\n",current->userID);
        current=current->next;
    }
    printf("\nDONE\n");

    return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID){
	 int index=hash(userID); //we find the user in the hash table

        if(user_hashtable_p[index]!=NULL && user_hashtable_p[index]->userID!=userID)
        //if the head is not NULL and the head of the list is the user we want to unregister
        {  
            user_t *tmp=user_hashtable_p[index]; //store a tmp value to the head
            user_t *prev=NULL;
            
            while(tmp!=NULL && tmp->userID!=userID) //search for the user in the particular chain
            {
               
                prev=tmp;
                tmp=tmp->next;
            }

            if(tmp==NULL)
            {
                printf("User not found\n");
                return 0;
            }else{ //if prev empty that means that it was the head
                if(prev==NULL)
                    user_hashtable_p[index]=tmp->next; //remove the head and update to point to the node 
                else
                    prev->next=tmp->next;  //update the pointers
                
                free(tmp);
                free(prev);

            }  
        }

    printf("U <%d>\n",userID);
    printf(" Chain <%d> of  Users:\n",index);
    user_t *current=user_hashtable_p[index];

    while(current!=NULL)
    {
        printf(" <%d>\n",current->userID);
    }

    return 1;
 }
 
void InOrderTraversalNewMovie(new_movie_t *root){
    if(root!=NULL)
    {
        InOrderTraversalNewMovie(root->lc);
        printf("  %d",root->movieID);
        InOrderTraversalNewMovie(root->rc);
    }
    
}

new_movie_t* insertToTree(new_movie_t *root,new_movie_t *node)
{
    if(root==NULL){
        //if the movie tree is empty make the node the head
        root=node;
        return node;
    }

    if(root->movieID>node->movieID){// if the node we want to insert is less than the root node we go left else we go right
        root->lc=insertToTree(root->lc,node);
    }
    else if(root->movieID<node->movieID){

        root->rc=insertToTree(root->rc,node);
    }

    return root;
}



 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

 int add_new_movie(int movieID, int category, int year){
    //We create a New node for the newmovie
	 new_movie_t *new_add=(new_movie_t*)malloc(sizeof(new_movie_t));
    if(new_add==NULL){
        return 0;
    }

    new_add->movieID=movieID;
    new_add->category=category;
    new_add->year=year;
    new_add->watchedCounter=0;
    new_add->sumScore=0;
    new_add->lc=NULL;
    new_add->rc=NULL;

    //we than insert according to the BST rules
    new_releases=insertToTree(new_releases,new_add);
    printf("A <%d> <%d> <%d> \n",movieID,category,year);
    printf("New releases Tree:\n");
    InOrderTraversalNewMovie(new_releases);
    printf("\nDONE\n");
    printf("\n\n");

    return 1;
 }
 
// copies the new movie into the given node but without maintaing the children
movie_t* _copyData(new_movie_t* new_movie_node, movie_t* movie_node){
    if(new_movie_node==NULL) return NULL;

    // copy data
    movie_node->movieID = new_movie_node->movieID;
    movie_node->sumScore = new_movie_node->sumScore;
    movie_node->year = new_movie_node->year;
    movie_node->watchedCounter = new_movie_node->watchedCounter;

    // remove pointer to kids
    movie_node->lc=NULL;
    movie_node->rc=NULL;

    return movie_node;
}


// append node into the tree of root
movie_t *Insert(movie_t* root,movie_t* node){
    if(root==NULL){ // if root is null
        return node; // and return it
    } 

    //we go left if the node value is less than the root value or right otherwise
    if(root->movieID > node->movieID){     
        root->lc=Insert(root->lc,node);
    }
    else if(root->movieID < node->movieID){
        root->rc=Insert(root->rc,node);
    }
    
    return root;
}

void _init(){ 
    //we initialize the categoryArray
    for(int i=0;i<SIZE;i++){
        categoryArray[i]=(movieCategory_t*)malloc(sizeof(movieCategory_t));
        categoryArray[i]->movie=NULL;
        categoryArray[i]->sentinel=NULL;
    }
}

void dm(new_movie_t* root,int pos){
    if(root==NULL) return;

    movie_t *node=(movie_t*)malloc(sizeof(movie_t));
    //if this node has the category we have taken as a parameter 
    if(root->category==pos)
    {
            //copy root into a new node
            node=_copyData(root , node); //copy to the movie node the data from the new_movie_t node

            // append node into category tree
            categoryArray[pos]->movie = Insert(categoryArray[pos]->movie,node); //and insert it in the category tree properly
    }  
    dm(root->lc,pos); //recursion 
    dm(root->rc,pos); //to go through the whole new realeases tree
    
}

void InOrderTraversal(movie_t *root){
    if(root==NULL) return;

    InOrderTraversal(root->lc);
    printf("%d ",root->movieID);
    InOrderTraversal(root->rc);
}


 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 int distribute_movies(void){
	_init();
    for(int i=0;i<6;i++){ //for each category we search the new realeases tree
        dm(new_releases,i);
    }
    
    free(new_releases); // we delete it

    printf("D\n");
    printf("Movie Category Array:\n");
    for(int i=0;i<6;i++){
        printf("category_%d ",i);
        InOrderTraversal(categoryArray[i]->movie);
        printf("\n");
    }
    return 1;
 }
 
 movie_t* search(movie_t* root, int movieID) {
    if (root == NULL) {
        return NULL;
    } //if the movie tree is empty

    if (root->movieID == movieID) { //if we have found the movieID we wanted 
        return root; //we return this node else we go left or right depending on the movieID value 
    } else if (root->movieID > movieID) {
        return search(root->lc, movieID);
    } else {
        return search(root->rc, movieID);
    }
 }

 void _InOrderTraversal(userMovie_t *root){
    if(root==NULL) return;
    _InOrderTraversal(root->lc);
    if(root->rc==NULL && root->rc==NULL){   
        printf("%d ",root->movieID);
    }
     _InOrderTraversal(root->rc);
}


userMovie_t *InsertMovie(userMovie_t *root,int movieID,int score,int category){
    //we insert a node to a leaf oriented tree
    userMovie_t* ptr=NULL;
    userMovie_t* NewNode=NULL;
    userMovie_t* prevptr=NULL;
    userMovie_t* filling;

    NewNode=(userMovie_t*)malloc(sizeof(userMovie_t));

    NewNode->movieID=movieID;
    NewNode->score=score;
    NewNode->category=category;
    NewNode->lc=NULL;
    NewNode->rc=NULL;
    NewNode->parent=NULL;


    filling=(userMovie_t*)malloc(sizeof(userMovie_t));
    filling->movieID=movieID;
    filling->score=score;
    filling->category=category;
    filling->lc=NULL;
    filling->rc=NULL;
    filling->parent=NULL;

    //create those 2 new nodes 1 for the left leaf and for the right leaf if it will be necessary
    
    ptr=root; //if the root is empty we make the newNode the root
    if(ptr==NULL){
        ptr=NewNode;
        return NewNode;
    }

    //we loop through the leaf oriented tree if it is not empty
    while(ptr!=NULL){
        if(ptr->movieID > movieID){ //we store each time to a prev pointer the ptr value before we advance left or right
            prevptr=ptr;
            ptr=ptr->lc;
        }else if(ptr->movieID < movieID){
            prevptr=ptr;
            ptr=ptr->rc;
        }else if(ptr->movieID==movieID){ //that means we already have the node so we pass the data and we return the updated root
            ptr->score=score;
            ptr->category=category;
            return root;
        }
    }

    if(prevptr->movieID < movieID){  //if the previous pointer is less than the movieId we want to insert

        prevptr->rc = NewNode; //make the NewNode it's right child
        prevptr->lc=filling; //the filling it s left 
        filling->movieID=prevptr->movieID; //and copy the movie id for the left child since it s a leaf and only the right child existed

        NewNode->parent=prevptr; //assert the parents for the children to be the prev pointer
        filling->parent=prevptr;
        
        
    } else if(prevptr->movieID > movieID){  //otherwise
       NewNode->movieID=prevptr->movieID; //copy the node to be the right child since the left already existed and assert the parent to children
       prevptr->rc=NewNode;
       prevptr->movieID=movieID;
       prevptr->lc=filling;

       NewNode->parent=prevptr;
       filling->parent=prevptr;
    }


    return root;

}


 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID,int category, int movieID, int score){
	 movie_t *_findmovie_ =categoryArray[category]->movie;
    _findmovie_=search(_findmovie_,movieID);
    

    if(_findmovie_==NULL){
        printf("\nMovie not found\n");
        return 0;
    } 
    
    _findmovie_->watchedCounter++;
    _findmovie_->sumScore += score;

    int index=hash(userID);

    user_t* temp=user_hashtable_p[index];

    while(temp!=NULL && temp->userID!=userID){
        temp=temp->next;
    }
    

    temp->history=InsertMovie(temp->history,movieID,score,category);


    printf("\nW %d %d %d %d",userID,category,movieID,score);
    printf("\n History Tree of User %d:\n",userID);
    _InOrderTraversal(user_hashtable_p[index]->history);
    printf("\nDONE\n");

    return 1;
 }
 

 int CreateSize(movie_t* root, int score, int numMovies) {
    if (root == NULL) {
        return numMovies; //return the number of movies
    }

    if (root->watchedCounter != 0) { //if this movie has been watched at least once
        int average = root->sumScore / root->watchedCounter; //find the average 

        if (average >= score) { //look if it is higher the score which the user suggested
            numMovies++; //if yes increase the number of movies
        }
    }

    // Recurse into both left and right subtrees
    numMovies = CreateSize(root->lc, score, numMovies);
    numMovies = CreateSize(root->rc, score, numMovies);

    return numMovies;
}

void CreateArray(movie_t* root, movie_t** A,int score,int *index){
    if(root==NULL){
        return ;
    }

    if(root->watchedCounter!=0){ //same here

        int average=(root->sumScore)/(root->watchedCounter);
 
        if(average>=score){ //this time we pass the node to a movie Array 
            A[*(index)]=root; 
            (*index)++;
        }
    }  

    //Recursion
    CreateArray(root->lc,A,score,index);
    CreateArray(root->rc,A,score,index);

}

void swap(movie_t** a,movie_t** b){ //swap function to swap the two movie nodes properly
    movie_t *temp=*a;
    *a=*b;
    *b=temp;
}

void heapify(movie_t **A,int N,int i){
    int largest=i; //we put the index of the array to be the largest

    int left=2*i+1;
    //these are the steps
    int right=2*i+2;
    

    //we check if the average of the left is greater than the average of the index of the array
    if(left<N && (A[left]->sumScore/A[left]->watchedCounter) > (A[largest]->sumScore/A[largest]->watchedCounter)){
        largest=left;
    } 

    //same but for the right
    if(right<N && (A[right]->sumScore/A[right]->watchedCounter) > (A[largest]->sumScore/A[largest]->watchedCounter)){
        largest=right;
    }

    //if the largest isn't i meaning that we have found another largest 
    if(largest != i){
        swap(&A[i],&A[largest]); //we swap the index and the largest
        heapify(A,N,largest); //and we call this function it's self again
    }

}

void HeapSort(movie_t **A,int N){ //create a max heap
    for(int i=N/2-1;i>=0;i--){
        heapify(A,N,i);
    }

    for(int i=N-1;i>=0;i--){ //heapify it 
        swap(&A[0],&A[i]);

        heapify(A,i,0);
    }
}

/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, int score){
	int numMovies=0;
    int index=0;
    movie_t** A=NULL;

    for(int i=0;i<SIZE;i++){ //for each category 

        movie_t* _ptr_=categoryArray[i]->movie; //we find the total movies that have a rating equal or higher than the score
        numMovies=CreateSize(_ptr_,score,numMovies); 
    }


    A=(movie_t**)malloc(sizeof(movie_t*) * numMovies);

    for(int i=0;i<SIZE;i++){ //we now for each category create a whole array
        movie_t* _ptr_=categoryArray[i]->movie;
        CreateArray(_ptr_,A,score,&index);
    }


    
    HeapSort(A,numMovies); //we heapSort it

    printf("\nF <%d> <%d>",userID,score);
    for(int i=0;i<numMovies;i++){
        int average=(A[i]->sumScore)/(A[i]->watchedCounter);
        printf("\n %d %d ",A[i]->movieID,average);
    }
    printf("\nDONE\n");
    
    
    return 1;
 }

userMovie_t *FindMostLeftLeaf(userMovie_t *root){
    if(root==NULL) 
        return NULL; 
    
    //we go to the left until we find the most left leaf
    while(root->lc!=NULL){
        root=root->lc;
    }
        
    return root;
}


userMovie_t* FindNextLeaf(userMovie_t* root){
    userMovie_t* tmp;
   
    if(root==NULL) return NULL;
     
    //we put the parent of the node to a tmp 
    tmp=root->parent;
    while(tmp!=NULL){ //we iterate and if we find a node with equal or higher than the tmp we stop the iteration
        if(tmp->movieID >= root->movieID){
            break;
        }
        tmp=tmp->parent;
    }
    
    //if tmp is null that means there are no other leafs 
    if(tmp==NULL){
        return NULL;
    }
   

    //we put now the right child 
    //and we search if this right child is a leaf 
    //if it is we return it 
    tmp=tmp->rc;
    while(tmp!=NULL){
        if(tmp->lc ==NULL && tmp->rc==NULL)
            return tmp;
        tmp=tmp->lc;
    }

    return NULL;
}



/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){
	int index=hash(userID); //find the position in the hash table
    //meaning the root in which the user is lisetd

    user_t * tmp=user_hashtable_p[index]; //we assert the root of the list to a tmp pointer

    while(tmp!=NULL && tmp->userID!=userID){ //while the tmp is not NULL and while we havent found the user we continue
        tmp=tmp->next;
    }
    
    if(tmp==NULL){ //if we stop because we reached the end of the list that means the user is not found
        printf("User not found!\n");
    }

    
    userMovie_t *FindLeaf; 

    int ScoreSum=0,counter=0; //we create those 2 helper-variables


    FindLeaf=FindMostLeftLeaf(tmp->history); //we find the most left leaf in the history tree of the user

    if(FindLeaf==NULL){ // we check if the history is empty
        printf("This user has no history\n");
        return 0;
    }
   
    counter++; //we increment the counter and add the score since we found already an existing node
    ScoreSum+=FindLeaf->score;
    
    
    while(FindLeaf != NULL){ //while no nodes are left 
        FindLeaf=FindNextLeaf(FindLeaf);  //we want to find the next leaf
        if(FindLeaf!=NULL){ 
            counter++;
            ScoreSum+=FindLeaf->score;
        }
    }

    int average;

    if(counter!=0){
        average=(ScoreSum/counter);
    }
    printf("Q %d %d\n",userID,average);
    printf("DONE\n");
	return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){
    //we take a tmp pointer that pointes to the root of the  certain category tree
	 movie_t *tmp=categoryArray[category]->movie;
     tmp=search(tmp,movieID); //we call the function in which through recursion checks for the movie in this category

    if(tmp==NULL){ //that means it didn't find the movie
        printf("Movie not found\n");
        return 0;
    }

    printf("I %d %d %d",movieID,category,tmp->year);
    printf("\nDONE");
    
    return 1;
 }
 
 void __InOrderTraversal(userMovie_t *root){
    if(root==NULL) return;
    __InOrderTraversal(root->lc);
    if(root->rc==NULL && root->rc==NULL){   
        printf(" %d %d",root->movieID,root->score);
    }
    __InOrderTraversal(root->rc);
 }

 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	 printf("\nM\n");
    printf("Movie Category Array:\n ");
    for(int i=0;i < SIZE;i++){
        printf(" category_%d ",i);
        InOrderTraversal(categoryArray[i]->movie);
        printf("\n");
    }
    printf("DONE\n"); 
	return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){
	 printf("\nP\n");
    for(int i=0;i <hashtable_size;i++){ //for each element in the hashtable
        if(user_hashtable_p[i]!=NULL){ //if it is not null we than go through the list of the users and for each one we print it's history
            printf("\t\nChain %d of Users:\n",i);
            user_t* current=user_hashtable_p[i];
            while(current!=NULL){
                printf("\t\t%d\n",current->userID);
                printf("\t\tHistory Tree:");
                userMovie_t* currentMovie=current->history;
                __InOrderTraversal(currentMovie);
                current=current->next;
            }
        }
    }
    printf("\nDONE\n");
	return 1;
}
 
