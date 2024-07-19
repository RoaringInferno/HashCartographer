# HashCartographer
A program to brute force hash functions for closed sets of strings to a specified operation depth, using bitwise operators.
## Usage
Arguments will be treated as strings for the program to parse.

In order to pass arguments with leading ```-```, escape it with a ```\```. For example, ```\--depth``` would add an argument called ```--depth```.
### Options
#### ```--depth = 1```
Adjusts the search depth of the program. Runtime increases exponentially with depth increase.

Personal benchmark:
On a MacBook Pro 8 core M1, 7 seconds for a ```depth = 2``` search for a hash for the months of the year. ```depth = 1``` runs in negligible time.
## Output
The program will output as follows:
```
For a closed set of strings:
- <lists arguments>
- ...
- ...
- etc.
Fastest: <formula> with opcount <x> and tightness <x> has hash table: <xx> <xx> <xxx> etc.
Tightest: <formula> with opcount <x> and tightness <x> has hash table: <xx> <xx> <xxx> etc.
```
