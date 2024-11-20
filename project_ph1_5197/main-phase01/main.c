/*
 * ============================================
 * file: main.c
 * @Author John Malliotakis (jmal@csd.uoc.gr)
 * @Version 23/10/2023
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2023-2024
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "streaming_service.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

#define SIZE 6

//initialazion for the head for each list queue etc

struct user* head1=NULL; 
struct suggested_movie* head=NULL;
struct movie* head2=NULL;
struct new_movie* new_movieHead=NULL;
struct new_movie* DistributeMovies[SIZE];


void print_movies(void){
	printf("\n\n D\n");   
	printf("Categorized Movies:\n");
    for (int i = 0; i < SIZE; i++) {  //we take every movie category,size=6
        switch (i) {
            case 0: printf(" Horror: "); break;
            case 1: printf(" Sci-fi: "); break;
            case 2: printf(" Drama: "); break;
            case 3: printf(" Romance: "); break;
            case 4: printf(" Documentary: "); break;
            case 5: printf(" Comedy: "); break;
        }

        struct new_movie* current = DistributeMovies[i]; //we create a new node new_movie type in which we store
		//each time the movie category 
		//we then iterate through the certain category and we print the mid and the year
        while (current != NULL) {
            printf("<mid_%u,%u> ", current->info.mid,current->info.year);
            current = current->next;
        }
        printf("\n");
    } 
	printf("DONE\n");
}


int register_user(int uid)
{
	//we first start by assigning to user struct the head of the list of the users which (head1) was globaly intialized
	struct user* current=head1;
	
	//we loop through the list of the users until we find the sentinel
	while(current->uid != -1)
	{
		//if we find the uid we want to register equal to a one that is already in the list we return -1
		if(current->uid == uid)
		{
			printf("User already registered\n");
			return -1;
		}
		
		current=current->next;
	}

	//if we get of the while that means no user with this uid was found and the registration starts

	struct user* new_user=malloc(sizeof(struct user)); //we create a new user(struct user) and we allocate memory for it
	if(new_user == NULL)//if there is any problem with the memory allocation we return -1
	{
		printf("Couldn't allocate");
		return -1;
	}

	//in the new_user struct in the uid section we pass the uid that we had as a parameter from the function 
	new_user->uid = uid;

	//we also create the suggest movies double linked list to null 
	new_user->suggestedHead =NULL;
	new_user->suggestedTail=NULL;

	// and also the watch history
	new_user->watchHistory=NULL;

	//we assign the next node of the user to be equal to the head 
	new_user->next = head1;

	//we update the head  
	head1=new_user;

	//we also update current to be equal to the head in order to print the users
	current=head1;

	printf("\nR <%d>\n",uid);
	int counter=1;
	printf(" Users=");
	while(current->uid!=-1)
	{
		printf("<%d_%d>,",current->uid, counter);
		counter++;
		current=current->next;
	}
	printf("\nDONE\n");

	return 0;
	
}

struct suggested_movie* delete(struct suggested_movie* head){
	// this function is to delete the double linked list

	//if the head is already empty return null
	if(head==NULL)
	{
		return NULL;
	}

	//we create a new temp node that we store the head in this value
	struct suggested_movie* current=head;

	//we advance the head to the next node 
	head=head->next;

	//if the list is not empty 
	//we assign in the node before the head null

	if(head!=NULL)
	{
		head->prev=NULL;
	}

	//we free the temp node 
	free(current);

	//we return the updated head
	return head;
}

void deleteq(){
	//this to delete the queue when we use the unregister user function
	//we assign a current node to head2
	struct movie* current=head2;
	struct movie* next;

	//we iterate through the loop
	// and in this loop we delete every node
	while(current!=NULL)
	{
		next=current->next;
		free(current);
		current=next;
	}
}


void unregister_user(int uid)
{	
	//we again create a node that we store the head 
	//in the loop we delete each time one by one a node from the user
	struct suggested_movie* curr=head;
		while(curr!=NULL){
			curr=delete(curr);
			curr=curr->next;
		}

	deleteq(); //we delete the queue which the Watchistory


	if (head1 == NULL) {
        printf("No users!\n");
    }

	//we create a temp node so we can iterate through the users 
    struct user* tmp = head1;
    struct user* prev = NULL;

	//while we dont reach the sentinel and while we dont find the uid we want to unregister
    while (tmp->uid != -1 && tmp->uid != uid) {
        prev = tmp; //in the prev(which equals NULL we assert the tmp)
        tmp = tmp->next; 
    }

    if (tmp == NULL) {
        printf("User not found!\n");
    } else {
        if (prev == NULL) {
            // User to be deleted is the first node (head1)
            head1 = tmp->next;
        } else {
			//in the prev function that points to the next node we assert the tmp that points to the next node
            prev->next = tmp->next;
        }
		//we free the temp node
        free(tmp);
    }

	struct user* current=head1;

	printf("\nR <%d>\n",uid);
	int counter=1;
	printf(" Users=");
	while(current->uid!=-1)
	{
		printf("<%d_%d>,",current->uid, counter);
		counter++;
		current=current->next;
	}
	printf("\nDONE\n");

}

int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){
	//we assert 2 struct of type new movie on temporary and one to use for it to add
	//also 1 new struct of movie info
	struct new_movie* tmp;
	struct new_movie* new_movie=malloc(sizeof(struct new_movie));
	struct movie_info* info=malloc(sizeof(struct movie_info));
	


	if(info==NULL || new_movie==NULL) {
		free(info);
		free(new_movie);
		return -1;
	}

	//we create the node and we store the mid and the year 
	info->mid=mid;
	info->year=year;

	//we assert the mid in the category that is supossed t obe
	new_movie->category=category;

	//we assert in the new movie that points to info the *info
	new_movie->info=*info;

	//we store the next node as null
	new_movie->next=NULL;

	//we create tmp node to find the movie
	struct new_movie* current=new_movieHead;

	while(current != NULL)
	{
		//if the movie exists
		if(current->info.mid==mid) 
		{
			printf("Duplicate movie!\n");
			return -1;
		}
		current=current->next;
	}

	//we create a key value in which we sotre the movie id 
	int key= mid;

	//if the list of the new movies is empty or the key value is less than the mid value
	if(new_movieHead==NULL || key<new_movieHead->info.mid) {

			//in the next node of the new movie struct we store the head
			new_movie->next=new_movieHead;

			//we update the head
			new_movieHead=new_movie;
	}
	else{
		//else we store the head to a tmp node
		tmp=new_movieHead;
		
		//while we have a next node and while the mid is less than the key value
		while(tmp->next!=NULL && tmp->next->info.mid < key)
		{
			tmp=tmp->next;
		}

		//we assert the nexto of the new movie to be the next node of the tmp 
		new_movie->next=tmp->next;

		//we stort the new movie node in the next node of the tmp
		tmp->next=new_movie;
	}

	printf("\n\n<%u><%d><%u>\n",mid,category,year);
	printf("  New movies= ");
	struct new_movie* curr=new_movieHead;
	int counter =1;

	while(curr!=NULL)
	{
		printf("<%u_%d,%d_%d,%u_%d>,",mid,counter,category,counter,year,counter);
		counter++;
		curr=curr->next;
	}
	printf("\nDONE\n");

	
	return 0;
}


void distribute_new_movies(void) {
	//we create a node in  which we will store the head for th new movies
    struct new_movie* current = new_movieHead;


	//we intialize each category which is a single linked list with null
    for (int i = 0; i < SIZE; i++) {
        DistributeMovies[i] = NULL;
    }


    while (current != NULL) {
		//we create a new movie pointeer and we assert it as the next
        struct new_movie* next = current->next;

		//we assert the cat in the current category we are at the while
        movieCategory_t cat = current->category;

		
        struct new_movie* prev = NULL;

		// in the curCategory we store the value of the array that has the categories with the position being the current category
        struct new_movie* curCategory = DistributeMovies[cat];

		//while the list is not empty and while the mid of the curCategory is less than the mid of the current category
        while (curCategory != NULL && curCategory->info.mid < current->info.mid) {

			//store in the prev category the current category 
            prev = curCategory;
            curCategory = curCategory->next;
        }

		//if no category was stored 
        if (prev == NULL) {
			//we assert in the next node of the current the value of the array that has the categories
            current->next = DistributeMovies[cat];

			//and we update the category 
            DistributeMovies[cat] = current;
        } else { //if a category was stored

			//in the next node of the prev we assert the current node 
            prev->next = current;

			//we change the category
            current->next = curCategory;
        }

        if (current == new_movieHead) {
            new_movieHead = next;
        }
        
        current = next;
    }

	print_movies();
}


int watch_movie(int uid,unsigned mid)
{
	struct user* finduser=head1;//this will help us to find the user
	struct movie_info info;
	int userfound=0,moviefound=0;

	 
	
	while(finduser->uid != -1 ) 
	{	
		if(finduser->uid==uid){
			userfound=1; //update the userfound 
			break;
		}
		finduser=finduser->next;
	}

	if(userfound==1) //if you found the user 
	{	
		//for each category store the current one in a struct * of new movie type 
		//and iterate through the list to find the movie that the user is watching
		for(int i=0;i<6;i++)
		{
			struct new_movie* current=DistributeMovies[i];
			while(current!= NULL){
				if(current->info.mid==mid){
					info=current->info;
					moviefound=1;
					break;
				}
				current=current->next;	
			}
		}
	}

	//if both the user and the movie were found
	//create a new node for the Watchistory of the user
	if(userfound==1 && moviefound==1){
		 
		struct movie* AddWH=malloc(sizeof(struct movie));

		

		if(AddWH==NULL)
			return -1;
		
		

		AddWH->info=info;
		AddWH->next=finduser->watchHistory;
		finduser->watchHistory=AddWH;

		printf("\n\nW<%d><%u>\n",uid,mid);
		
		int counter=1;
		printf(" User<%d>Watch History=",uid);
		struct movie* curr=finduser->watchHistory;
		while(curr!=NULL){
			printf("<%u_%d>,",curr->info.mid,counter);	
			counter++;
			curr=curr->next;
		}
		printf("\nDONE\n");

		return 0;
	} 
  return -1;
}

struct movie* popd(struct movie** watchistory)
{
	//we first use a double pointer because we don't want to change the real watch history

	if((*watchistory)==NULL)
	{
		return NULL;
	}

	//we create a new Head in which we assert the head that is called watchistory and is a parameter
	struct movie* newHead = *watchistory;

	//in order to delete the node	we update the head to point to the next node
	*watchistory = (*watchistory)->next;
	newHead->next=NULL;
	
	//we return the updated head
	return newHead;
}

void InsertAsNext(struct suggested_movie** cf,struct movie* cm)
{
	struct suggested_movie* new=malloc(sizeof(struct suggested_movie));
	//in this one we create a new node for the double linked 
	//we assert the info which includes the year and movie id 
	//and we also create the prev and next nodes  to point to null
	new->info=cm->info;
	new->next=NULL;
	new->prev=NULL;

	//if the head of the list of the user is null 
	//we assert the new node as to be the head due to the fact that it is the only one at the moment
	if((*cf)==NULL){
		*cf=new;
	}else{
		//if it is not the first node we add the node in the position that it should be added
		struct suggested_movie* current = *cf;

		while(current->next!=NULL)
		{
			current = current->next;
		}

		//we update the pointers
		new->prev = current;
		current->next = new;
	}
}

void InsertAsPrev(struct suggested_movie** cb,struct movie* cm)
{
	struct suggested_movie* new=malloc(sizeof(struct suggested_movie));
	//same as before
	new->info=cm->info;
	new->next=NULL;
	new->prev=NULL;

	if((*cb)==NULL){
		*cb=new;
	}else{
		//this time we want to add it from the back so insted of next we go prev
		struct suggested_movie* current = *cb;

		while(current->prev!=NULL)
		{
			current = current->prev;
		}

		new->next = current;
		current->prev = new;
	}
}

int suggest_movies(int uid) {
    struct user* current = head1;


    while (current->uid !=-1 && current->uid != uid) {
        current = current->next;
    }

    if (current == NULL) return -1;

	//this 2 are need to point to the head and to the tail of the double linked list 
	//because with the head we insert from the front 
	//with the tail we insert from the back
    struct suggested_movie* currFront = current->suggestedHead;
    struct suggested_movie* currBack = current->suggestedTail;
    int counter = 0;

	struct user* curr = head1;

	//we want to "take" movies for each user to suggest to a user
    while (curr->uid != -1) {
        if (curr->uid != uid) { //if this is not the user that the movies will be suggested
			//pop a movie from this user's watchistory
            struct movie* CurrentMovie = popd(&(curr->watchHistory));
			//if this user has movies
            if (CurrentMovie != NULL) {
				//if it is %2 ==0 insert it from the front else from the back
                if (counter % 2 == 0) {
                    InsertAsNext(&(current->suggestedHead), CurrentMovie);
					//if the list of the suggested movies from the user is null we assert the currfront as the new head
                    if (currFront == NULL) {
                        currFront = current->suggestedHead;
                    } else {
						//else we just add it 
                        currFront = currFront->next;
                    }
                } else {//same but for prev
                    InsertAsPrev(&(current->suggestedTail), CurrentMovie);                   
                    if (currBack == NULL) {
                        currBack = current->suggestedTail;
                    } else {
                        currBack = currBack->prev; 
                    }
                }
                counter++; //we increment the counter 
            }
        }
        curr = curr->next;
    }

	//if both of the new list that we created are not empty we have to link them in order for it to 1 double linked list
    if (currFront != NULL && currBack != NULL) {
        currFront->next = currBack;
        if (currBack != NULL) {
            currBack->prev = currFront;
        }
    }

	int count = 0;
	printf("\nS<%d>\n",uid);
	printf(" User<%d> Suggested Movies= ");
	struct suggested_movie* temporary=current->suggestedHead;

	while(temporary!=NULL) {
		count++;
		printf("<%d_%d>,",temporary->info.mid,count);
		temporary=temporary->next;
	}
	printf("\nDONE\n");


    return 0;
}



struct suggested_movie* createL(struct movie_info info)
{
	//here we create a new List of suggested movies
	struct suggested_movie* newNode=malloc(sizeof(struct suggested_movie));
	newNode->prev=NULL;
	newNode->next=NULL;
	newNode->info=info;
			
	return newNode;
}

struct suggested_movie* InsertL(struct suggested_movie* NewLL,struct movie_info info){
	struct suggested_movie* new=createL(info);
	//and here is the insertion in which we check if the list is empty etc
	if(NewLL==NULL) return new;
	else{
		struct suggested_movie* current=NewLL;
		while(current->next!=NULL){
			current=current->next;
		}
		current->next=new;
		new->prev=current;
		return NewLL;
	}
}

struct suggested_movie* SortNewList(struct suggested_movie* List){	
	//here we create a node
	struct suggested_movie* sorted=malloc(sizeof(struct suggested_movie));
	sorted->prev=NULL;
	sorted->next=NULL;

	//here we pass the parameter which is the list in the current pointer
	struct suggested_movie* current=List;	

	//the next node of sorted will point to the list
	sorted->next=current;
	struct movie_info key;

	current=current->next; //we advnace
	while(current){ 
		key=current->info; //in the key we put the current info
		//we store the prev of the current node in a new tmp node
		struct suggested_movie* current2=current->prev;

		//while current2 is not null and while the mid is bigger than the key
		while(current2 && current2->info.mid>key.mid)
		{
			//if the next node is not null than insert the new node
			if(current2->next) current2->next->info=current2->info;


			current2=current2->prev;
		}
		//if current2 after the while has reached the null
		if(!current2)
		{	
			//in the next node of the sorted add the key in the info
			//that means that it was inserted in the correct position because this list is sorted
			sorted->next->info=key;
		}
		else{
			current2->next->info=key;
		}

		current=current->next;
	}

	//we return the next node of the sorted list
	return sorted->next;
}	

int filtered_movie_search(int uid, movieCategory_t category1,movieCategory_t category2, unsigned year){
	struct new_movie* CatMovies[2];//we store the 2 categories in a new array of categories
	CatMovies[0]=DistributeMovies[category1];
	CatMovies[1]=DistributeMovies[category2]; 
	struct suggested_movie* NewList=NULL; 


	for(int i=0;i<2;i++)
	{
		struct new_movie* current=CatMovies[i]; //we use a tmp node to store the current category
		//we iterate and when we find the the info.year that is bigger than the parameter year
		//we add the node to the NewList structure Which is a list 
		//without deleting the node from the new_movie structure
		while(current!=NULL)
		{
			if(current->info.year>=year)
			{
				NewList=InsertL(NewList,current->info);
			}
			current=current->next;
		}
	}
	//we sort the NewList

	NewList=SortNewList(NewList);

	struct suggested_movie* app=head;
	//if the list is empty the head will be the head of the newList
	if(app==NULL){
		app=NewList;
	}
	else{
		//else we create a new node and we asserted to the head 
		//we iterate through it and than when it will reach the end it means that it is the perfect position to add the
		//new node the NewList
			struct suggested_movie* LastN=app;
			while(LastN->next!=NULL)
				LastN=LastN->next;

			LastN->next=NewList;
	}

	
	printf("DA<%d> <%d> <%d> <%u>\n",uid,category1,category2,year);
	printf("  User %d Suggested Movies = ", uid);
    struct suggested_movie* currentNode = NewList;

    while (currentNode != NULL) {
        printf("%d", currentNode->info.year);
        if (currentNode->next != NULL) {
            printf(", ");
        }
        currentNode = currentNode->next;
    }
    printf("\nDONE\n");


}



void print_users(void){	
	struct user* AllUsers=head1;
	printf("\nP\n");

	while(AllUsers->uid != -1){
		printf("Users: \n");
		printf(" <%d>\n", AllUsers->uid);
		printf("  Suggested:");
		struct suggested_movie* movieprint=AllUsers->suggestedHead;
		while(movieprint != NULL)
		{
			printf("<mid_%u>",movieprint->info.mid);
			movieprint=movieprint->next;
		}
		printf("\n");

		printf("  Watch History: ");
		struct movie* current=AllUsers->watchHistory;
		while(current!=NULL)
		{
			printf("<mid_%u>", current->info.mid);
			current=current->next;	
		}

		AllUsers = AllUsers->next;
		printf("\n");
	}

	printf("\nDONE\n");
}

void take_off_movie(unsigned mid)
{
	printf("T<%u>\n",mid);

	struct user* current=head1;


	while(current->uid!=-1)
	{
		//this time we only need 1 pointer for the suggested movies list 
		//because we Linked them
		struct suggested_movie* currFront=current->suggestedHead;

		//if the Head is null that means the movie doesnt exist here
		if(currFront==NULL)
		{
			printf("List is empty\n");
		}
		else if(currFront->info.mid==mid)
		{	
			//else if we found the movie we delete it from the double linked list 
			//and we update the head pointers
			//we also check if it was the last node 
			//so we also update the tail if necessary
			current->suggestedHead=current->suggestedHead->next;
			if(current->suggestedHead==NULL)
				current->suggestedTail=NULL;

		}else{
			//else we search for the movie
			while(currFront->next != NULL && currFront->next->info.mid !=mid)
			{
				currFront = currFront->next;
			}

			if(currFront->next==NULL)
			{
				break;
			}else{//if we found it 
			//we delete it again and also updating the pointers
				struct suggested_movie* tmp=currFront->next;

				currFront->next=currFront->next->next;
				if(currFront->next!=NULL){
					currFront->next->prev=currFront;
				}else{
					current->suggestedTail=currFront;
				}

				
				free(tmp);
				
			}
		}
		printf(" <%u> removed from <uid_%d> suggested list",mid,current->uid);
		current=current->next;
	}

	printf("\n\n");
		
	//here we delete from the category list
	int movie_found=0;
	for(int i=0;i<SIZE;i++){	
		struct new_movie* previous = NULL;
		struct new_movie* current=DistributeMovies[i];
		while(current !=NULL && current->info.mid != mid)
		{
			previous = current;
			current=current->next;
		}

		//if the movie wasnt found in this category go to the next category
		if(current==NULL)
		{
			continue;
		}
		else{ 
			//else if the movie was found here 
			movie_found=1;
			
			//the next of the previous pointer will be asserted with the next of the current pointer
			if(previous)
			{
				previous->next = current->next;
			}
			else
			{
				//else go to the next category
				DistributeMovies[i] = DistributeMovies[i]->next;
			}

			printf("<%u> removed from <%d> category list\n",mid,current->category);
			printf("\nCategory list=");
			struct new_movie* tempo=DistributeMovies[i];

			while(tempo!=NULL){
				printf("<mid_%u>,",tempo->info.mid);
				tempo=tempo->next;
			}
		}
		if(movie_found==1)
			break;

		
	}
	
	printf("\n\nDONE\n");
}


int main(int argc, char *argv[])
{
	head1 = (struct user*)malloc(sizeof(struct user));
    head1->uid = -1;
    head1->suggestedHead = NULL;
    head1->suggestedTail = NULL;
    head1->watchHistory = NULL;

    for(int i = 0; i < SIZE; i++)
    {
        DistributeMovies[i] = NULL;
    }

	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file) {
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}

	while (fgets(line_buffer, MAX_LINE, event_file)) {
		char *trimmed_line;
		char event;
		int uid;
		unsigned mid, year;
		movieCategory_t category1, category2;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/* 
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1) {
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event) {
			/* Comment, ignore this line */
			case '#':
				break;
			case 'R':
				if (sscanf(trimmed_line, "R %d", &uid) != 1) {
					fprintf(stderr, "Event R parsing error\n");
					break;
				}
				register_user(uid);
				break;
			case 'U':
				if (sscanf(trimmed_line, "U %d", &uid) != 1) {
					fprintf(stderr, "Event U parsing error\n");
					break;
				}
				unregister_user(uid);
				break;
			case 'A':
				if (sscanf(trimmed_line, "A %u %d %u", &mid, &category1,
							&year) != 3) {
					fprintf(stderr, "Event A parsing error\n");
					break;
				}
				add_new_movie(mid, category1, year);
				break;
			case 'D':
				distribute_new_movies();
				break;
			case 'W':
				if (sscanf(trimmed_line, "W %d %u", &uid, &mid) != 2) {
					fprintf(stderr, "Event W parsing error\n");
					break;
				}
				watch_movie(uid, mid);
				break;
			case 'S':
				if (sscanf(trimmed_line, "S %d", &uid) != 1) {
					fprintf(stderr, "Event S parsing error\n");
					break;
				}
				suggest_movies(uid);
				break;
			case 'F':
				if (sscanf(trimmed_line, "F %d %d %d %u", &uid, &category1,
							&category2, &year) != 4) {
					fprintf(stderr, "Event F parsing error\n");
					break;
				}
				filtered_movie_search(uid, category1, category2, year);
				break;
			case 'T':
				if (sscanf(trimmed_line, "T %u", &mid) != 1) {
					fprintf(stderr, "Event T parsing error\n");
					break;
				}
				take_off_movie(mid);
				break;
			case 'M':
				print_movies();
				break;
			case 'P':
				print_users();
				break;
			default:
				fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
						event);
				break;
		}
	}
	fclose(event_file);
	return 0;
}
