# DFDL C Codegen

To run the DFDL examples in this repository, [Apache Daffodil](https://daffodil.apache.org/) must be [installed](https://daffodil.apache.org/releases/) from source and modified with the Scala file in this directory.

With Daffodil installed, to add this directory's file (or make any other modifications):

* Replace _/daffodil/daffodil-codegen-c/src/main/scala/org/apache/daffodil/codegen/c/generators/CodeGeneratorState.scala_ with the file in this directory

* In _/daffodil/_ run _sbt compile_ and then _sbt daffodil-cli/stage_

* Run Daffodil from _/daffodil/daffodil-cli/target/universal/stage/bin/daffodil_
