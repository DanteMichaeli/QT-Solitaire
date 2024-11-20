# Meeting Notes

In this file, you are required to take notes for your weekly meetings.
In each meeting, you are required to discuss:

1. What each member has done during the week?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next week for everyone
4. Deviations and changes to the project plan, if any

# Meeting dd.mm.2021 HH::MM

**Participants**:

1. Member 1
2. Member 2
3. Member 3
4. Member 4

## Summary of works

1. Member 1

   Implementing the class XX. Tested the class XX.
   Results are in `tests/<class-xx-tests>`. Resolved the identified problems.

2. Member 2

   Same as above

3. ...

## Challenges

1. The integration of UI with the monsters requires an abstract interface.
2. ...

## Actions

1. Member 1 is going to look into defining an abstract interface for monsters
   to enable easy UI integration.
2. Member 2 is going to work with Member 1 to use abstract interface in derived
   monster classes.
3. Member 2 is going to test the interface.
4. Member 3 is going to use ...

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

Short summary of current project status.

### TODOs

1. Member 1: Write an action.
2. ...

---

# Meeting 30.10.2024 12-15

**Participants**:

1. Ukko Ihamäki
2. Daniel Michaeli
3. Kasper Tulonen

## Summary of works

1. Daniel Michaeli

   Set up dev env and written instructions on how to set up for devs and running project

2. Ukko Ihamäki

   Been busy, but catching up on project

3. Kasper Tulonen

   Brainstormed the UML / logical structure of the program

## Challenges

1. The UML is a bit lacking in structure, plan to fix this before sprint DL to get it into the project plan
2. The dev env setup instructions are not fully working for other devs, need to find alternative solutions for some installations

## Actions

1. The UML will not be further modified during this sprint, and the current version will go into the project plan
2. The team will decide two members to finalize the UML digitally and write the project plan
3. The rest of the members will try to set up the dev env.

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

Dev env is set up for some, but not all developers. Initial UML is done. Project plan is not written yet, DL this Friday!

---

# Meeting 9.11.2024 19:00-19:20

**Participants**:

1. Ukko Ihamäki
2. Daniel Michaeli
3. Kasper Tulonen
4. Robin Runne

## Summary of works

1. Daniel Michaeli

   Has been busy, but will catch up with the QT gui class implementations next week

2. Ukko Ihamäki

   Expanded the simple QT main app window into a working mvp, with functionality for selecting game mode, settings, starting game and quitting the app

3. Kasper Tulonen

   Implemented basic program classes for deck, piles and card

4. Robin Runne

   Has been busy, will catch up on implementing the Game class next week, utilizing the classes Kasper has created

## Challenges

1. Nothing much, quite smooth sailing thus far!

## Actions

1. Daniel will expand on the QT functionality by adding graphical representations of each class
2. Robin will create the game class utilizing the card, deck and piles
3. We will have to implement some testing library to start writing unit tests. How to fix Valgrind for mac???

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

A working app instance is created, with basic implementation of the first classes. We are lacking in unit testing and have to fix that ASAP, including the problem of valgrinding with mac.
