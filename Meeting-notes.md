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

# Meeting 20.11.2024 16:00-17:30

**Participants**:

1. Ukko Ihamäki
2. Daniel Michaeli
3. Kasper Tulonen
4. Robin Runne

## Summary of works

1. Daniel Michaeli

   Created some, but not all of the GUI classes, incorporate into Ukkos main app

2. Ukko Ihamäki

   No larger changes since last meeting (Expanded the simple QT main app window into a working mvp, with functionality for selecting game mode, settings, starting game and quitting the app)

3. Kasper Tulonen

   No larger changes since the last meeting (Implemented basic program classes for deck, piles and card)

4. Robin Runne

   Started implementing game class utilizing Kasper's logical classes. Ran into some ambiguity later on.

## Challenges

1. Some tasks were too ambiguous and large to fully complete without further communication between the team
2. Logical + GUI classes seems unnecessarily complex, especially regarding pointers and ownership mgmt

## Actions

1. Squash logical and gui classes into one. Class is mainly a QT gui-type, but contains all logical functionality as well. Kasper will start on this, and others will pick up
2. We still need to implement unit testing. Daniel will take care of finding and installing a relevant library
3. After having reconciled the squashing and everyone understanding it, we can continue working on new tasks with this as a basis
4. Tasks need to be smaller and more detailed to easily collaborate and finish them

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

A working app instance is created, with basic implementation of all the logical classes and some gui classes. Some redesigns are in plan, and will be implemented in this sprint (mentioned above). Simple GUI will remain more or less as is.

# Meeting 30.11.2024 16:00-17:30

**Participants**:

1. Ukko Ihamäki
2. Daniel Michaeli
3. Kasper Tulonen

## Summary of works

1. Daniel Michaeli

   Installed Catch2 unit testing library, and added tests to all classes

2. Ukko Ihamäki

   Working on animations, sounds and other GUI related tasks in QT Creator. Has ran into some problems but we'll fix those later

3. Kasper Tulonen

   Squashed all logical and gui classes into one. Some bug fixed and worked on connecting classes in game using signals and slots (QT)

4. Robin Runne

   Worked on Game Class

## Challenges

1. We had problems of objects knowing about each other when actions are taken, e.g. mouse press events. We try to solve this by implementing QTs signals and slots mechanisms, instead of having back and forth references from classes to each other, which would be unnecessarily complex.

2. Some issues with adding audio to GUI.

3. We are in a slight hurry atm. We need to prioritize getting the game class and gui card movements done to get a working simple version of the game. Only then can we start tweakin and fine tuning.

## Actions

1. Work on game class function that moves cards. After this we should more or less have a working game
2. Make sure selecting a face-up card moves the entire face-up sub-pile. Can't move a single card if other face up cards are on top of it.
3. Other tasks: fix failing unit tests due to changes, statistics, animations, easy/hard mode, et.c.

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

Most of the important pieces are created, but some functionality still missing for a proper working game. This should be fixed ASAP, and only then can we start making things pretty.
