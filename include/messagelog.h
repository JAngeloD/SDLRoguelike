#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <string>

#include "game.h"
#include "message.h"

class message;

struct Queue {
    int front, rear, capacity;
    message* queue;
    Queue();
    void loadQueue(int c);
    ~Queue() {//delete[] queue;
    }
    // function to insert an element
    // at the rear of the queue
    void enqueue(message text);

    void removeBack();
    // function to delete an element
    // from the front of the queue
    void dequeue();
    message& getMessages() {return *queue;}
};

class messagelog
{
    public:
        messagelog();
        ~messagelog();

        void loadInterface(std::string fontPath, SDL_Rect interfaceArea);
        void addMessage(std::string message, Uint8 r,Uint8 g, Uint8 b); //Adds message to queue
        void draw(SDL_Rect interfaceArea); //Draws all messages in a queue
        void wrapAround(message wrap, SDL_Rect interfaceArea, int indentation);
        void destroyInterface();

        void destroyOldMessages();

    private:
        TTF_Font* messageFont;
        int textHeight = 32;
        int textWidth = 16;
        struct Queue* messageQueue;

};

#endif // MESSAGELOG_H
