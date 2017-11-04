#!/usr/bin/env python3

# Copyright 2017 Will Mitchell
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import logging
from optparse import OptionParser
import os
import re
import sys

def skip_lines(file, num):
    """Reads and ignores num lines from file"""
    for i in range(num):
        file.readline()

def read_density(file):
    """Reads an attribute and its density"""
    log = logging.getLogger('read_density')

    name = file.readline().strip().rstrip(":")
    log.debug("Read density for '%s'", name)
    skip_lines(file, 2)
    vals = file.readline().strip().split(" ")
    count = file.readline().strip().split(" ")
    data = [x for x in zip(vals, count)]

    return (name, data)

def merge_correct_incorrect_total(correct, incorrect, total):
    """Merges the three lists of tuples and verifies the total"""
    log = logging.getLogger('read_density')

    merged = {}

    # Merge
    for x, y in total[1]:
        merged[x] = [0, 0, y]
    for x, y in correct[1]:
        if x != "":
            merged[x][0] = y
    for x, y in incorrect[1]:
        if x != "":
            merged[x][1] = y

    # Verify
    sum = 0
    for k in merged:
        v = merged[k]
        sum += int(v[2])
        if int(v[0]) + int(v[1]) != int(v[2]):
            log.error("Invalid totals for set size %d: %s + %s != %s", k, v[0], v[1], v[2])

    return sum, merged

def read_file(filename):
    """Read the file into a huge dict"""
    log = logging.getLogger('read_file')

    log.info("Processing file '%s'", filename)
    with open(filename) as file:
        skip_lines(file, 5)

        m = int(re.search(r"Size \(m\) = (\d+)", file.readline()).group(1))
        hash_setup = file.readline()
        k = int(re.search(r"k = (\d)", hash_setup).group(1))
        insert_policy = file.readline()

        skip_lines(file, 6)

        alphabet = re.search(r"Using alphabet: (.*)", file.readline()).group(1)
        filters = file.readline().rstrip()

        log.debug("m = %d, k = %d alphabet = '%s'", m, k, alphabet)
        log.debug("filters: '%s'", filters)

        # Now have all information about BF setup
        setup = (m, k, hash_setup, insert_policy, alphabet, filters)

        # Skip lines until line "Complete. Total of ## lines." which is printed
        # when all tasks are complete
        num_lines = 0
        while True:
            line = file.readline()
            if re.match(r"Complete. Total of \d+ lines.", line) != None:
                num_lines = int(re.search(r"Complete. Total of (\d+) lines.", line).group(1))
                break

        log.debug("Total lines: %d", num_lines)

        # Read in info about Bloom filter
        skip_lines(file, 2)
        if num_lines != int(re.search(r"Total trials: (\d+)", file.readline()).group(1)):
            log.error("Num of total lines does not match number of trials")
        skip_lines(file, 3)
        bf_bits_set = read_density(file)
        bf_est_elts = read_density(file)

        # Read graph stats
        skip_lines(file, 3)
        graph_vertices_all = read_density(file)
        graph_vertices_real = read_density(file)
        graph_vertices_false = read_density(file)
        graph_edges = read_density(file)

        # Read DFS
        skip_lines(file, 3)
        total = read_density(file)
        correct = read_density(file)
        incorrect = read_density(file)
        skip_lines(file, 1)
        trials, dfs = merge_correct_incorrect_total(correct, incorrect, total)
        dfs_missed = file.readline().strip().split(" ")

        if trials != num_lines:
            log.error("Total trials incorrect for DFS. Found %d expected %d", trials, num_lines)

        # Read simple paths
        skip_lines(file, 3)
        total = read_density(file)
        correct = read_density(file)
        incorrect = read_density(file)
        skip_lines(file, 1)
        trials, simple = merge_correct_incorrect_total(correct, incorrect, total)
        simple_missed = file.readline().strip().split(" ")

        if trials != num_lines:
            log.error("Total trials incorrect for simple. Found %d expected %d", trials, num_lines)

        # Read edge-disjoint
        skip_lines(file, 3)
        total = read_density(file)
        correct = read_density(file)
        incorrect = read_density(file)
        skip_lines(file, 1)
        trials, edge = merge_correct_incorrect_total(correct, incorrect, total)
        edge_missed = file.readline().strip().split(" ")

        if trials != num_lines:
            log.error("Total trials incorrect for edge. Found %d expected %d", trials, num_lines)

        return setup, dfs, dfs_missed, simple, simple_missed, edge, edge_missed

def merge_data(data):
    log = logging.getLogger('merge_data')

    merged = [[], {}, [], {}, [], {}, []]

    # Setup info already ensured to be same
    merged[0] = data[0][0]

    for x in data:
        # Merge word lists
        for i in [2, 4, 6]:
            merged[i] += x[i]

        # Merge the dicts
        for i in [1, 3, 5]:
            for k in x[i]:
                if not k in merged[i]:
                    merged[i][k] = x[i][k]
                else:
                    merged[i][k] = [int(a) + int(b) for a,b in zip(merged[i][k], x[i][k])]

    return merged

def write_single_file(data, filename, aux):
    log = logging.getLogger('write_single_file')

    log.debug("Writing to '%s'", filename)
    with open(filename, 'w') as outfile:
        for set_size in sorted(data):
            print(aux + ", ".join([str(set_size)] + [str(x) for x in data[set_size]]), file=outfile)


def write_files(data, stem, aux):
    log = logging.getLogger('write_files')

    setup, dfs, dfs_missed, simple, simple_missed, edge, edge_missed = data

    out_filename = stem + "-dfs.csv"
    log.debug("Will write DFS stats to '%s'", out_filename)
    write_single_file(dfs, out_filename, aux)

    out_filename = stem + "-simple.csv"
    log.debug("Will write simple path stats to '%s'", out_filename)
    write_single_file(simple, out_filename, aux)

    out_filename = stem + "-edge.csv"
    log.debug("Will write edge-disjoint path stats to '%s'", out_filename)
    write_single_file(edge, out_filename, aux)


def process_cmd_line(argv):
    """Process the command line options."""
    parser = OptionParser()
    parser.set_usage("%prog [OPTIONS] [FILE(S)]")
    parser.set_defaults(quiet=False)
    parser.set_defaults(verbose=0)

    parser.add_option("-a", "--add-aux", dest="aux", type="string",
                      help="STRING to prepend to each line of output. [default '']",
                      metavar="STRING")
    parser.add_option("-l", "--log", dest="log_filename", type="string",
                      help="FILE to write log entries to. [default stderr]",
                      metavar="FILE")
    parser.add_option("-s", "--stem", dest="out_stem", type="string",
                      help="Base stem to use for csv output [required]",
                      metavar="STEM")
    parser.add_option("-v", "--verbose", action="count", dest="verbose",
                      help="include informational output [default]")
    parser.add_option("-q", "--quiet",  action="store_false", dest="quiet",
                      help="suppress most output")

    (options, args) = parser.parse_args(argv)

    # Not enough specified, print usage and quit
    if len(args) < 1 or options.out_stem is None:
        parser.print_help()
        sys.exit(1)

    # Determine how noisy to be
    log_level = logging.WARNING # default
    if options.quiet:
        log_level = logging.CRITICAL
    elif options.verbose == 0:
        log_level = logging.INFO
    elif options.verbose >= 1:
        log_level = logging.DEBUG

    # DEBUG messages and higher get passed on to individual handlers
    logging.getLogger('').setLevel(logging.DEBUG)

    # Setup the console handler, writes to sys.stderr
    chandler = logging.StreamHandler()
    chandler.setLevel(log_level)
    cformatter = logging.Formatter('%(name)s: %(levelname)s %(message)s')
    chandler.setFormatter(cformatter)
    logging.getLogger('').addHandler(chandler)

    # Setup a file handler if requested
    if options.log_filename != None:
        fhandler = logging.FileHandler(options.log_filename)
        fhandler.setLevel(log_level)
        fformatter = logging.Formatter('%(asctime)s %(name)s %(levelname)s '\
                                           + '%(message)s')
        fhandler.setFormatter(fformatter)
        logging.getLogger('').addHandler(fhandler)

    # Logging for this function
    log = logging.getLogger('process_cmd_line')

    # Process the input files
    data = []

    for file in args:
        if not os.path.exists(file):
            log.error("The file '%s' does no exist", file)
        else:
            data.append(read_file(file))

    # Validate compatible BF setups
    for i in range(len(args)):
        if data[i][0] != data[0][0]:
            log.error("Incompatible BF setups in '%s' and '%s'", args[0], args[i])
            sys.exit(1)

    log.info("Read in all files.")

    merged_data = merge_data(data)

    log.info("Data merged.")

    aux = ""
    if options.aux != None:
        aux = options.aux

    write_files(merged_data, options.out_stem, aux)

    log.debug("Script complete")

if __name__ == "__main__":
    process_cmd_line(sys.argv[1:])

