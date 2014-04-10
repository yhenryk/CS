#!/usr/local/cs/bin/python

"""
Output lines selected randomly from a set of files or a file. Can append the
-n option to specify how many lines from each file to be picked randomly. Can
also append -w so that a line can only be picked once. If the file is empty,
or the lines requested is more than the amount of new lines when using the
option -w, it will have an error. If n is 0 or negative, it will have an error.
If no file is supplied, an error message appears. If n is not specified, then
its default is one. If w is not specificed, then it will not ignore duplicates.

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename, argument):
        self.array = []
        self.size = 0
        for args in argument:
            #print self.array
            try:
                f = open(args, 'r')
                self.array.append(f.readlines())
                f.close()
                self.size=self.size+1
                #print self.size
            except IOError as e:
                sys.stdout.write("Error")
                #print 'Oh dear.'
    def chooseline(self, number, verbose):
        for index in range(self.size):
            if verbose == True:
                lines = random.sample(self.array[index], number)
                for repeat in range(number):
                    sys.stdout.write(lines[repeat])
            else:
                for second in range(number):
                    sys.stdout.write(random.choice(self.array[index]))
            #if duplicate == 1
            #else:
                #sys.stdout.write(random.choice(self.array[index]))
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)")
    parser.add_option("-w", "--without-replacement", action="store_true", 
            dest="verbose", default=False, help="no duplicates if activated")
                       
    options, args = parser.parse_args(sys.argv[1:])

    try:
        numlines = int(options.numlines)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    if len(args) < 1:
        parser.error("wrong number of operands")
    input_file = args[0]

    #try:
    try:
        generator = randline(input_file, args)
        #for index in range(numlines):
            #sys.stdout.write(generator.chooseline())
        generator.chooseline(numlines, options.verbose)
    except IOerror as why:
        msg = "Cannot open file: [%d] %s" % (why.errno, why.strerror)
        print(msg)
    #else:
        #try:
           # generator = randline(input_file, args)
           # generator.chooseline(numlines, options.verbose)
       # except IOError as (errno, strerror):
           # parser.error("I/O error({0}): {1}".
                         #format(errno, strerror))

if __name__ == "__main__":
    main()
