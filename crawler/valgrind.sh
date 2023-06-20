# This is in addition to testing.sh and provides all required testing along
# (letters at depths 0,10, toscrape at depths 0,1, wikipedia at depths 0,1)

# make directories
mkdir ../data
mkdir ../data/val_letters10 ../data/val_toscrape1 ../data/val_wikipedia0 ../data/val_wikipedia1

# make binary files
make

#valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/val_letters10 10
#valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/val_toscrape1 1
#valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/val_wikipedia0 0
#valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/val_wikipedia1 1
