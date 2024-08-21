
An xbnf parser that accpets two endian primitives for fixed-width integers:

* `big_endian(<integer>, <type>)`
* `little_endian(<integer>, <type>)`
where `<type>` is either `int16`, `int32`, `int64`, `uint16`, `uint32`, or `uint64`
