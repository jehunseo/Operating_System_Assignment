# Readme

## EDF / RM Implementation
+ app.c

	+ assign every task's start / period value

+ OS_TCB 

	+ add appropriate TCB variable for scheduling

+ OS_TimeTick()
	
	+ when called, update deadline and compute time

+ OS_SchedNew()

	+ implement EDF / RM scheduling : find most urgent task priority