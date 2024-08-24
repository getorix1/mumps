March 28, 2024

ReadMe for the Mumps version

Information Storage and Retrieval examples using the Multi-Dimensional 
and Hierarchical Toolkit: a C++ library of Mumps functionality.

There are two versions of this software. One is written in Mumps and the
the other is written in C++ using the MDH class library which provides Mumps-like
facilities in C++.

The Mumps version is in the directory 'Mumps-version' and the MDH version
is in the directory 'MDH-version'. This README file is used by both versions.

In the Mumps version, you have the option to run the Mumps interpretively
or to compile the Mumps and run the result. To specify compilation
(the default) or interpretation, modify the 'compile' bash variable in
the file 'control.script'. Make compile equal 'yes' for compiled execution
and 'no' for interpretive execution. At present, the retrieval phase is
written for compile only.

You should not expect the same results from the Mumps versus the MDH version
as the algorithms differ. 

Both versions split the database into segments and index/retrieve from the
segments in parallel. The number of segments is the number of available CPUs.

-----

Install Mumps with the native single user database option.

Note: differences in results between the MDH and Mumps versions of this
software are due to differences in settings and algorithms.

Please make all files with the endings .script and .mps executable 
if you want to run the examples.

Quick start:

    Install Mumps (see Mumps-Language-Processors directory)
    for the single user native database.

    Next (in the appropriate directory)

        make -j 4 -B

    Where -j 4 means to run four compiles in parallel (increase or
    decrease depending on your computer's number of cpus).

    Where -B means compile everything.

Index a portion of the database:

    The text database is in the parent of the appropriate directory and has the file
    name 'ohsu.medline'.

    The first time this database is accessed, it will be converted to the
    files 'ohsu.converted' and 'titles.data'. This may take some time depending 
    on your system.  The converted database also resides in the parent directory.
    The conversion will not take place again unless you delete this file.

    To index the first 5000 documents in the database, type:

        ./control.script 5000

    The number shown is the number of documents from the data base that will 
    be indexed.  The indexing will be done in X segments where X is the number of
    CPUs on your machine. 

    If you fail to specifiy a number of documents, 1000000 will be used

    The indexing uses the files 'ohsu.converted' and 'titles.data'.

Retrieval

    You may run a test retrieval with retrieve.script which uses the term
    in the file tstquesy. Since your test database is very small, don't
    expect much in the way of results.

    The script file 'retrieve.script' will run the retrieval. Do not use the
    similarly named executable files as they will not work by themselves.

    The programs in 'retrieve.script' expects the name of a file containing a
    search query. If none is provided, it uses the file 'tstquery'.

    A query file consists of one line of keywords separated by blanks. The
    combination of the keywords is the query vector.

    The first time you run the code, the ohsu.medline file will be converted to
    ohsu,converted and this file is then used for indexing operations. The original
    file is unchanged.

OHSU Medline Database

    The original OHSU Medline database was obtained from:

        https://trec.nist.gov/data/t9_filtering.html

    And reformatted.

    From the website:

    "... The OHSUMED test collection is a set of 348,566 references from
    MEDLINE, the on-line medical information database, consisting of
    titles and/or abstracts from 270 medical journals over a five-year
    period (1987-1991). The available fields are title, abstract, MeSH
    indexing terms, author, source, and publication type. The National
    Library of Medicine has agreed to make the MEDLINE references in the
    test database available for experimentation, restricted to the
    following conditions:

    1. The data will not be used in any non-experimental clinical,
    library, or other setting.

    2.  Any human users of the data will explicitly be told that the data
    is incomplete and out-of-date.

    The OHSUMED document collection was obtained by William Hersh
    (hersh@OHSU.EDU) and colleagues for the experiments described in the
    papers below:

    Hersh WR, Buckley C, Leone TJ, Hickam DH, OHSUMED: An interactive
    retrieval evaluation and new large test collection for research, 
    Proceedings of the 17th Annual ACM SIGIR Conference, 1994, 192-201.

    Hersh WR, Hickam DH, Use of a multi-application computer workstation
    in a clinical setting, Bulletin of the Medical Library Association,
    1994, 82: 382-389. ..."

    Full details on the original format are at the web site referenced above.
