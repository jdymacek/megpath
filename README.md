# megpath

Arguments:
-------------------------------------------------------------

|Argument |Description				             |Example			                            |
|---------|:------------------------------------------------:|------------------------------------------------------|
|analysis | specifies which heading to follow                | analysis = **"one"**                                 |
|filename | the name of the csv file we pass in              | filename = **"test.csv"**                            |
|origin   | the point in the csv where data starts           | origin = *[**0,0**]*                                 |
|directory| the directory where results will be written      | directory = **"../testRun/"**                        |
|patterns | holds the names of patterns to specify           | patterns = *[**"pattern_one","pattern_two", ""**]*   |
|max_runs | the maximum number of times to run               | max_runs = **40000**                                 |
|controls | the control columns that can be compared against | controls = *[**0,1,2,3**]*                           |
|columns  | the columns to compare againt the controls       | columns = *[**4,5,6,7**]*                            |

- Notes:
  * Controls and columns must be the same size.

Running the Program
-------------------------------------------------------------
- Standard
	* ./connmf	"*argument file*"

- Distributed
	* ./run.sh	"*argument file*"	"*version to run*"	"*number of hosts*"
	
- Versions
	* ParallelPatterns	(**pp**)	
	* FuncThrow	(**ft**)
	* DistNaive	(**dn**)

- GSEA
	* ./gsea	"*argument file*"
	* Argument file requires two arguments:	coefficient_file, genome_file
	
Compiling
-------------------------------------------------------------
- The software can be compiled using the command "*make all*" and subsequently cleaned with "*make clean*".
