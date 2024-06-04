Hello!

Tom, I know you apologized for being a bit late in response but here I am not responding till about two months later lol.
Sorry for that, I just got busy with spring Semester. The whole semester I was kind of behind since I did not prepare well enough over winter "break". I'm trying to implement a lot over the summer to make my time during the year less painful.

As for the topics:

# RISC-V

I talked with Joe devietti about his expereince with it in CIS 4710 and he said it went well. Of course there were some hiccups but that happens when most things change. Next offering will be smoother and he plans to stick with RISC-V. He was able to use a subset of RISC-V that fits on the front of a page and half the back. Link here: [] this is including things like Memory fences, several variations on multiply, divide, load, store, and a lot of pseudo instructions. Of course it will still be more complex than LC4 (even just looking at how it is a 32-bit encoding for each instruction instead of LC4's 16-bit), but it shouldn't be too much more complex.

To be fair, I don't think the advantadges of moving to RISC-V are THAT strong, though there are some:
- If it is "real" then they can find more documentaiton on it (though this also menas it may be easier to cheat with it)
  - I use godbolt/compiler-explorer a good bit and there are options to generate RISC-V code. It would be nice to show off this tool as it is especially useful for talking to people who care about performance in C/C++ code
  - We can probably take advantadge of some real tools like gcc or clang cross compilers. RISC-V is modular enough that we can make sure that the asm these compilers generate stays mostly in the ASM we use in the course (as long as we use our code correctly, e.g. if we don't want to 16-bit store then don't use the `short` data type in the C code). 
  - It is something that students can put on their resume and not get laughed at
- It helps prep them for 5710 and 5710 will also re-inforce their understanding of RISC-V
  - I also know that two other courses use assembly: CIS 3410 (compilers) and CIS 5510 (Computer & Network Security). Though I beleive both use a subset of x86 (x86-Lite)

I think I am going to try and pivot to RISC-V for Fall, but I will fall back on LC4 if it doesn't look like I can't get it all ready over the summer.

# C

I think I feel pretty strongly about having more C in CIS 2400. We don't have to have the students be masters at C, but I have had several students spring semester in CIS 3800 that do not recall ever calling malloc (and they had taken CIS 2400 in the last year). I'm sure that you both did talk about malloc and had them use it, but I think we should probably have more C (and probably somewhat more complex C) in the course so that they better know C. I also know that it is not my just my course that expects people leave CIS 2400 knowing C, from talking to students and some other faculty CIS 4600, CIS 5050, CIS 1901 and others expect them to be more familiar with C and dynamic memory.

# Making Space for more C

Related to the previous point on C programming, I talked to a lot of students and some faculty to try and get a better idea of what other courses are expecting students to learn in CIS 2400. This includes CIS 4710, CIS 5510, CIS 3800, CIS 4600, ESE 3500, CIS 1901, and others.

The list of topics other courses expect to be covered in CIS 2400 seem to include:
- C
  - The C memory model!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    - dynamic memory allocation
    - the stack
  - pointers!!!!!!!!!!!!!!!!!!!!1
  - bits, bytes & data representation
  - dealing with a relatively unsafe programming language
- Digital Logic:
  - Logic gates
  - muxes & demuxes
  - incrementer
  - timing diagrams, delay, clocks
  - notion of hardware being inherently "parallel"
- ASM
  - some familiarity with an assembly language
  - RISC-V or x86 would be nice depending on the course

I think we cover all of this, but I konw from my course (and talking to other courses) that they could use more C programming practice. I want to make room for this.

I took a look through the topics that were not mentioned above but covered in the course and that includes:
- Transistors & CMOS (especially for the EE people, ESE 2670 goes over all this and more)
- Latches & Flip Flops, though these are useful for understanding the gates and timing better
- the OS and I/O. The amount we talk about the OS currently in the course is so minimal that I can't use it much in 3800 and students do not remember it much. 
