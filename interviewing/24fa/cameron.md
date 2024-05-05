***************************
* Candidate: Cameron *
***************************
current freshman, took with tom
really liked class
Questions:
- Imagine you are given the power to make any changes to the course, what do you change? What do you keep the same about it?
  - If you haven't taken the course, you can scan the website from the last time I taught the course: https://www.seas.upenn.edu/~cis2400/22fa/ or you can describe an imaginary course at a high level
  - likes structure, buildup from transistors
  - hw was confusing in layout, struggle
  - hw content was interesting
  - like how it all fits together: c, asm, lc4
  - locally work on code and use debugger + gdb is better
    - clion

- Let's say a student comes to your office hours around half way through the semester and says they think they don't have what it takes to understand the material. That they have tried, but do not think they are the kind of person to "get it". What do you do?
  - have felt that way during the semester
  - eveyrone is finding this course challenging
  - what exactly is making you feel this way /q
  - could be something returning that you never worked thru in the first place
  - oh, ask questions, i would help
  - 

- Initiatives
  - hw writeup
  - ed help, bc he really found it valuable as a student
  - 

Debugging:
- start: 5:47
- reaffirm purpose of str dup
- can u walk me thru what youre currently doing
- read code, then asked for output when running
- issue with returning res /q
- when we're making a copy of it, we want to make sure that we are storing the duplicate in a way that the function calling would be able to access 
  - stack storage
  - we want to use something that would be accessible, since stack gets overwritten a lot
  - we want to use the heap to do that
  - heap is not overwritten until you free it at a later time
- important to add one to account for null terminated part of the string /q
- we'll have one more char in res to allocate before returning after the string, how do we terminate it
  - lets the student come up with way to do it
- now getting blank output 
- we can try running 
- we might need to fix second part of code in first function
- issue with merge arrays; something to do with
- look at second function (merge)
- this is a similar thing going on here, res array is being stored in the stack, we can change it in a similar way /q
- if int is not one byte, so it may not allocate enough memory, so we need to multiply by sizeof(int)
- we hav e 2 arr of integers, c isnt able to go to index we want it to
- since int is not just one byte
- allocate for sizeof int* (instead of allocating len*sizeof(int))

Questions:
- oh on zoom? (farmer does zoom oh)
- when hear back?
- 



Other Notes:
- 


Ratings [1-5]:
knowledge       - 
organization    - 
confidence      - 
charisma        - 
empathy         - 
passion teach   - 
passion material- 