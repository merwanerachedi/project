#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

struct Node* createDoublyLinkedList(int size) {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    printf("Enter %d elements for the doubly linked list:\n", size);
    for (int i = 0; i < size; ++i) {
        int data;
        printf("Enter element %d: ", i + 1);
        scanf("%d", &data);

        struct Node* newNode = createNode(data);

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

void drawNode(struct Node* node, Vector2 pos, int nodeCount) {
    int rectWidth = SCREEN_WIDTH / (nodeCount * 2);
    int rectHeight = SCREEN_HEIGHT / 20;

    DrawRectangle((int)pos.x - rectWidth / 2, (int)pos.y - rectHeight / 2, rectWidth, rectHeight, BLUE);
    DrawText(TextFormat("%d", node->data), (int)pos.x - MeasureText(TextFormat("%d", node->data), 20) / 2, (int)pos.y - 10, 20, BLACK);
}

void drawArrow(Vector2 start, Vector2 end) {
    DrawLineV(start, end, BLACK);

   // Vector2 arrow[3] = { {end.x, end.y}, {end.x - 5, end.y - 5}, {end.x + 5, end.y - 5} };
    //DrawTriangleLines(arrow[0], arrow[1], arrow[2], BLACK);
}
  

void visualizeDoublyLinkedList(struct Node* head) {
    int nodeCount = 0;
    struct Node* temp = head;

    while (temp != NULL) {
        nodeCount++;
        temp = temp->next;
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Doubly Linked List Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

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

        EndDrawing();
    }

    CloseWindow();
}

void freeDoublyLinkedList(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int size;
    printf("Enter the size of the doubly linked list: ");
    scanf("%d", &size);

    struct Node* head = createDoublyLinkedList(size);

    visualizeDoublyLinkedList(head);

    freeDoublyLinkedList(head);

    return 0;
}
