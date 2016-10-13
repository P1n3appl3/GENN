import timeit

def test():
# Put test code here
    print "test"

#Runs 1000000 itterations
print timeit.timeit("test()", setup="from __main__ import test")
