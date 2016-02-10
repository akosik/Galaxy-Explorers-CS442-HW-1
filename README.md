# Galaxy Explorers
### Erik and Alec 

File Decoder: Every File Contains 3 letters E for explorer, A-C-V for assembly-c-vectorized, and D-F for double-float.
(Disclaimer: All times are in nanoseconds.)
#### Part 1
##### A.
We picked 10,000 I (where I stands for iterations) for 2^8 to 2^11, 1000 I for 2^11 to 2^15, 200I for 2^16 to 2^19, then 20 for 2^19 to 2^24. We picked these iterations because as the size of the objects increases, the dataset will get bigger, so finding an accurate mean time per coordinate would require less iterations.  


##### B.
We ran input sizes of base 2 a total of ten times then found the mean of those ten runs. When receving the data, if one of the runs was 30 % slower or faster than the rest, it would be thrown out.


##### C.
![alt tag] (https://github.com/akosik/Galaxy-Explorers-CS442-HW-1/blob/master/Python.gif "Python Graph")


### Part 2
![alt tag](https://github.com/akosik/Galaxy-Explorers-CS442-HW-1/blob/master/Python%2BC.gif "Python and C Graph")


We were able to run more iterations for the C code than for Python because the C code was much faster.
(Disclaimer: To look at the graphs separately, look at ECF.gif and Python.gif in folder)


### Part 3
![alt tag](https://github.com/akosik/Galaxy-Explorers-CS442-HW-1/blob/master/ECD%2BECF.gif "C Float and Double Graph")

The float C function had about a 1 second advantage throughout all input sizes


### Part 4
![alt tag] (https://github.com/akosik/Galaxy-Explorers-CS442-HW-1/blob/master/Python%2BC%2Bassembly.gif "Python, C and Assembly Graph")
(Disclaimer: To look at the assembly graph separately, look at EAF.gif)


### Part 5


### Part 6

![alt tag] (https://github.com/akosik/Galaxy-Explorers-CS442-HW-1/blob/master/EVF.gif "Vector Graph")

For the vectorized assembly code, our program ran significantly faster than the regular assembly code until we got to inputs of size 2^19 and above, where the vectorized assembly still kept a small performance advantage. When looking at the double and float for the vectorized assembly code, we found that the float was faster than double by about 2x, as expected, since we can add twice as many floats at a time in the vector registers.



