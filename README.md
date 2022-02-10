# JSON Data Modeling
This code base is part of a research project to explore the effects of different RDF/JSON representations on querying Knowledge Graphs (KG). To this end, we designed and developed a parser to convert KG datasets from RDF/N-Triples syntax to the three different JSON collections (See Parsers directory). We ran our experiment using the publicly available Benchmarks by loading the different representations corresponding to a KG into different data managment systems by mapping it onto the relevant JSON documents. The benchmark queries (See Queries directory) were executed over each of the JSON representations separately and query execution times computed to analyze the effects of the different representations on KG query performance. Our results (See Results directory) indicate that the representation type has a significant performance impact on query execution.

Please note that to run the parsers, it is needed to set the path of input RDF/N-Triples KG dataset within the source code. The parser can be compiled using GNU C++ Compiler, e.g., g++ -std=c++11 SNV.cpp -o runSNV (e.g., SNV.cpp is the developed parser to generate the Subject-based Name/Value (SNV) representation of a given KG).


# Authors
* Masoud Salehpour (University of Sydney)
* Joseph G. Davis  (University of Sydney)

