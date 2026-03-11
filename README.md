This project has been created as part of the 42 curriculum by *relaforg*

# Codexion

## Description

The **Codexion** project introduce us to thread programming and its challenges.
Given x coders and x dongles, each coders need to execute y compile cycle.
A cycle consists in compilling, debugging and refactoring. In order to compile,
a coder need to have access to 2 dongles.

## Instructions

To compile:

```bash
make
```

To execute:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug>
<time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

## Resources

- man
- claude

## Blocking cases handled

- Each dongle can only be held by one coder
- If a coder cannot hold 2 dongles, he does not hol any
- Dongles are realeased after compilation
- If a coder's neighboor has priority over the coder but cannot compile,
  it does not block the coder.
- Two scheduler
  - FIFO: The first to wait is the first to get access
  - EDF: The coder closest to burnout get access first
- Last used timing is saved in dongle struct to furtherly check the cooldown
- Last compile timing is saved in the coder's thread
- Coders do not use printf at all, they add a message with timing to a log_queue
  the monitor thread after will dequeue each message in order and print

## Thread synchronization mechanisms

- Each shared data is protected via a mutex (pthread_mutex_t)
- Each coder try to access the dongle pool, then check if he has the priority,
  then if his 2 dongles are available. And at last if all previuos checks are ok,
  launch compile cycle
