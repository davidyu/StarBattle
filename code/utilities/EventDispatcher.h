#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct EventDispatcher EventDispatcher;
typedef bool (* EventDispatcherCallback)(void * sender, const char * eventID, void * eventData, void * context);

struct EventTarget;

#define EventDispatcher_structContents \
	void * owner; \
	\
	int numberOfTargets; \
	int targetListSize; \
	struct EventTarget * targets; \
	\
	void (* dispose)(void * self); \
	void (* registerForEvent)(void * self, const char * eventID, EventDispatcherCallback callback, void * context); \
	void (* unregisterForEvent)(void * self, const char * eventID, EventDispatcherCallback callback); \
	bool (* dispatchEvent)(void * self, const char * eventID, void * eventData);

struct EventDispatcher {
	EventDispatcher_structContents
};

EventDispatcher * EventDispatcher_create(void * owner);
void EventDispatcher_init(EventDispatcher * self, void * owner);

void EventDispatcher_dispose(void * selfPtr);
void EventDispatcher_registerForEvent(void * selfPtr, const char * eventID, EventDispatcherCallback callback, void * context);
void EventDispatcher_unregisterForEvent(void * selfPtr, const char * eventID, EventDispatcherCallback callback);
bool EventDispatcher_dispatchEvent(void * selfPtr, const char * eventID, void * eventData);

#endif

#ifdef __cplusplus
}
#endif