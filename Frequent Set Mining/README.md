
# Frequent Itemset Mining

This project is an implimentation of [Apriori](https://en.wikipedia.org/wiki/Apriori_algorithm) and [Fptree](https://www.geeksforgeeks.org/ml-frequent-pattern-growth-algorithm/) algorithms to mine frequent itemsets. \
All the items are assumed to be integers.


## References

 - [Mining Frequent Patterns without Candidate Generation - Jiawei Han, Jian Pei, and Yiwen Yin. 2000](https://www.philippe-fournier-viger.com/spmf/fpgrowth_04.pdf)
 - [Frequent Patterns without Candidate Generation](https://www.cs.sfu.ca/~jpei/publications/sigmod00.pdf)


## Run Locally

Clone the project

```bash
  git clone https://github.com/cs5190435/Data-Mining
```

Go to the project directory

```bash
  cd Data-Mining/Frequent Set Mining/
```

Execute the bash script.

```bash
  ./2019CS50435.sh -apriori <dataset-name> X <output filename>
  ./2019CS50435.sh -fptree <dataset-name> X <output filename>

```
- X is the threshold value . It is percentage and not the actual count. 
- dataset-name is the input file 
- output filename is the output file in which all the Frequent itemsets are written in [lexicographical](https://en.wikipedia.org/wiki/Lexicographic_order) order 



