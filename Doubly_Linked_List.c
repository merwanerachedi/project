#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define REC_HEIGTH 50
#define REC_WIDTH 80
#define ARROW_LENGTH 30

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
    return head;
}



void drawNode(struct Node* node, Vector2 pos) {
    DrawRectangle((int)pos.x - REC_WIDTH, (int)pos.y -  REC_HEIGTH, REC_WIDTH, REC_HEIGTH, GREEN);
    DrawText(TextFormat("%d", node->data), (int)pos.x-50 , (int)pos.y-35 , 20, BLACK);
}


void drawArrow(Vector2 start, Vector2 end) {
    DrawLineV(start, end, BLACK);
    ;
  
}

void visualizeDoublyLinkedList(struct Node* head) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Doubly Linked List Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int count = 0;
        struct Node* current = head;
        Vector2 nodePosition = {100, SCREEN_HEIGHT / 2};

        while (current != NULL) {
            drawNode(current, nodePosition);

            if (current->next != NULL) {
            drawArrow((Vector2){nodePosition.x,nodePosition.y-35 }, (Vector2){nodePosition.x + REC_WIDTH * 2  + ARROW_LENGTH , nodePosition.y-35});
            Vector2 end = (Vector2){nodePosition.x + REC_WIDTH * 2  + ARROW_LENGTH , nodePosition.y-35};
            DrawTriangle(end, (Vector2){end.x -5, end.y - 5}, (Vector2){end.x - 5, end.y + 5}, BLACK);
            }
            if (current->prev != NULL) {
                drawArrow((Vector2){nodePosition.x - REC_WIDTH, nodePosition.y-15}, (Vector2){nodePosition.x - REC_WIDTH * 3 - ARROW_LENGTH + 2, nodePosition.y-15});
                Vector2 end = (Vector2){nodePosition.x - REC_WIDTH * 3 - ARROW_LENGTH + 2, nodePosition.y-15};
                DrawTriangle(end, (Vector2){end.x +5, end.y + 5}, (Vector2){end.x + 5, end.y - 5}, BLACK);
            }

            nodePosition.x += REC_WIDTH * 3 + ARROW_LENGTH;
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
