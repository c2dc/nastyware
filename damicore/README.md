# DAMICORE

  DAMICORE is an easy-to-use clustering and classification tool.

  DAMICORE is a pipeline of algorithms which is agnostic to the type of data
thanks to NCD, a compressor-based metric which views any piece of data as
simply a string of bits. This is particularly well-suited for heterogenous
datasets, datasets with difficult characteristics extraction and text datasets.

     Data    Metric      Distance matrix    Simplification Phylogenetic tree
    -----.           .---------------------.              .-----------------.
     x_1 |           |     x_1 x_2 ... x_n |              |   x_1-.  x_n    |
     x_2 |  .-----.  | x_1 d11 d12     d1n |    .----.    |        \  |     |
     x_3 |->| NCD |->| x_2 d21 d22     d2n |--->| NJ |--->|         O-O---- |-->
     ... |  '-----'  | ...                 |    '----'    |        /        |
     x_n |           | x_n dn1 dn2     dnn |              | x_2---'         |
    -----'           '---------------------'              '-----------------'
   
            Community detection        Clusters
    -----.                        .-----------------.
         |                        |  ___       ___  |
         |    .-------------.     | |x_1|     |   | |
    tree |--->| Fast Newman |---->| |x_2| ... |C_m| |
         |    '-------------'     | |x_n|     |   | |
         |                        | '---'     '---' |
    -----'                        '-----------------'


## Dependencies

### Installation
* python 3.10
* python-igraph (visit http://igraph.org/python for more informations)
* python-qt4
     sudo add-apt-repository ppa:rock-core/qt4
     sudo apt update
     sudo apt install libqt4-declarative libqt4* libqtcore4 libqtgui4 libqtwebkit4 qt4*

### Execution

  DAMICORE relies on compressors to calculate the distance between a pair of objects (files).

* gzip (available in almost all \*nix systems)
* bzip2 (available in almost all \*nix systems)

### Recommended

* FigTree (available in http://tree.bio.ed.ac.uk/software/figtree/)

## Usage

The simplest way to use DAMICORE is to provide as argument a directory
containing all files to be compared:

     $ ./damicore.py executable-functions/

This outputs a list of pairs of file name and corresponding cluster index.
For now we are lacking a good tool to visualize this clustering, but there are
other tools that might help. We can output intermediate steps into different
files for analysis:

     $ ./damicore.py executable-functions/ --ncd-output results/ncd-matrix.phylip --format phylip \
     --tree-output results/tree.newick --graph-image results/tree-image.png \
     --output results/final.clusters --compress gzip

This outputs the NCD matrix using PHYLIP format, the neighbor-joining tree in
Newick format (readable by FigTree), an image with a graph visualization and
the final clusters into another file.

By default, the script uses PPMd as compressor. If you don't have it installed,
try using gzip or bzip2:

     $ ./damicore.py executable-functions/ --compressor gzip

For more information on available options, see --help.

## Acknowledgment

Thanks to Bruno Kim who provided the initial python code.

## License

This software is licensed under the GPLv2.

Texts examples dataset collected by Francisco José Monaco (monaco@icmc.usp.br)

More informations in https://github.com/brunokim/damicore-python.
