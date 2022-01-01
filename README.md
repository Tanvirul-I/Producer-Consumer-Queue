# Producer-Consumer-Queue

A producer-consumer queue that implements a semaphore using mutexes and condition variables.

The implementation uses POSIX Mutexes to create a producer-consumer queue that adds items if the queue has enough room, locking the queue until room is available. Similarly, if a consumer requests an item, it will attempt to return an item, if the queue is empty it will lock the queue until an item is added.
