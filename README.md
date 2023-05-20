# sort
Exercise from K&amp;R 5.14-5.17  with the intent of going fully in, with 5.17 defining the start structure.

## Details

It is a toy sorting program (newbie project) compared
to any serious sort program, but it does handle:

* lexiographic
* dictionary, 
* numeric
* case insensitive
* reverse sort,

With individual sorting of upto 255 fields, with the option
of setting "global" values, which any field specified
without further options will use.

The field handling is naive, without any way to set an
offset within the fields.

The  comparsion functions are fully  dedicated to remove
any uneccessary if-tests within the comparision. On the
other hand, there is just one set of comparison function
that handles both lines and individual fields.


## Motivation: 

	Main motivation is the belief in doing the exerceises  of
	K&R as something utterly important if I am to continue
	with C-programming, which I will  for a while.

	Secondly, I had some ideas/concepts I want to try for
	speeding up  the sorting on fields with as little overhead
	as possible.


## Result

Input is prepared into columns but only for as many fields
as necessary, and reassmbled again after sorting.

The cost of sorting on a field, is a quicksort algorithm
with two more parameters, and a 3 more assignement
instructions in the swap routine, which I count for as a
success efficency wise, in comparision with other approaches
that involves extracting the fields from within quicksort,
where I at best would need two calls to field extraction
routine, for two lines, maybe multiple times per line during
sorting.


### Missing parts:

	As it stands and is left, it is still a toy, since I
	haven't seen any reason for incorporating:

* Fast io and support for large files: (mmap) for files, and
maybe fread for streams.

* A merge sort algorithm for sorting with stability with
respect to sorting on multiple fields.  As quicksort
doesn't really make much sense when adjacent rows should
be kept adjacent.


### Robustness:

The program as such is fairly robust with regards to input,
lines with missing fields for sorting are promptly printed
to stderr and rejected from the input to the sorting.
