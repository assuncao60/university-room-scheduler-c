# University Room & Event Scheduler

A robust command-line application developed in C for managing university room bookings, scheduling events, and preventing time-space conflicts.

## Key Features

* **Modular Architecture (ADT):** Built using strict Abstract Data Types (ADTs) with clean encapsulation, preventing any data leakage between modules.
* **Dynamic Memory Management:** Leverages custom dynamic structures with automated resizing to seamlessly handle growing data demands without system crashes.
* **Advanced Queue Management:** Implements dynamic Queue structures linked via nodes to handle events within individual rooms efficiently.
* **Conflict Detection System:** A multi-layered algorithmic check that automatically detects and rejects overlapping events by evaluating both room availability and professor schedules.

## Technologies & Concepts Used

* **Language:** C
* **Data Structures:** Dynamic Queues, Linked Lists, Pointers, and Dynamic Tables.
* **Algorithms:** Insertion Sort (for global event ordering by chronological and alphabetical priority) and Custom DateTime Arithmetic.

## How to Test

Since this is a command-line application, you can easily compile and test it using any standard C compiler (like GCC) via your terminal.
