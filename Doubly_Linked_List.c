#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Button definition
typedef struct Button {
    Rectangle rec;
    Color col;
} Button;

// Initializes button properties
void init_button(Button *but, Rectangle rect, Color colr) {
    but->rec = rect;
    but->col = colr;
}

// Checks if the mouse is over the button
bool is_mouse_over_button(Button but) {
    return CheckCollisionPointRec(GetMousePosition(), but.rec);
}

// Checks if left mouse click is pressed on the button
bool is_left_click_pressed(Button but) {
    return (is_mouse_over_button(but) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

//noeud def
struct Node {
    int data;
    Color col;
    struct Node* prev;
    struct Node* next;
};

//noeud create
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->col = BLUE;
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

//DLL create set values
struct Node* createDoublyLinkedList(int *tab,int size) {
    struct Node* head = NULL;
    struct Node* tail = NULL;
  
    for (int i = 0; i < size; ++i) {
        struct Node* newNode = createNode(tab[i]);

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }        
    }
    tail->next = NULL;
    return head;
}

//DLL Create Random values
struct Node* createRandomDoublyLinkedList(int size){
    struct Node* head = NULL;
    struct Node* tail = NULL;
    
    for (int i = 0; i < size; ++i){
        struct Node* newNode = createNode(GetRandomValue(-1000,1000));

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }       
    }
    tail->next = NULL;
    return head;
}

//DLL get end
struct Node* getEnd(struct Node* head)
{
    if (head != NULL) 
    {
    while(head->next != NULL){
        head = head->next;
    }
    
    }
    return head;

}

//noeud represent
void drawNode(struct Node* node, Vector2 pos, int nodeCount) {
    int rectWidth = SCREEN_WIDTH / (nodeCount * 2);
    int rectHeight = SCREEN_HEIGHT / 20;

    DrawRectangle((int)pos.x - rectWidth / 2, (int)pos.y - rectHeight / 2, rectWidth, rectHeight, node->col);
    DrawRectangleLines((int)pos.x - rectWidth / 2, (int)pos.y - rectHeight / 2, rectWidth, rectHeight, BLACK);
    DrawText(TextFormat("%d", node->data), (int)pos.x - MeasureText(TextFormat("%d", node->data), 20) / 2, (int)pos.y - 10, 20, BLACK);
}

//arrow represent
void drawArrow(Vector2 start, Vector2 end) {
    DrawLineV(start, end, BLACK);
}

// fonction visu DLL
void visualizeDoublyLinkedList(struct Node* head,RenderTexture2D canvas){
    int nodeCount = 0;
    struct Node* temp = head;

    while (temp != NULL) {
        nodeCount++;
        temp = temp->next;
    }
    BeginTextureMode(canvas);
                
    int count = 0;
    struct Node* current = head;
    Vector2 nodePosition = {SCREEN_WIDTH / (nodeCount + 1), SCREEN_HEIGHT / 2};

    while (current != NULL) {
        drawNode(current, nodePosition, nodeCount);

        if (current->next != NULL) {
            Vector2 arrowStart = {nodePosition.x + (SCREEN_WIDTH / (nodeCount * 2))/2, nodePosition.y+5};
            Vector2 arrowEnd = {nodePosition.x + (SCREEN_WIDTH / nodeCount + 1) - (SCREEN_WIDTH / (nodeCount * 2))/2, nodePosition.y+5};
                
            drawArrow(arrowStart, arrowEnd);
                
            Vector2 arrowStart2 = {nodePosition.x + (SCREEN_WIDTH / (nodeCount * 2))/2, nodePosition.y-5};
            Vector2 arrowEnd2 = {nodePosition.x + (SCREEN_WIDTH / nodeCount + 1) - (SCREEN_WIDTH / (nodeCount * 2))/2, nodePosition.y-5};
              
            drawArrow(arrowStart2, arrowEnd2);
         
            DrawTriangle(arrowStart, (Vector2){arrowStart.x +5, arrowStart.y + 5}, (Vector2){arrowStart.x + 5, arrowStart.y - 5}, BLACK);
        }
            
        if(current->prev != NULL){
            Vector2 arrowStart = {nodePosition.x - (SCREEN_WIDTH / (nodeCount * 2))/2, nodePosition.y-5};
                
            DrawTriangle(arrowStart, (Vector2){arrowStart.x -5, arrowStart.y - 5}, (Vector2){arrowStart.x - 5, arrowStart.y + 5}, BLACK);
        }

        nodePosition.x += SCREEN_WIDTH / (nodeCount + 1);
        current = current->next;
        count++;
    }
    EndTextureMode();
}
  
//fonction to change color of current node
void change_color(struct Node *current,Color col){
    if(current != NULL){
        current->col=col; 
    }
}
  
//fonction to search
void search_val(struct Node *current,int val,bool *st){
    if (current != NULL){
        if(current->prev != NULL) change_color((current->prev),BLUE);
        if(current->data == val){
            change_color((current),GREEN);
            *st = true;
        }
        else{
            change_color(current,RED);        
        }
    }
}
  
//fonction to sort
void selectionSortDoublyLinkedList(struct Node* head) {
    if (head == NULL || head->next == NULL) {
        return; // Nothing to sort if the list is empty or has only one node
    }

    struct Node* current = head;

    while (current->next != NULL) {
        struct Node* minNode = current;
        struct Node* temp = current->next;

        while (temp != NULL) {
            if (temp->data < minNode->data) {
                minNode = temp;
            }
            temp = temp->next;
        }

        // Swap data between current and minNode
        int tempData = current->data;
        current->data = minNode->data;
        minNode->data = tempData;

        current = current->next;
    }
}

//Check if value is in the Dll  
int isNumberInList(struct Node* head, int number) {
    while (head != NULL) {
        if (head->data == number) {
            return 1;  // Number is found
        }
        head = head->next;
    }
    return 0;  // Number is not found
}

//Delete element in DLL 
struct Node* DeleteElement(struct Node** head,int value) { 

    struct Node* current = *head;

    while (current != NULL && current->data != value) {
        current = current->next;
    }
    
    // If the node to be deleted is the head
    if (current == *head) {
        *head = current->next;
    }
    
    // Adjust the pointers of the previous and next nodes
    if (current->prev != NULL) {
        current->prev->next = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    free(current);

    return *head;
}

//insert element par la tete 
void insertAtHead(struct Node** head, int value) {
    // Create a new node
    struct Node* newNode = createNode(value);
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = *head;

    // If the list is not empty, update the previous pointer of the current head
    if (*head != NULL) {
        (*head)->prev = newNode;
    }

    // Update the head pointer to the new node
    *head = newNode;
}


//pour voir si la liste est triée
bool isDoublyLinkedListSorted(struct Node* head) {
    
    if (head == NULL || head->next == NULL) {
        return true;
    }
    
    struct Node* current = head;
    while (current->next != NULL) {
        
        if (current->data > current->next->data) {
            return false;
        }
        current = current->next;
    }
    return true;
}

void visu_trie(struct Node* head,struct Node** Pmin)
{
   
   if(head != NULL)
   {
     
    
    if(head->data <= (*Pmin)->data){
        *Pmin = head;
        (*Pmin)->col = GREEN;
    }else{
        head->col = RED;
    }
    

   }

}


int main(){
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Input Visualization");
    
    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //init the rendertexture
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
    
    SetTargetFPS(60);
    
    //button default dimesions
    float buttonWidth = SCREEN_WIDTH * 0.4f;
    float buttonHeight = SCREEN_HEIGHT * 0.2f;
    

    //button setup
    Button button_0 = {0};
    init_button(&button_0,(Rectangle){SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight }, RED);
    
    Button button = { 0 };
    init_button(&button, (Rectangle) { SCREEN_WIDTH / 2 - buttonWidth / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2, buttonWidth, buttonHeight }, BLACK);
    
    Button button_random = { 0 };
    init_button(&button_random,(Rectangle){SCREEN_WIDTH / 2 - buttonWidth * (3 / 2) , SCREEN_HEIGHT / 2 + buttonHeight / 2 , buttonWidth / 2 , buttonHeight }, RED);
    
    Button button_set = { 0 };
    init_button(&button_set,(Rectangle){SCREEN_WIDTH / 2 + buttonWidth / 3 , SCREEN_HEIGHT / 2 + buttonHeight / 2  , buttonWidth / 2 , buttonHeight }, RED);

    Button button_sort = { 0 };
    init_button(&button_sort, (Rectangle){SCREEN_WIDTH / 2.7 + buttonWidth * (3 / 2) , SCREEN_HEIGHT - 50, buttonWidth / 2, buttonHeight / 2 - 20}, GREEN);
   
    Button button_supp = { 0 };
    init_button(&button_supp, (Rectangle){SCREEN_WIDTH / 2.3 - buttonWidth * (3 / 2), SCREEN_HEIGHT - 50, buttonWidth / 2, buttonHeight / 2 - 20}, RED);
   
    Button button_insert = { 0 };
    init_button(&button_insert, (Rectangle){SCREEN_WIDTH / 2 - buttonWidth/4 , SCREEN_HEIGHT - 50, buttonWidth / 2, buttonHeight / 2 - 20}, BLUE);
   
    Button button_new = { 0 };
    init_button(&button_new, (Rectangle){30, 30 , buttonWidth / 2 , buttonHeight / 2 - 20}, GREEN);
    
    Button button_search = { 0 };
    init_button(&button_search, (Rectangle){SCREEN_WIDTH - buttonWidth / 2 - 30 , 30 , buttonWidth / 2 , buttonHeight / 2 - 20}, GREEN);

    // varibles de deroulement
    bool step1 = true;
    bool step2 = false;
    bool step3 = false;
    bool step4_1 = false;
    bool step4_2 = false;
    bool step5 = false;
    bool step_search = false;
    bool step_search_val = false;
    bool step_del = false;
    bool step_add = false;
    bool start=false;
    bool step_trie=false;
    //reset affichage
    bool occ = false;
    bool aff = false;
    bool NoDel = false;
   
    // Text input properties
    int textSize = 0;
    char text[64] = { 0 };
    Vector2 textPosition = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10 };
    bool inputComplete = false;
    int enteredNumber = 0;
    bool dataInputComplete = false;

    
    char searchInput[32] = "";
    static int searchInputSize = 0;
    
    static bool searchInputComplete = false;
    static int searchValue = 0;
    
    
    static int* values = NULL;
    struct Node* head = NULL;
    int currentNode = 1;
    
    int nodeCount = 0;
    struct Node* temp;
    struct Node* reset;
    
    int x;
    struct Node *p = NULL;
    struct Node *after = NULL;
    struct Node *before = NULL;

    while (!WindowShouldClose()) {   
    
        //debut step1: Welcome screen 
        if (step1){
            BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            DrawText("Welcome, press the button below to create a DLL.",SCREEN_WIDTH/2- MeasureText("Welcome, press the button below to create a DLL.",20)+250,SCREEN_HEIGHT/3,20,BLACK);
            DrawRectangleRec(button_0.rec,button_0.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight , BLACK );
            DrawText("Create",button_0.rec.x + button_0.rec.width / 2 - MeasureText("Create",20) + 30,button_0.rec.y + button_0.rec.height / 2 - 20 / 2, 20 , BLACK );
    
            EndTextureMode();
        }
     
        //deroulement des etapes
        if(is_left_click_pressed(button_0) && step1 ) { 
            step1=false;
            step2 = true;
        }
 
        //debut step2: Définir la taille de la DLL
        if(step2){ 
            if (!inputComplete) {
                int key = GetKeyPressed(); // Get the pressed key 

                if ((key >= 48 && key <= 57) && (textSize < 63)) { // Check if the key is a number and within text limit
                    text[textSize] = (char)key; // Store entered digit in the text array
                    textSize++;
                }
                else if (key == KEY_BACKSPACE && textSize > 0) { // Check for backspace to delete the last digit
                    textSize--;
                    text[textSize] = '\0'; // Null-terminate the string to erase the last character
                }

                if (IsKeyPressed(KEY_ENTER) && (atoi(text) != 0)) { // Clicking on ENTER will finalize the input
                    inputComplete = true;
                    enteredNumber = atoi(text); // Convert the entered text to an integer
                }
            }
            
            BeginTextureMode(target);
            ClearBackground(GRAY);
            DrawRectangleRec(button.rec, button.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight , WHITE );

            DrawText(text, textPosition.x, textPosition.y, 40, WHITE); // Display the entered digits

            if (!inputComplete ) {
                DrawText("Enter the size of the DLL.", SCREEN_WIDTH / 2 - MeasureText("Enter the size of the DLL.", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight , 20, BLACK);
                DrawText("Click on the ENTER key to finalize input.", SCREEN_WIDTH / 2 - MeasureText("Click on the ENTER key to finalize input.", 20) / 2, SCREEN_HEIGHT / 2 + buttonHeight , 20, BLACK);
            }
            else{
                DrawText("Input Completed!", SCREEN_WIDTH / 2 - MeasureText("Input Completed!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 130, 20, BLACK);
                DrawText("press on the SPACEBAR for the next step.", SCREEN_WIDTH / 2 - MeasureText("press on the SPACEBAR for the next step!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 170, 20, BLACK);     
            }

            EndTextureMode();
        }
        
        if(IsKeyPressed(KEY_SPACE) && inputComplete ){
            step2 = false;
            step3 = true;
        }

        //debut step3: choix du type de la liste (random or pre set)
        if(step3 && enteredNumber > 0){
            BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            
            DrawText("Select one of the choices below.", SCREEN_WIDTH / 2 - MeasureText("Select one of the choices below.", 20) / 2, SCREEN_HEIGHT / 2 - 100 , 20, BLACK);
        
            DrawRectangleRec(button_random.rec, button_random.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth * (3 / 2) , SCREEN_HEIGHT / 2 + buttonHeight / 2 , buttonWidth / 2 , buttonHeight , BLACK );
            DrawText("random values",button_random.rec.x + button_random.rec.width / 2 - MeasureText("random values",18) + 60,button_random.rec.y + button_random.rec.height / 2 - 20 / 2 ,18,BLACK);
            
            DrawRectangleRec(button_set.rec, button_set.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 + buttonWidth / 3 , SCREEN_HEIGHT / 2 + buttonHeight / 2  , buttonWidth / 2 , buttonHeight , BLACK );
            DrawText("pre set values",button_set.rec.x + button_set.rec.width / 2 - MeasureText("pre set values",18) + 60,button_set.rec.y + button_set.rec.height / 2 - 20 / 2 ,18,BLACK);
 
            EndTextureMode();
        }
 
        if(step3){
            if(is_left_click_pressed(button_set)){
                step3 = false;
                step4_1 = true;
            }           
            if(is_left_click_pressed(button_random)){
                step3 = false;
                step4_2 = true;
            }
        }
        
        //debut step4_1: Donner les valeurs de la pre set DLL
        if(step4_1){ 
            BeginTextureMode(target);
            ClearBackground(GRAY);
            DrawText("Enter values for the Doubly Linked List:", SCREEN_WIDTH / 2 - MeasureText("Enter values for the Doubly Linked List:", 20) / 2, SCREEN_HEIGHT / 4, 20, BLACK);
            DrawText("Press ENTER after each input.", SCREEN_WIDTH / 2 - MeasureText("Press ENTER after each input.", 20) / 2, SCREEN_HEIGHT / 4 + 40, 20, BLACK);
            EndTextureMode();

            // Input values of nodes for the DLL
            if (values == NULL) {
                values = (int*)malloc(sizeof(int) * enteredNumber);
                for (int i = 0; i < enteredNumber; ++i) {
                    values[i] = 0;
                }
            }

            static char inputText[32] = "";
            static int textSizeInput = 0;
            static Vector2 textPositionInput = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10 };

            if (currentNode <= enteredNumber) {
                if (!IsKeyDown(KEY_ENTER)) {
                    int key = GetKeyPressed();
                    if ((key >= 48 && key <= 57) && (textSizeInput < 31)) {
                        inputText[textSizeInput] = (char)key;
                        textSizeInput++;
                    }
                    else if (key == KEY_BACKSPACE && textSizeInput > 0) {
                        textSizeInput--;
                        inputText[textSizeInput] = '\0';
                    }
                }
                else if (IsKeyPressed(KEY_ENTER) && textSizeInput > 0) {
                    values[currentNode - 1] = atoi(inputText);
                    memset(inputText, 0, sizeof(inputText)); //init les valeurs du tab inputText a 0
                    textSizeInput = 0;
                    currentNode++;
                }
            }
        
            else {
                // L'apparition du bouton de tri apres la saisez des donnes 
                if (currentNode > enteredNumber) {
                    head = createDoublyLinkedList(values, enteredNumber);
                    step4_1 = false;
                    step5 = true;
                    dataInputComplete = true; // Ajout de cette ligne
                    temp = head;
                }
            }
            
            BeginTextureMode(target);
            DrawRectangleRec(button.rec, button.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight , WHITE );
            DrawText(inputText,textPositionInput.x,textPositionInput.y,40,GREEN);
            EndTextureMode();
        }
       
        //debut step 4_2: Création d'une DLL avec des valeurs aléatoires 
        if(step4_2){
            head = createRandomDoublyLinkedList(enteredNumber);
            step5 = true;
            step4_2 = false;  
            temp = head;            
        }
    
        //debut final step for creation
        if(step5){
      
            BeginTextureMode(target);
            ClearBackground(RAYWHITE);
           
           if(step_trie){
               
               BeginTextureMode(target);
              
               
               if(!isDoublyLinkedListSorted(head))
               {
                   if(temp != NULL){
                       if (after == NULL ){
                           x=before->data;
                           before->data = p->data;
                           p->data = x;
                           temp = temp->next;
                           after = temp;
                           p = temp;
                           before = temp;
                           
                           
                       }else{
                           DrawText("sorting...:",SCREEN_WIDTH / 2 - MeasureText("sorting...:", 20) / 2,SCREEN_HEIGHT / 2 - 75,20,BLACK);
                           visu_trie(after,&p);
                           after = after->next;
                           sleep(2);
                           
                       }
                           
                   }
                  
                   
                   
                   
             
               }else{
                   DrawText("Sorted:",SCREEN_WIDTH / 2 - MeasureText("Sorted:", 20) / 2,SCREEN_HEIGHT / 2 - 75,20,BLACK);
                   sleep(3);
                   step_trie = false;
               }
               EndTextureMode();
           } 
           
           
            if((step_search && start) && (temp != NULL)){
                int cpt = 1;
                 
                BeginTextureMode(target);
                if(!occ){
                    DrawText("searching...",SCREEN_WIDTH / 2 - MeasureText("searching...:", 20) / 2,SCREEN_HEIGHT / 2 - 75,20,BLACK);
                    EndTextureMode();
                    search_val(temp,searchValue,&occ);
                    cpt++;
                    
                    temp = temp->next;
                    reset = temp->prev;
                }
                else{
                    step_search = false;
                }
           
                sleep(1);
                EndTextureMode();
            }
                
            
            
            if(temp == NULL && !occ && !aff ){
                BeginTextureMode(target);
                DrawText("the value you are looking for doesnt exist in this DLL.",SCREEN_WIDTH / 2 - MeasureText("the value you are looking for doesnt exist in this DLL.:", 20) / 2,SCREEN_HEIGHT / 2 - 100,20,BLACK);
                
                
                EndTextureMode();   
            }
            if(occ && !aff){
                BeginTextureMode(target);
                DrawText(("the value has been found in the DLL!"),SCREEN_WIDTH / 2 - MeasureText("the value has been found in the DLL!", 20) / 2,SCREEN_HEIGHT / 2 - 100,20,BLACK);
                
                 
                EndTextureMode();
            }
            
            
                 
            if(aff)
            {
                if(temp != NULL)
                {
                    change_color(temp->prev,BLUE);
                }
                reset=getEnd(head);
                change_color(reset,BLUE);
            }
            
            if (NoDel){
                DrawText("No deleted element", SCREEN_WIDTH / 2 - MeasureText("No deleted element", 20) / 2, SCREEN_HEIGHT / 1.5 , 20, BLACK); 
            }
             
            visualizeDoublyLinkedList(head, target);
         
            BeginTextureMode(target);
             
            DrawRectangleRec(button_sort.rec, button_sort.col);
            DrawRectangleLines(button_sort.rec.x, button_sort.rec.y, button_sort.rec.width, button_sort.rec.height, BLACK);
            DrawText("Trie  ", button_sort.rec.x + button_sort.rec.width / 2 - MeasureText("Trie ", 20) / 2,
            button_sort.rec.y + button_sort.rec.height / 2 - 20 / 2, 20, BLACK);
             
            DrawRectangleRec(button_insert.rec, button_insert.col);
            DrawRectangleLines(button_insert.rec.x, button_insert.rec.y, button_insert.rec.width, button_insert.rec.height, BLACK);
            DrawText("insertion  ", button_insert.rec.x + button_insert.rec.width / 2 - MeasureText("insertion ", 20) / 2,
            button_insert.rec.y + button_insert.rec.height / 2 - 20 / 2, 20, BLACK);
             
            DrawRectangleRec(button_supp.rec, button_supp.col);
            DrawRectangleLines(button_supp.rec.x, button_supp.rec.y, button_supp.rec.width, button_supp.rec.height, BLACK);
            DrawText("suppression  ", button_supp.rec.x + button_supp.rec.width / 2 - MeasureText("suppression ", 20) / 2,
            button_supp.rec.y + button_supp.rec.height / 2 - 20 / 2, 20, BLACK);
             
            DrawRectangleRec(button_new.rec, button_new.col);
            DrawRectangleLines(button_new.rec.x, button_new.rec.y, button_new.rec.width, button_new.rec.height, BLACK);
            DrawText("create  ", button_new.rec.x + button_new.rec.width / 2 - MeasureText("create ", 20) / 2,
            button_new.rec.y + button_new.rec.height / 2 - 20 / 2, 20, BLACK);
            
            DrawRectangleRec(button_search.rec, button_search.col);
            DrawRectangleLines(button_search.rec.x, button_search.rec.y, button_search.rec.width, button_search.rec.height, BLACK);
            DrawText("search  ", button_search.rec.x + button_search.rec.width / 2 - MeasureText("search ", 20) / 2,
            button_search.rec.y + button_search.rec.height / 2 - 20 / 2, 20, BLACK);
            
            
            
            start = true;
            
  
            EndTextureMode();          
        }
 
        //trie par selection 
        if (is_left_click_pressed(button_sort)){
            occ = false;
            aff=true;
            NoDel = false;
            step_trie = true;
            temp=head;
            
            after = temp;
            p = temp;
             before = temp;
            
        }
               
        //generate new
        if (is_left_click_pressed(button_new)){
            textSize = 0;

            inputComplete = false;
            enteredNumber = 0;
            memset(text, 0, sizeof(text));
            dataInputComplete = false;
            
            occ = false;
            
            NoDel = false; 

            currentNode = 1;
    
            nodeCount = 0;

            step2 = true;
            step5 = false;
        }
        
        //select search
        if (is_left_click_pressed(button_search)){
            
            
            if(temp != NULL)
                {
                    change_color(temp->prev,BLUE);
                }
                reset=getEnd(head);
                change_color(reset,BLUE); 
                
                
            step_search_val = true;
            start = false;
            step5 = false;
            temp = head;
            
            occ = false;
            aff=false;
            NoDel = false; 
          
            searchInputSize=0;
            searchInputComplete=false;
            memset(searchInput, 0, sizeof(searchInput));      

                
        }
        
        //input de la valeur a rechercher
        if(step_search_val){
          
            if (!searchInputComplete) {
                int key = GetKeyPressed();

                if ((key >= 48 && key <= 57) && (searchInputSize < 31)) {
                    searchInput[searchInputSize] = (char)key;
                    searchInputSize++;
                } 
                else if (key == KEY_BACKSPACE && searchInputSize > 0) {
                    searchInputSize--;
                    searchInput[searchInputSize] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && searchInputSize > 0) {
                    searchValue = atoi(searchInput);
                    searchInputComplete = true;
                }
            }
            else{           
                step_search_val = false;
                step_search = true;
                step5 = true;
            }

            BeginTextureMode(target);
            ClearBackground(GRAY);
    
            DrawRectangleRec(button.rec, button.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight ,   WHITE );

            DrawText(searchInput, textPosition.x, textPosition.y, 40, WHITE); // Display the entered digits

            if (!searchInputComplete) {
                DrawText("Enter the value to search:", SCREEN_WIDTH / 2 - MeasureText("Enter the value to search:", 20) / 2, SCREEN_HEIGHT / 2 - 100, 20, BLACK);
                DrawText("Press ENTER to confirm", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to confirm", 20) / 2, SCREEN_HEIGHT / 2 + 90, 20, BLACK);
            }
        
            EndTextureMode();
        }
        
        //Supprimer un element 
        if (is_left_click_pressed(button_supp)){ 
            textSize = 0;

            inputComplete = false;
            enteredNumber = 0;
            memset(text, 0, sizeof(text));
            dataInputComplete = false;
            
            
            aff=true;
            NoDel = false; 
            
            step5 = false;
            step_del = true;
        }
        
        //Ajouter un element 
        if (is_left_click_pressed(button_insert)){  
            textSize = 0;

            inputComplete = false;
            enteredNumber = 0;
            memset(text, 0, sizeof(text));
            dataInputComplete = false;
            
            
            aff=true;
            NoDel = false; 
            
            step5 = false;
            step_add = true;
        }
        
        //debut step_del 
        if(step_del){
            if (!inputComplete) {
                int key = GetKeyPressed(); // Get the pressed key 

                if ((key >= 48 && key <= 57) && (textSize < 63)) { // Check if the key is a number and within text limit
                    text[textSize] = (char)key; // Store entered digit in the text array
                    textSize++;
                }
                else if (key == KEY_BACKSPACE && textSize > 0) { // Check for backspace to delete the last digit
                    textSize--;
                    text[textSize] = '\0'; // Null-terminate the string to erase the last character
                }

                if (IsKeyPressed(KEY_ENTER) && (atoi(text) != 0)) { // Clicking on ENTER will finalize the input
                    inputComplete = true;
                    enteredNumber = atoi(text); // Convert the entered text to an integer
                }
            }
            
            BeginTextureMode(target);
            ClearBackground(GRAY);
            DrawRectangleRec(button.rec, button.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight , WHITE );

            DrawText(text, textPosition.x, textPosition.y, 40, WHITE); // Display the entered digits

            if (!inputComplete ) {
                DrawText("Enter the value of the element to add", SCREEN_WIDTH / 2 - MeasureText("Enter the value of the element to add", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight , 20, BLACK);
                DrawText("Click on the ENTER key to finalize input.", SCREEN_WIDTH / 2 - MeasureText("Click on the ENTER key to finalize input.", 20) / 2, SCREEN_HEIGHT / 2 + buttonHeight , 20, BLACK);
            }
            else{
                DrawText("Input Completed!", SCREEN_WIDTH / 2 - MeasureText("Input Completed!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 130, 20, BLACK);
                DrawText("press on the SPACEBAR for the next step.", SCREEN_WIDTH / 2 - MeasureText("press on the SPACEBAR for the next step!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 170, 20, BLACK);     
            }
            EndTextureMode();
            
            if(IsKeyPressed(KEY_SPACE) && inputComplete){
                if(isNumberInList(head,enteredNumber)){    
                    DeleteElement(&head,enteredNumber);                    
                }
                else{
                    NoDel = true; 
                }
                step_del = false;
                step5 = true;
            }
        }
        
        //debut step add
        if(step_add){
            if (!inputComplete) {
                int key = GetKeyPressed(); // Get the pressed key 

                if ((key >= 48 && key <= 57) && (textSize < 63)) { // Check if the key is a number and within text limit
                    text[textSize] = (char)key; // Store entered digit in the text array
                    textSize++;
                }
                else if (key == KEY_BACKSPACE && textSize > 0) { // Check for backspace to delete the last digit
                    textSize--;
                    text[textSize] = '\0'; // Null-terminate the string to erase the last character
                }

                if (IsKeyPressed(KEY_ENTER) && (atoi(text) != 0)) { // Clicking on ENTER will finalize the input
                    inputComplete = true;
                    enteredNumber = atoi(text); // Convert the entered text to an integer
                }
            }
            
            BeginTextureMode(target);
            ClearBackground(GRAY);
            DrawRectangleRec(button.rec, button.col);
            DrawRectangleLines(SCREEN_WIDTH / 2 - buttonWidth / 2 , SCREEN_HEIGHT / 2 - buttonHeight / 2 , buttonWidth , buttonHeight , WHITE );

            DrawText(text, textPosition.x, textPosition.y, 40, WHITE); // Display the entered digits

            if (!inputComplete ) {
                DrawText("Enter the value of the element to add", SCREEN_WIDTH / 2 - MeasureText("Enter the value of the element to add", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight , 20, BLACK);
                DrawText("Click on the ENTER key to finalize input.", SCREEN_WIDTH / 2 - MeasureText("Click on the ENTER key to finalize input.", 20) / 2, SCREEN_HEIGHT / 2 + buttonHeight , 20, BLACK);
            }
            else{
                DrawText("Input Completed!", SCREEN_WIDTH / 2 - MeasureText("Input Completed!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 130, 20, BLACK);
                DrawText("press on the SPACEBAR for the next step.", SCREEN_WIDTH / 2 - MeasureText("press on the SPACEBAR for the next step!", 20) / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2 + 170, 20, BLACK);     
            }
            EndTextureMode();
             
            if(IsKeyPressed(KEY_SPACE) && inputComplete ){
                insertAtHead(&head,enteredNumber);
                step_add = false;
                step5 = true;
            }
        }
          

        //debute main inter
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

        EndDrawing();
        
    }//end of the while loop 

    free(values);
    free(head);

    CloseWindow();
    return 0;
}
