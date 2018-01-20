#include<stdio.h>
#include<LinkedListAPI.h>
#include<string.h>

List initializeList(char* (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first,const void *second)){
	List list1; 
    /*
	list1 = malloc(sizeof(List));
	if(list1 == NULL){
		printf("no memory availible\n");
		exit(0);
	}
    
	list1->head = NULL;
	list1->tail = NULL;
    list1->deleteData = *deleteFunction;
    list1->compare = compareFunction;
    list1->printData = printFunction;
    */

    list1.head= NULL;
    list1.tail=NULL;
    list1.length = 0;
    list1.deleteData = deleteFunction;
    list1.compare= compareFunction;
    list1.printData= printFunction;

	return list1;
}

Node *initializeNode(void *data){
	Node* node1;
	node1 = malloc(sizeof(Node));
	if (node1 == NULL){
		printf("No more memory availible\n");
		exit(0);
	}

	node1->data = data;
	node1->previous = NULL;
	node1->next = NULL;

return node1;
}


void insertFront(List *list, void *toBeAdded){
    
    if (list == NULL || toBeAdded == NULL){
        return;
    }
    
    Node* newNode = initializeNode(toBeAdded);
    
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
        newNode->next = list->head;
        list->head->previous = newNode;
        list->head = newNode;
    }
    list->length = getLength(*list);
}

void insertBack(List *list, void *toBeAdded){

    if (list == NULL || toBeAdded == NULL){
        return;
    }
                        
    Node* newNode = initializeNode(toBeAdded);
                                
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
        newNode->previous = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->length = getLength(*list);
	
}

void clearList(List* list){
    
    if (list == NULL){
        return;
    }
    
    if (list->head == NULL && list->tail == NULL){
        return;
    }
    
    Node* tmp;
    
    while (list->head != NULL){
        list->deleteData(list->head->data);
        tmp = list->head;
        list->head = list->head->next;
        free(tmp);
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->length = getLength(*list);
}


void insertSorted(List *list, void *toBeAdded){
    if (list == NULL || toBeAdded == NULL){
        return;
    }
    
    if (list->head == NULL){
        insertBack(list, toBeAdded);
        return;
    }
    
    if (list->compare(toBeAdded, list->head->data) <= 0){
        insertFront(list, toBeAdded);
        return;
    }
    
    if (list->compare(toBeAdded, list->tail->data) > 0){
        insertBack(list, toBeAdded);
        return;
    }
    
    Node* currNode = list->head;
    
    while (currNode != NULL){
        if (list->compare(toBeAdded, currNode->data) <= 0){
        
            char* currDescr = list->printData(currNode->data); 
            char* newDescr = list->printData(toBeAdded); 
        
            printf("Inserting %s before %s\n", newDescr, currDescr);

            free(currDescr);
            free(newDescr);
        
            Node* newNode = initializeNode(toBeAdded);
            newNode->next = currNode;
            newNode->previous = currNode->previous;
            currNode->previous->next = newNode;
            currNode->previous = newNode;

            list->length = getLength(*list);
        
            return;
        }
    
        currNode = currNode->next;
    }
    list->length = getLength(*list);
    
    return;
}

void* deleteDataFromList(List* list, void* toBeDeleted){
    if (list == NULL || toBeDeleted == NULL){
        return NULL;
    }
    
    Node* tmp = list->head;
    
    while(tmp != NULL){
        if (list->compare(toBeDeleted, tmp->data) == 0){
            //Unlink the node
            Node* delNode = tmp;
            
            if (tmp->previous != NULL){
                tmp->previous->next = delNode->next;
            }else{
                list->head = delNode->next;
            }
            
            if (tmp->next != NULL){
                tmp->next->previous = delNode->previous;
            }else{
                list->tail = delNode->previous;
            }
            
            void* data = delNode->data;
            free(delNode);
            
            list->length = getLength(*list);
            return data;
            
        }else{
            tmp = tmp->next;
        }
    }
    
    list->length = getLength(*list);
    
    return NULL;
}

void *getFromFront(List list){
    Node *firstNode;
    firstNode = list.head;
    if(firstNode != NULL){
        return firstNode->data;
    }
    else{
        return NULL;
    }
}

void *getFromBack(List list){
    Node *lastNode;
    lastNode = list.tail;
    if(lastNode != NULL){
        return lastNode->data;
    }
    else{
        return NULL;
    }
}

char* toString(List list){
    ListIterator iter = createIterator(list);
    char* str;
        
    str = (char*)malloc(sizeof(char));
    strcpy(str, "");
    
    void* elem;
    while( (elem = nextElement(&iter)) != NULL){
        char* currDescr = list.printData(elem);
        int newLen = strlen(str)+50+strlen(currDescr);
        str = (char*)realloc(str, newLen);
        strcat(str, "\n");
        strcat(str, currDescr);
        
        free(currDescr);
    }
    
    return str;
}

ListIterator createIterator(List list){
    ListIterator iter;

    iter.current = list.head;
    
    return iter;
}


void* nextElement(ListIterator* iter){
    Node* tmp = iter->current;
    
    if (tmp != NULL){
        iter->current = iter->current->next;
        return tmp->data;
    }else{
        return NULL;
    }
}

int getLength(List list){
    ListIterator iter;
    int numel;
    iter.current = list.head;
    Node *temp;
    numel = 0;
    while(iter.current != NULL){
        temp = iter.current;
        iter.current = temp->next;
        numel++;
    }
    


return numel;

}

void* findElement(List list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord){

    ListIterator iter;
    Node* temp;
    iter.current = list.head;
    void* element;
    element = NULL;

    while (iter.current != NULL && element == NULL){
        temp = iter.current;
        if(customCompare(temp->data, searchRecord)){
            element = temp->data;
        }
        iter.current = temp->next;
    }
    return element;



}
/*

My own test script!
bool customCompare(const void* first, const void* second){
    int listVal = *(int*)first;
    int compVal = *(int*)second; 
    if(listVal == compVal){

        return 1;
    }
    return 0;


}




int main(void){
    List list;
    list = initializeList(NULL, NULL, NULL);
    if(list.deleteData == NULL){
        printf("List created\n");
    }
    int test1 = 1;
    int test2 = 2;
    int test3 = 3;
    
    insertFront(&list, &test1);
    insertBack(&list, &test2);
    insertBack(&list, &test3); 

    if (*((int*)list.head->data) == 1){
        printf("node 1 created");
    }
    int len;
    len = getLength(list);
    if (len == 3){
        printf("correct length\n");
    }
    int find = 3;
    int data;

    data = *(int*)findElement(list, customCompare, &find);
    printf(">>>>>>%d\n", data);

    




    return 0;
}

*/

/*void deleteList(List *list){
	Node *currentNode;
	Node *nextNode;
	Node *lastNode;
	currentNode = list->head;
	lastNode = list->tail;
	int i = 0;
	while(currentNode != lastNode){
		nextNode = currentNode->next;
		free(currentNode);
		currentNode = nextNode;
		i++;
		printf("node#: %d deleted \n", i);
	}
	free(lastNode);
	i++;
	printf("Final Node # %d deleted\n", i);
}

void insertSorted(List *list, void *toBeAdded){
	Node *addNode;
	Node *currentNode;
	int compare;
	compare = 0;
	currentNode = list->head;
	addNode = initializeNode(toBeAdded);
	while (currentNode->next != NULL){
		list->compare(currentNode->data, addNode->data);
		if(compare){
		//add after
			Node *tempNode;
			tempNode=currentNode->next;
			currentNode->next = addNode;
			addNode->next = tempNode;
			addNode->previous = currentNode;
			tempNode->previous = addNode;
			break;
		}
		currentNode = currentNode->next;
	}
}

void* deleteDataFromList(List *list, void *toBeDeleted){
       	Node *currentNode;
	Node *previousNode;
	Node *deleteNode;
       	currentNode = list->head;
	if (list->head==NULL){
		//return EXIT_FAILURE;

	}
	if(currentNode-> data != toBeDeleted){
		currentNode = currentNode->next;
	}
	else{
		deleteNode = currentNode;
		currentNode = currentNode->next;
		
		list->head = currentNode;
		currentNode->previous = NULL;
		free(deleteNode);
		
//		return EXIT_SUCCESS;
	}
       	while (currentNode != NULL){
       		if(currentNode->data == toBeDeleted){
       			deleteNode=currentNode;
			previousNode = currentNode->previous;
			currentNode = currentNode->next;
			free(deleteNode);
void* deleteDataFromList(List* list, void* toBeDeleted){
    if (list == NULL || toBeDeleted == NULL){
        return NULL;
    }
    
    Node* tmp = list->head;
    
    while(tmp != NULL){
        if (list->compare(toBeDeleted, tmp->data) == 0){
            //Unlink the node
            Node* delNode = tmp;
            
            if (tmp->previous != NULL){
                tmp->previous->next = delNode->next;
            }else{
                list->head = delNode->next;
            }
            
            if (tmp->next != NULL){
                tmp->next->previous = delNode->previous;
            }else{
                list->tail = delNode->previous;
            }
            
            void* data = delNode->data;
            free(delNode);
            
            return data;
            
        }else{
            tmp = tmp->next;
        }
    }
    
    return NULL;
}
       			currentNode->previous = previousNode;
			previousNode->next = currentNode;
       		//	return EXIT_SUCCESS;
       		}
       		currentNode = currentNode->next;
       	}
	return NULL;
}



void printForward(List list){
	Node *currentNode;
	currentNode = list.head;
	while (currentNode != NULL){
		list.printData(currentNode->data);
		currentNode = currentNode->next;
	}
}

void printBackwards(List list){
	Node *currentNode;
	currentNode = list.tail;
	while (currentNode != NULL){
		list.printData(currentNode->data);
		currentNode = currentNode->previous;
	}
}
*/
