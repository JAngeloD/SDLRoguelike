#include "messagelog.h"

Queue::Queue() {}

void Queue::loadQueue(int c) {
    front = rear = 0;
    capacity = c;
    queue = new message[capacity];
}

void Queue::dequeue() {
    // if queue is empty
    if (front == rear) {
        printf("\nQueue is  empty\n");
        return;
    }

    // shift all the elements from index 2 till rear
    // to the left by one
    else {
        for (int i = 0; i < rear - 1; i++) {
            queue[i] = queue[i + 1];
        }

        // decrement rear
        rear--;
    }
    return;
}

void Queue::removeBack() {
    rear--;
}

void Queue::enqueue(message text) {
    // check queue is full or not
    if (capacity == rear) {
        dequeue();
    }
    // insert element at the rear
    queue[rear] = text;
    rear++;
    return;
}

messagelog::messagelog() {}

messagelog::~messagelog()
{
    destroyInterface();
}

void messagelog::loadInterface(std::string fontPath, SDL_Rect interfaceArea) {
    messageFont = TTF_OpenFont( fontPath.c_str(), 32);
    messageQueue = new Queue();
    messageQueue->loadQueue(interfaceArea.h / 30);
}

void messagelog::addMessage(std::string message, Uint8 r,Uint8 g, Uint8 b ) {
    struct message newMessage;
    SDL_Color textColor = {r, g, b, 255};
    newMessage.loadmessage(message, textColor, this->messageFont);
    messageQueue->enqueue(newMessage);

}

void messagelog::wrapAround(message wrap, SDL_Rect interfaceArea, int indentation) {
    message messageBuffer = wrap;
    int maxLength = (int)(interfaceArea.w / textWidth) - indentation;
    SDL_Color color = {255,255,255,255};

    messageQueue->removeBack();
    for (int i = 0; i < wrap.messageLength; i += maxLength) {
        message newMessage;
        newMessage.loadmessage(wrap.text.substr(i,maxLength), color, this->messageFont);
        messageQueue->enqueue(newMessage);
    }
}

void messagelog::draw(SDL_Rect interfaceArea) {

    struct message* drawQueue = &messageQueue->getMessages();
    int indentation = interfaceArea.w / 16;
    int startingPoint = interfaceArea.y + (interfaceArea.h - interfaceArea.h / 10);
    this->textWidth = interfaceArea.w / 36;
    this->textHeight = interfaceArea.h / 8;

    for( int i = 0; i < messageQueue->rear; i++) {
        if(drawQueue[messageQueue->rear - 1 - i].messageLength > (interfaceArea.w / textWidth) - (indentation/textWidth)) {
            wrapAround(drawQueue[messageQueue->rear - 1 - i], interfaceArea, indentation/textWidth);
        }
        drawQueue[messageQueue->rear - 1 - i].renderMessage(interfaceArea.x + indentation, //x
                                                           (startingPoint - textHeight) - (i * textHeight), //y
                                                            drawQueue[messageQueue->rear - 1 - i].messageLength * textWidth, //w
                                                            textHeight); //h
    }

}

void messagelog::destroyOldMessages() {
    messageQueue->removeBack();
}

void messagelog::destroyInterface() {
    TTF_CloseFont(messageFont);
}
