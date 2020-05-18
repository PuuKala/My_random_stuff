# check_dependencies.py
# Author: Sami Karkinen
# This script requires graphviz to be installed into the environment path.
# P.S. If a co-worker wants refactoring(commenting+clarification of the code),
# the author can do it quickly.

from graphviz import Digraph
from os import listdir
from os.path import isfile

def check_if_subfolders(folder = '.'):
    non_hidden = [x for x in listdir(folder) if not x.startswith('.')]
    folders = [x for x in non_hidden if not isfile(folder + '/' + x)]
    return folders

def print_code_files(folder = '.'):
    subfolders = check_if_subfolders(folder)
    for subfolder in subfolders:
        print_code_files(folder + '/' + subfolder)
    c_files = [x for x in listdir(folder) if x.find('.c') >= 0]
    h_files = [x for x in listdir(folder) if x.find('.h') >= 0]
    print('.c files in folder', folder, ':', c_files)
    print('.h files in folder', folder, ':', h_files)

def make_digraph_from_deps(graph = Digraph(name='full_graph' ,format='png', strict=True), folder = '.'):
    subfolders = check_if_subfolders(folder)
    for subfolder in subfolders:
        graph = make_digraph_from_deps(graph, folder + '/' + subfolder)

    files = [x for x in listdir(folder) if x.find('.c') >= 0]
    files.extend([x for x in listdir(folder) if x.find('.h') >= 0])

    for file in files:
        with open(folder + '/' + file) as f:
            for line in f.readlines():
                if line.startswith('#include'):
                    dep = line.split()[1].split('.')[0]
                    if not dep.startswith('<'):
                        mod_name = file.split('.')[0]
                        dep = dep.strip('"')
                        if dep != mod_name:
                            graph.edge(mod_name, dep)

    return graph

def get_direct_deps(graph, top_node):
    deps = set()
    for i in range(len(graph.body)):
        if graph.body[i].split()[0] == top_node:
            deps.add(graph.body[i].split()[2])
    return deps

def make_subgraph_from_connections(graph, top_node, removes=[], reverse=False):
    nodes = set()
    new_nodes = set({top_node})
    while (nodes|new_nodes) != nodes:
        nodes = nodes|new_nodes
        new_nodes = set()
        for node in nodes:
            new_nodes = new_nodes|get_direct_deps(graph, node)
        for rm in removes:
            new_nodes.discard(rm)
    poplist = []
    for i in range(len(graph.body)):
        if reverse:
            node = graph.body[i].strip('\t').split()[2]
            dep = graph.body[i].strip('\t').split()[0]
        else:
            node = graph.body[i].strip('\t').split()[0]
            dep = graph.body[i].strip('\t').split()[2]
        if node not in nodes:
            poplist.append(i)
        elif dep in removes:
            poplist.append(i)
    poplist.sort(reverse=True)
    for i in poplist:
        graph.body.pop(i)
    return graph

if __name__ == '__main__':
    from argparse import ArgumentParser
    parser = ArgumentParser(description='Dot graph maker from .c and .h file dependencies. Does not include system dependencies(includes with <> instead of "").')
    parser.add_argument('-n', '--node', type=str, default='', help='Top node you want to make a subgraph from. If not given, only full graph is made.')
    parser.add_argument('-r', '--remove', type=str, default=[], nargs='*', help='Nodes you want to remove from the subgraph. Does not affect full graph.')
    parser.add_argument('-f', '--file', type=str, default='', help='File defining subgraph node and removes. Overrides node and remove commands.')
    parser.add_argument('--reverse', action='store_true', help='Checks reverse dependencies, aka. which nodes depend on the given node. Does not affect full graph.')
    args = parser.parse_args()
    parser.print_help()
    graph = make_digraph_from_deps()
    graph.render(cleanup=True)
    if args.file:
        with open(args.file) as f:
            node = f.readline().strip()
            removes = f.read().split()
    elif args.node:
        node = args.node
        removes = args.remove
    else:
        exit()
    
    subgraph = make_subgraph_from_connections(graph, node, removes, args.reverse)
    subgraph.render(filename=(node + '.gv'), cleanup=True)