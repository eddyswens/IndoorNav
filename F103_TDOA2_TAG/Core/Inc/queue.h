#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "uwb.h"

typedef struct QueueNode
{
    lpsLppShortPacket_t data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
    int size;    // Текущий размер очереди
    int maxSize; // Максимальный размер очереди
} Queue;

// Функция для создания нового узла очереди
QueueNode *createNode(lpsLppShortPacket_t data)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Функция для создания новой очереди с заданным максимальным размером
Queue *createQueue(int maxSize)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    queue->maxSize = maxSize; // Устанавливаем максимальный размер
    return queue;
}

// Функция для добавления элемента в очередь
bool enqueue(Queue* queue, lpsLppShortPacket_t *data) 
{
    // Проверка на переполнение очереди
    if (queue->size >= queue->maxSize) {
        // printf("Ошибка: очередь переполнена. Невозможно добавить элемент: %d\n", *data);
        return false;
    }
    lpsLppShortPacket_t temp = *data;

    QueueNode *newNode = createNode(temp);

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
    // printf("Добавлено в очередь: %d\n", data);
    return true;
}

// Функция для удаления элемента из очереди
bool dequeue(Queue *queue, lpsLppShortPacket_t *elem)
{
    if (queue->front == NULL)
    {
        // printf("Очередь пуста\n");
        return false;
    }

    QueueNode *temp = queue->front;
    lpsLppShortPacket_t data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;
    *elem = data;
    return true;
}

// Функция для проверки, пуста ли очередь
int isEmpty(Queue *queue)
{
    return queue->front == NULL;
}

// Функция для получения размера очереди
int size(Queue *queue)
{
    return queue->size;
}

// Функция для просмотра первого элемента в очереди без удаления
lpsLppShortPacket_t peek(Queue *queue)
{
    if (isEmpty(queue))
    {
        // printf("Очередь пуста\n");
        return; // Признак пустой очереди
    }
    return queue->front->data;
}

// Освобождение памяти очереди
void freeQueue(Queue *queue)
{
    lpsLppShortPacket_t temp;
    while (!isEmpty(queue))
    {
        dequeue(queue, &temp);
    }
    free(queue);
}

#endif // QUEUE_H